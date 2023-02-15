#pragma once
#include <set>
#include <string>
class IAutomaton
{
private:
	std::set<std::string> m_states;
	std::set<char> m_symbols;
	std::set<std::string> m_finalStates;
	std::set<std::tuple<std::string, char, std::string>> m_transitions;
	std::string m_startState;
public:
	virtual bool verify() = 0;
	virtual bool checkWord(const std::string& word) = 0;
	virtual bool isDeterministic() = 0;
};

