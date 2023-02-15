#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <sstream>
#include <regex>
#include <set>
#include "Productions.h"
#include "FiniteAutomaton.h"
class Grammar
{
private:
	std::vector<char> m_neterminals;
	std::vector<char> m_terminals;
	char m_startSymbol;
	std::vector<Productions> m_productions;
	std::set<std::string> m_generatedWords;

	template<typename T, typename U>
	 U randomize(T list) // template randomize function
	{
		
		std::random_device randomDevice;
		std::mt19937 eng(randomDevice());
		std::uniform_int_distribution<> distr(0, list.size() - 1);
		return list[distr(eng)];
	}

	bool isTerminal(const char& ch);
	bool isAccepted(const std::string& word);
	std::vector<std::tuple<char, char, char>> convertProductionsToTransitions(); // automaton related function
public:
	Grammar() = default;


	friend std::istream& operator>>(std::istream& in, Grammar& gr);
	friend std::ostream& operator<<(std::ostream& out, Grammar& gr);

	
	std::string generateWord();
	bool verify();
	bool isRegular();
	void displayGenerated();

	FiniteAutomaton getAutomaton();
};

