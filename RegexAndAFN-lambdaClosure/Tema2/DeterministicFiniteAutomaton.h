#pragma once
#include <vector>
#include <string>
#include <iostream>
#include<set>
#include<map>
#include <stack>
#include "IAutomaton.h"
#include "FiniteAutomaton.h"

class DeterministicFiniteAutomaton : public IAutomaton
{
private:
	std::set<std::string> m_states;
	std::set<char> m_alphabet;
	std::set<std::tuple<std::string, char, std::string>> m_transitions; // initialState, symbol, nextState
	std::string m_startState;
	std::set<std::string> m_finalStates;

	void findLambdaStates(const FiniteAutomaton& automaton, std::set<std::string>& nedetStates);
	std::set<std::string> readSymbol(const char& symbol, const std::set<std::string>& states, const FiniteAutomaton& automaton);
	bool isState(const std::string& ch);
	bool isSymbol(const char& ch);

public:

	DeterministicFiniteAutomaton() = default;

	DeterministicFiniteAutomaton(std::set<std::string> states, std::set<char> alphabet, std::string startState, std::set<std::tuple<std::string, char, std::string>> transitions, std::set<std::string> finalStates);
	~DeterministicFiniteAutomaton();

	bool verify();
	bool checkWord(const std::string& word);
	bool isDeterministic();

	void convertFromNedeterministic(const FiniteAutomaton& nAutomaton);


	friend std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomaton& automaton);

};