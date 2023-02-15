#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include "Grammar.h"
#include "Production.h"

using namespace std;

class PushDownAutomaton {
private:
    std::unordered_set<std::string> m_states;
    std::unordered_set<std::string> m_alphabet;
    std::unordered_set<string> m_stackAlphabet;
    std::string m_initialState;
    std::vector<std::string> m_finalStates;
    std::vector < std::tuple < std::string, std::string, std::string >> m_transitions;

public:
    PushDownAutomaton(std::vector<Production> transitions, std::unordered_set<std::string> alphabet, std::unordered_set<std::string> stackAlphabet, std::string initialState);

    std::vector<std::tuple<std::string, std::string, std::string>> 
        createTransitions(std::vector<Production> transitions);

    bool checkWord(std::string word) const;

    void displayAutomaton();

    bool isDeterministic() const;
};