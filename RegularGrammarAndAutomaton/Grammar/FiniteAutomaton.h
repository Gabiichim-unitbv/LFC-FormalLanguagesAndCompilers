#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>

class FiniteAutomaton
{
private:
	std::vector<char> m_states;
	std::vector<char> m_alphabet;
	std::vector<std::tuple<char, char, char>> m_transitions; // initialState, alphabetElement, finalState
	char m_startState;
	char m_finalState = 'T';

	bool isState(const char& ch);
	bool isAlpha(const char& ch);
public:
	FiniteAutomaton() = default;

	FiniteAutomaton(std::vector<char> states, std::vector<char> alphabet, char startState, std::vector<std::tuple<char, char, char>> transitions);
	~FiniteAutomaton();

	bool verify();
	bool checkWord(const std::string& word);
	bool isDeterministic();

	void showAlphabet();

	friend std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton);
};

