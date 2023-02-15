#pragma once
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include "FiniteAutomaton.h"
class ExprReg
{
private:
	std::string m_exprReg;
	std::vector<char> m_polishNotation;
	int priority(const char& ch);

public:
	ExprReg() = default;
	void setPolishNotation();
	FiniteAutomaton getAutomaton();
	bool isValid();
	
	friend std::istream& operator>>(std::istream& in, ExprReg& regex);
	friend std::ostream& operator<<(std::ostream& out, const ExprReg& regex);
};

