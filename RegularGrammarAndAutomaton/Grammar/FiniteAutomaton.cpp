#include "FiniteAutomaton.h"

bool FiniteAutomaton::isState(const char& ch)
{
    for (const auto& c : m_states)
        if (c == ch)
            return true;
    return false;
}

bool FiniteAutomaton::isAlpha(const char& ch)
{
    for (const auto& c : m_alphabet)
        if (c == ch)
            return true;
    return false;
}

FiniteAutomaton::FiniteAutomaton(std::vector<char> states, std::vector<char> alphabet, char startState, std::vector<std::tuple<char, char,char>> transitions)
    : m_states{states}, m_alphabet {alphabet}, m_startState{ startState }, m_transitions {transitions}
{}

FiniteAutomaton::~FiniteAutomaton()
{}

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

        // checking the if the transitions are correct 
        for (const auto& trans : m_transitions)
        {
            auto& [initialState, alphabetElement, finalState] = trans;
            if (!isAlpha(alphabetElement))
                return false;

            if(!isState(initialState) && !isState(finalState))
                return false;
        }
}

bool FiniteAutomaton::checkWord(const std::string& word)
{
    std::vector<char> possibleStates;
    std::vector<char> currentStates;
    currentStates.push_back(m_startState);

    for (const auto& ch : word)
    {
        for (const auto& state : currentStates) // iterating through states vector
        {
            for (const auto& trans : m_transitions)
            {
                auto& [initialState, alphabetElement, finalState] = trans;
                if (initialState == state && alphabetElement == ch)
                    possibleStates.push_back(finalState); // getting all the possible states from the current states in the vector
            }
        }
        if (possibleStates.size() == 0) // checking if it blocked
            return false; 

        currentStates = possibleStates; // now the possible states become the new current states
        possibleStates.clear();
    }

    bool found = false;
    for (const auto& state : currentStates) // checking if the last current states vector contain the final state
        if (state == m_finalState)
        {
            found = true;
            return found;
        }

    return found;
}

bool FiniteAutomaton::isDeterministic()
{
    std::map<std::pair<char, char>, int> countTransitions; // checking if a state goes with the same alpha in 2 different states
    for (const auto& trans : m_transitions)
    {
        auto& [initialState, alphabetElement, finalState] = trans;
        std::pair<char, char> transition = std::make_pair(initialState, alphabetElement);
        countTransitions[transition]++;                      
        if (countTransitions[transition] > 1)             
            return false;
    }
    return true;
}

void FiniteAutomaton::showAlphabet()
{
    for (const auto& alpha : m_alphabet)
        std::cout << alpha << ' ';
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton)
{
    out << "\nStates: ";
    for (const auto& state : automaton.m_states)
        out << state << ' ';
    out << automaton.m_finalState;
    out << "\nAlphabet: ";
    for (const auto& alpha : automaton.m_alphabet)
        out << alpha << ' ';
    out << "\nStarting state: " << automaton.m_startState;

    out << "\nTransitions:\n";

    for (const auto& transition : automaton.m_transitions)
    {
        auto& [initialState, alphabetElement, finalState] = transition;
        out << "(" << initialState << ", " << alphabetElement << ") = " <<finalState << '\n';
    }
    return out;
}
