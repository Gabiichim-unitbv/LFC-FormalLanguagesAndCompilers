#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <set>
#include "IAutomaton.h"


#define lambda 'λ'

class FiniteAutomaton : public IAutomaton
{
private:
	
	std::set<std::string> m_states;
	std::set<char> m_symbols;
	std::set<std::string> m_finalStates;
	std::set<std::tuple<std::string, char, std::string>> m_transitions;
	std::string m_startState;

	bool isFinalState(const std::string& states);
public:
	FiniteAutomaton() = default;

	FiniteAutomaton(std::set<std::string> states, std::set<char> alphabet,
		std::string startState, std::set<std::tuple<std::string, char, std::string>> transitions);
	~FiniteAutomaton();

	bool AreLambdaTransitionsLeft(const std::set<std::string>& states) const;
	bool verify();
	bool inefficientCheckWord(const std::string& word);
	bool checkWord(const std::string& word);
	bool isDeterministic();
	void removeTransition(const std::tuple<std::string, char, std::string>& trans);

	void showAlphabet();

	std::set<std::string> transition(const std::set<std::string>& states, const char& input);
	std::set<std::string> lambdaClosure(const std::string& state);

	void setTransition(const std::string& initialState, const char& symbol, const std::string& nextState);
	void setTransition(std::tuple<std::string, char, std::string> transition);
	void setStates();
	void setFinalStates(const std::set<std::string>& finalStates);
	void setSymbols();
	void setStartState(const std::string& startState);
	

	std::string getStartState() const;
	std::set<std::string> getFinalStates() const;
	std::set<char> getSymbols() const;
	std::set < std::tuple<std::string, char, std::string>> getTransitions() const;
	std::set<std::string> getStates() const;
	int size() const;

	friend std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton);
};

