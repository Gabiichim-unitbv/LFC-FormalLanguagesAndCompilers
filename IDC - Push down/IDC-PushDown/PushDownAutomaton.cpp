#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include "Grammar.h"
#include "PushDownAutomaton.h"

using namespace std;

PushDownAutomaton::PushDownAutomaton(std::vector<Production> transitions, std::unordered_set<std::string> alphabet, std::unordered_set<std::string> stackAlphabet, std::string initialState)
{
	m_transitions = createTransitions(transitions);
	m_alphabet = alphabet;
	m_stackAlphabet = stackAlphabet;
	m_initialState = initialState;
	m_initialState = "q0";
	m_states.insert(m_initialState);
	m_finalStates.push_back("empty");
}

std::vector < std::tuple < std::string, std::string, std::string >>
PushDownAutomaton::createTransitions(std::vector<Production> transitions)
{
	std::vector < std::tuple < std::string, std::string, std::string >> trans;
	for (const auto& transition : transitions)
	{

		std::string leftSide;
		leftSide = transition.first();

		std::string rightSide;
		rightSide = transition.second();

		std::string terminal;
		terminal = rightSide[0];


		if (rightSide.size() > 1)
		{
			std::string valueForStack;
			valueForStack = rightSide.substr(1);

			trans.push_back({ terminal,leftSide,valueForStack });
		}
		else {
			trans.push_back({ terminal,leftSide,"lambda" });
		}

	}
	return trans;
}

bool PushDownAutomaton::checkWord(std::string word) const {
	return true;
}

void PushDownAutomaton::displayAutomaton() {
	std::cout << "Your Automaton is:" << std::endl;

	std::cout << "States: ";
	for (const auto& state : m_states)
	{
		std::cout << state << " ";
	}
	std::cout << std::endl;

	std::cout << "Alphabet: ";
	std::cout << "States: ";
	for (const auto& alphabet : m_alphabet)
	{
		std::cout << alphabet << " ";
	}
	std::cout << std::endl;

	std::cout << "Stack Alphabet: ";
	for (const auto& stackAlphabet : m_stackAlphabet)
	{
		std::cout << stackAlphabet << " ";
	}
	std::cout << std::endl;

	std::cout << "Transitions: " << std::endl;
	for (const auto& transition : m_transitions)
	{
		std::cout << "(" << m_initialState << ", " << std::get<0>(transition) << ", " << std::get<1>(transition) << " )= (" << m_initialState << ", " << std::get<2>(transition) << ")" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Starting symbol: " << m_initialState << std::endl;

	std::cout << "Final States: ";
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		std::cout << m_finalStates[i] << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

bool PushDownAutomaton::isDeterministic() const {
	for (int i = 0; i < m_transitions.size() - 1; i++)
	{
		for (int j = i + 1; j < m_transitions.size(); j++)
		{
			if (
				std::get<0>(m_transitions[i]) == std::get<0>(m_transitions[j]) &&
				std::get<1>(m_transitions[i]) == std::get<1>(m_transitions[j]) &&
				std::get<2>(m_transitions[i]) != std::get<2>(m_transitions[j])
				)
			{
				return false;
				break;
			}
		}
	}
	return true;
}