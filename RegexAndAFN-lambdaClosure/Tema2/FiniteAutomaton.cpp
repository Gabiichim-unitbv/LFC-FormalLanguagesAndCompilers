#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton(std::set<std::string> states, std::set<char> alphabet, std::string startState, std::set<std::tuple<std::string, char, std::string>> transitions)
    : m_states{states}, m_symbols {alphabet}, m_startState{ startState }, m_transitions {transitions}
{}

FiniteAutomaton::~FiniteAutomaton()
{}

bool FiniteAutomaton::isFinalState(const std::string& state)
{
            if (m_finalStates.find(state) != m_finalStates.end())
                return true;
    return false;
}

bool FiniteAutomaton::AreLambdaTransitionsLeft(const std::set<std::string>& states) const
{
    for (const auto& state : states)
    {
        for (const auto& trans : m_transitions)
        {
            auto const& [initialState, symbol, nextState] = trans;
            if (initialState == state && symbol == lambda)
                return true;
        }
    }
    return false;
}

bool FiniteAutomaton::verify()
{
    // checking if has a transition from startState
    bool hasStartTransition = false;
    for (const auto& trans : m_transitions)
    {
        auto& [initialState, alphabetElement, finalState] = trans;
        if (initialState == m_startState)
        {
            hasStartTransition = true;
            break;
        }
    }
    if (!hasStartTransition)
        return false;

        for (auto t : m_transitions) 
        {
            if (std::get<1>(t) == lambda && isFinalState(std::get<2>(t)))
            {
                return false;
            }
        }

        if(m_finalStates.size() == 0) 
        {
            return false;
        }
        // Check that there is at least one transition
        if (m_transitions.size() == 0) 
        {
            return false;
        }
    
        return true;
}

bool FiniteAutomaton::inefficientCheckWord(const std::string& word)
{
    std::vector<std::string> possibleStates;
    std::vector<std::string> currentStates;
    currentStates.push_back(m_startState);

    std::string copyWord = word;
    int count = 0;
    for (int i = 0; i < copyWord.size(); i++)
    {
        bool wasLambda = false;
        bool wasCh = false;
        auto oldStates = currentStates;
        for (const auto& state : currentStates) // iterating through states vector
        {
            for (const auto& trans : m_transitions)
            {
                
                auto& [initialState, alphabetElement, nextState] = trans;
                if (initialState == state)
                {
                    if (alphabetElement == copyWord[i])
                    {
                        possibleStates.push_back(nextState);
                        wasCh = true;
                    } // getting all the possible states from the current states in the vector
                    else
                        if (alphabetElement == lambda)
                    {
                        possibleStates.push_back(nextState);
                            wasLambda = true;
                    }
                }
            }
        }
        if (wasLambda && !wasCh)
            i--;
        count++;
        

        if (possibleStates.size() == 0) // checking if it blocked
            return false;

        currentStates = possibleStates; // now the possible states become the new current states
        possibleStates.clear();

        //if (oldStates == currentStates)
        //    break;
    }

    bool found = false;
    for (const auto& state : currentStates) // checking if the last current states vector contain the final state
        if (m_finalStates.find(state) != m_finalStates.end())
        {
            found = true;
            return found;
        }

    return found;
}
    

bool FiniteAutomaton::checkWord(const std::string& word)
{
    std::set<std::string> currentStates = lambdaClosure(m_startState);
    
    for (const auto& ch : word)
    {
        std::set<std::string> nextStates;
        for (const auto& state : currentStates)
        {
            std::set<std::string> lambdaS = lambdaClosure(state);

            nextStates.insert(lambdaS.begin(), lambdaS.end());
        }
        currentStates = transition(nextStates, ch);
    }
    for (auto state : currentStates) {
        if (m_finalStates.find(state) != m_finalStates.end()) {
            return true;
        }
    }
    return false;
}

bool FiniteAutomaton::isDeterministic()
{

    std::map<std::string, int> transCount;
    for (const auto& trans : m_transitions)
    {
        auto const& [initialState, symbol, nextState] = trans;
        transCount[initialState]++;
    }
    
    for (const auto& count : transCount)
        if (count.second > 1)
            return false;
    return true;
}

void FiniteAutomaton::removeTransition(const std::tuple<std::string, char, std::string>& trans)
{
    m_transitions.erase(trans);
}

void FiniteAutomaton::showAlphabet()
{
    for (const auto& alpha : m_symbols)
        std::cout << alpha << ' ';
}

std::set<std::string> FiniteAutomaton::transition(const std::set<std::string>& states, const char& input)
{
    std::set<std::string> result;
    for (const auto& state : states) 
    {
        for (const auto& t : m_transitions) 
        {
            if (std::get<0>(t) == state && std::get<1>(t) == input) 
            {
                    result.insert(std::get<2>(t));
            }
        }
    }
    return result;
}

std::set<std::string> FiniteAutomaton::lambdaClosure(const std::string& state)
{
    std::set<std::string> result;
    std::set<std::string> toVisit;
    toVisit.insert(state);
    while (!toVisit.empty()) {
        std::string current = *toVisit.begin();
        toVisit.erase(toVisit.begin());
        result.insert(current);
        for (auto t : m_transitions) {
            if (std::get<0>(t) == current && std::get<1>(t) == lambda) {
                    if (result.find(std::get<2>(t)) == result.end()) {
                        toVisit.insert(std::get<2>(t));
                    
                }
            }
        }
    }
    return result;
}

void FiniteAutomaton::setTransition(const std::string& initialState, const char& symbol, const std::string& nextState)
{
    m_transitions.insert(std::make_tuple(initialState, symbol, nextState));
}

void FiniteAutomaton::setTransition(std::tuple<std::string, char, std::string> transition)
{
    m_transitions.insert(transition);
}


void FiniteAutomaton::setFinalStates(const std::set<std::string>& finalStates)
{
    for (const auto& state : finalStates)
        if (m_states.find(state) == m_states.end())
            throw(std::exception("Final state not present in our states"));

    for (const auto& ch : finalStates)
        m_finalStates.insert(ch);
}

void FiniteAutomaton::setSymbols()
{
    for (const auto& trans : m_transitions)
        if(std::get<1>(trans) != lambda)
        m_symbols.insert(std::get<1>(trans));
}

void FiniteAutomaton::setStates()
{
    for (const auto& trans : m_transitions)
    {
        m_states.insert(std::get<0>(trans));
        m_states.insert(std::get<2>(trans));
    }
}

void FiniteAutomaton::setStartState(const std::string& startState)
{
    bool flag = false;
    for (const std::string& state : m_states)
        if (startState == state)
            flag = true;

    if (!flag)
        throw(std::exception("Start state not presents in states"));

    for(const auto& state : m_finalStates)
        if(startState == state)
            throw(std::exception("Start state cannot be a final state"));

    m_startState = startState;
}

std::string FiniteAutomaton::getStartState() const
{
    return m_startState;
}

std::set<std::string> FiniteAutomaton::getFinalStates() const
{
    return m_finalStates;
}

std::set<char> FiniteAutomaton::getSymbols() const
{
    return m_symbols;
}

std::set<std::tuple<std::string, char, std::string>> FiniteAutomaton::getTransitions() const
{
    return m_transitions;
}

std::set<std::string> FiniteAutomaton::getStates() const
{
    return m_states;
}

int FiniteAutomaton::size() const
{
    return m_transitions.size();
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton)
{
  
    out << "\nStates: ";
    for (const auto& state : automaton.m_states)
        out <<"q" + state << ' ';

    out << "\nAlphabet: ";
    for (const auto& alpha : automaton.m_symbols)
        out << alpha << ' ';

    out << "\nStarting state: " <<"q" + automaton.m_startState;

    out << "\nTransitions:\n";
    for (const auto& transition : automaton.m_transitions)
    {
        auto& [initialState, symbol, nextState] = transition;
        out << "(q" << initialState << ", " << symbol << ") = q" << nextState << '\n';
    }
    return out;
}
