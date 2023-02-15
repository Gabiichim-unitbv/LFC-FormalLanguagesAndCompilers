#include "DeterministicFiniteAutomaton.h"

void DeterministicFiniteAutomaton::findLambdaStates(const FiniteAutomaton& automaton, std::set<std::string>& nedetStates)
{
    std::set<std::string> visitedStates = nedetStates;
    std::set<std::string> toRemoveStates = nedetStates;
    while (automaton.AreLambdaTransitionsLeft(toRemoveStates))
    {
        for (const auto& state : nedetStates)
        {
            for (const auto& trans : automaton.getTransitions())
            {
                const auto& [initialState, symbol, nextState] = trans;
                if (state == initialState && symbol == lambda)
                {
                    nedetStates.insert(nextState);
                    visitedStates.insert(nextState);
                }
            }
        }
        for (const auto& state : toRemoveStates)
            nedetStates.erase(state);

        toRemoveStates = nedetStates;
    }

    nedetStates = visitedStates;
}

std::set<std::string> DeterministicFiniteAutomaton::readSymbol(const char& symbol, const std::set<std::string>& states, const FiniteAutomaton& automaton)
{
    std::set<std::string> delta;

    for(const auto& state : states)
        for (const auto& trans : automaton.getTransitions())
        {
            const auto& [initialState, sym, nextState] = trans;
            if (state == initialState && symbol == sym)
                delta.insert(nextState);
        }
    return delta;
}

bool DeterministicFiniteAutomaton::isState(const std::string& ch)
{
    for (const auto& state : m_states)
        if (state == ch)
            return true;
    return false;
}

bool DeterministicFiniteAutomaton::isSymbol(const char& ch)
{
    for (const auto& symbol : m_alphabet)
        if (symbol == ch)
            return true;
    return false;
}


DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(std::set<std::string> states, std::set<char> alphabet, std::string startState, std::set<std::tuple<std::string, char, std::string>> transitions, std::set<std::string> finalState)
    : m_states{ states }, m_alphabet{ alphabet }, m_startState{ startState }, m_transitions{ transitions }, m_finalStates {finalState}
{}

DeterministicFiniteAutomaton::~DeterministicFiniteAutomaton()
{}

bool DeterministicFiniteAutomaton::verify()
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

    // checking the if the transitions are correct 
    for (const auto& trans : m_transitions)
    {
        auto& [initialState, alphabetElement, finalState] = trans;
        if (!isSymbol(alphabetElement))
            return false;

        if (!isState(initialState) && !isState(finalState))
            return false;
    }
}

bool DeterministicFiniteAutomaton::checkWord(const std::string& word)
{
    std::vector<std::string> possibleStates;
    std::vector<std::string> currentStates;
    currentStates.push_back(m_startState);

    for (const auto& ch : word)
    {
        for (const auto& state : currentStates) // iterating through states vector
        {
            for (const auto& trans : m_transitions)
            {
                auto& [initialState, alphabetElement, nextState] = trans;
                if (initialState == state && alphabetElement == ch)
                {
                    possibleStates.push_back(nextState);  
                } 
            }
        }
        if (possibleStates.size() == 0) // checking if it blocked
            return false;

        currentStates = possibleStates;
        possibleStates.clear();

    }
    for(const auto& state1 : currentStates)
        for(const auto& state : m_finalStates)
            if(state1 == state)
        return true;

    return false;
}

bool DeterministicFiniteAutomaton::isDeterministic()
{
    std::map<std::string, int> transCount;
    for (const auto& trans : m_transitions)
    {
        auto const& [initialState, symbol, nextState] = trans;
        transCount[initialState]++;
    }

    for (const auto& count : transCount)
        if (count.second > 1)
            return true;
    return false;
}

void DeterministicFiniteAutomaton::convertFromNedeterministic(const FiniteAutomaton& nAutomaton)
{
    std::stack <std::set<std::string>> newStates;
    std::multimap<std::set<std::string>, std::tuple<std::string, char, std::string>> result;
    std::set<std::string> nedetStates;

    nedetStates.insert(nAutomaton.getStartState());
    findLambdaStates(nAutomaton, nedetStates);
    newStates.push(nedetStates);

    int contor = 0;
    int contorTo = contor;
    std::set<std::string> lastResult;
    do 
    {

        std::set<std::string> last;
        if (newStates.size() >= 2)
        {
            auto toPutBack = newStates.top();
            newStates.pop();
            last = newStates.top();
            newStates.pop();
            newStates.push(toPutBack);
        }
        else
        {
            last = newStates.top();
            newStates.pop();
        }
        //lastResult = last;

        for (const auto& symbol : nAutomaton.getSymbols())
        {
            auto delta = readSymbol(symbol, last, nAutomaton);
            if (delta.size() == 0)
                continue;

            findLambdaStates(nAutomaton, delta);

            if (result.find(delta) == result.end())
            {
                newStates.push(delta);
                result.insert({ delta , {"q'" + std::to_string(contor), symbol, "q'" + std::to_string(contorTo + 1)} });
                contorTo++;
            }
            else
            {
                result.insert({ delta, {"q'" + std::to_string(contor), symbol, std::get<2>(result.find(delta)->second)} }); // { delta, {qcontor, symbol, qcontor deja existent} }
            }

        }
        contor++;
    } 
    while (!newStates.empty());

   
        //std::cout << "AFD Corect\n\n\n";
        for (int i = 0; i < contor; i++)
            m_states.insert("q'" + std::to_string(i));
        m_alphabet = nAutomaton.getSymbols();
        m_startState = "q'0";

        for (const auto& x : result)
        {
            auto states = x.first;
            for (const auto& state : states)
                if (state == *nAutomaton.getFinalStates().begin())
                {
                    m_finalStates.insert(std::get<2>(x.second));

                }
        }

        for (const auto& x : result)
        {
            const auto& [lambdaStates, transition] = x;
            m_transitions.insert(transition);
        }
    

    auto& res = result;
}




std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomaton& automaton)
{
    out << "\nStates: ";
    for (const auto& state : automaton.m_states)
        out << state << ' ';
    out << "\nAlphabet: ";
    for (const auto& alpha : automaton.m_alphabet)
        out << alpha << ' ';
    out << "\nStarting state: " << automaton.m_startState;
   
    out << "\nFinal states: ";
    for (const auto& state : automaton.m_finalStates)
        std::cout << state << ' ';

    out << "\nTransitions:\n";

    for (const auto& transition : automaton.m_transitions)
    {
        auto& [initialState, alphabetElement, finalState] = transition;
        out << "(" << initialState << ", " << alphabetElement << ") = " << finalState << '\n';
    }
    return out;
}

