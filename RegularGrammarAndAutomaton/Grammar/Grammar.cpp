#include "Grammar.h"



std::istream& operator>>(std::istream& in, Grammar& gr)
{
    // read neterminals
    {int neterminalsNumber;
    in >> neterminalsNumber;
    for (int i = 0; i < neterminalsNumber; i++)
    {
        char neterminal;
        in >> neterminal;
        gr.m_neterminals.push_back(neterminal);
    }
   
    // read terminals
    int terminalsNumber;
    in >> terminalsNumber;
    for (int i = 0; i < terminalsNumber; i++)
    {
        char neterminal;
        in >> neterminal;
        gr.m_terminals.push_back(neterminal);
    }

    // read start simbol
    in >> gr.m_startSymbol;

       // productions read
    }
    int prodNumber;
    in >> prodNumber;
    in.ignore();
    for (int i = 0; i < prodNumber; i++)
    {
        Productions production;
        in >> production;
        gr.m_productions.push_back(production);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, Grammar& gr)
{
    // display neterminals
    out << "Neterminals: ";
    for (const auto& it : gr.m_neterminals)
        out << it << ' ';

 
    // display terminals
    out << "\nTerminals: ";
    for (const auto& it : gr.m_terminals)
        out << it << ' ';


    // display the start simbol
    out << "\nStart symbol: ";
    out << gr.m_startSymbol;



    // display productions
    out << "\nProductions:\n";
    for (const auto& it : gr.m_productions)
        out << it << '\n';

    return out;
}

bool Grammar::isTerminal(const char& ch)
{
    for (const auto& terminal : m_terminals)
        if (ch == terminal)
            return true;
    return false;
}

bool Grammar::isAccepted(const std::string& word)
{
    for (const auto& ch : word)
        if (!isTerminal(ch))
            return false;
    return true;
}

std::vector<std::tuple<char, char, char>> Grammar::convertProductionsToTransitions()
{
    std::vector<std::tuple<char, char, char>> transitions;
    for (auto prod = m_productions.begin(); prod != m_productions.end(); prod++)
    {
        char currentState;
        if (prod->getRightMember().size() > 1)
            currentState = prod->getRightMember()[1];
        else
            currentState = 'T';
        std::tuple<char, char, char> trans(prod->getLeftMember()[0], prod->getRightMember()[0], currentState);
        transitions.push_back(trans);
    }

    return transitions;
}


std::string Grammar::generateWord()
{
    // starting from S symbol
    std::string word(1, m_startSymbol);
    Productions randProduction = randomize<std::vector<Productions>, Productions>(m_productions);
    while(randProduction.getLeftMember()[0] != m_startSymbol)
       randProduction = randomize<std::vector<Productions>, Productions>(m_productions);

    word = std::regex_replace(word, std::regex(word), randProduction.getRightMember());
    std::cout << m_startSymbol << " => " << word;

    if (isAccepted(word)) // if the string is made of only terminals is accepted
        m_generatedWords.insert(word);
    else
    {
        while (!isAccepted(word))
        {
            char randomNeterminal = randomize <std::string, char>(word); // choosing random neterminal
            while (isTerminal(randomNeterminal))
               randomNeterminal = randomize <std::string, char>(word);


            
            Productions randProduction = randomize<std::vector<Productions>, Productions>(m_productions); // choosing random production
            while (randProduction.getLeftMember()[0] != randomNeterminal)
                randProduction = randomize<std::vector<Productions>, Productions>(m_productions);

            word = std::regex_replace(word, std::regex(std::string(1, randomNeterminal)), randProduction.getRightMember());
            std::cout <<" => " << word;
        }
        std::cout << '\n';
        int lastSize = m_generatedWords.size();
        m_generatedWords.insert(word);
        if (lastSize == m_generatedWords.size()) // set container ignores duplicates so if the size didn't changed you can tell here was generated a duplicate
        {
            std::cout << "This word was already generated!\nTrying again...\n";
            generateWord();
        }
    }

 
    return word;
}

bool Grammar::verify()
{
    // 1) VN n VT = Ø
    for (const auto& neterminal : m_neterminals)
        for (const auto& terminal : m_terminals)
            if (neterminal == terminal)
                return false;

    // 2) S ∈ VN
    bool existStartSimbol = false;
    for (const auto& neterminal : m_neterminals)
        if (neterminal == m_startSymbol)
        {
            existStartSimbol = true;
            break;
        }
    if (!existStartSimbol)
        return false;


  
    // 3) pentru fiecare regulă, membrul stâng conține cel puțin un neterminal
    for (auto prod = m_productions.begin(); prod != m_productions.end(); prod++)
    {
        std::string leftMember = prod->getLeftMember();
        
        for (const auto& chLeft : leftMember)
        {
            bool hasNeterminal = false;
            for (const auto& neterminal : m_neterminals)
                if (neterminal == chLeft)
                {
                    hasNeterminal = true;
                    break;
                }

            if (!hasNeterminal)
                return false;
        }
    }

    // 4) există cel puțin o producție care are în stânga doar S
    bool foundProduction = false;
    for (auto prod = m_productions.begin(); prod != m_productions.end() && !foundProduction; prod++)
    {
        std::string leftMember = prod->getLeftMember();
        if (leftMember.size() == 1 && leftMember[0] == m_startSymbol)
            foundProduction = true;
    }

    if (!foundProduction)
        return false;

    // 5) fiecare producție conține doar elemente din VN și VT
    for (auto prod = m_productions.begin(); prod != m_productions.end(); prod++)
    {
        // checking leftMember
        for (const auto& chLeft : prod->getLeftMember())
        {
            bool exist = false;
            for (const auto& neterminal : m_neterminals)
                if (chLeft == neterminal)
                {
                    exist = true;
                    break;
                }

            if (exist)
                break;
            else
                for (const auto& terminal : m_terminals)
                    if (chLeft == terminal)
                    {
                        exist = true;
                        break;
                    }
            if (exist)
                break;
            else
                return false;
        }

        // checking rightMember
        for (const auto& chRight : prod->getRightMember())
        {
            bool exist = false;
            for (const auto& neterminal : m_neterminals)
                if (chRight == neterminal)
                {
                    exist = true;
                    break;
                }

            if (exist)
                break;
            else
                for (const auto& terminal : m_terminals)
                    if (chRight == terminal)
                    {
                        exist = true;
                        break;
                    }
            if (exist)
                break;
            else
                return false;
        }
    }

    return true;
}

bool Grammar::isRegular()
{
    for (auto prod = m_productions.begin(); prod != m_productions.end(); prod++)
    {
        std::string leftMember = prod->getLeftMember();
        std::string rightMember= prod->getRightMember();
        if (leftMember.size() > 1 || isTerminal(leftMember[0]))
            return false;

        if (rightMember.size() > 2)
            return false;

            if (rightMember.size() > 1)
            {
                if (!isTerminal(rightMember[0]))
                    return false;

                if (isTerminal(rightMember[1]))
                    return false;
            }
                else
                    if(!isTerminal(rightMember[0]))
                        return false;
    }
    return true;
}

void Grammar::displayGenerated()
{
    std::cout << "\n_GENERATED WORDS_\n";
    for (const auto& word : m_generatedWords)
        std::cout << "{ " << word << " }, ";
    std::cout << '\n\n';
    std::cout << std::endl;
}

FiniteAutomaton Grammar::getAutomaton()
{
    std::vector<std::tuple<char, char, char>> transitions = convertProductionsToTransitions();
    return FiniteAutomaton(m_neterminals, m_terminals, m_startSymbol, transitions);
}


