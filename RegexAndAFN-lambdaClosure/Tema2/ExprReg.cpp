#include "ExprReg.h"
#include <iostream>


bool ExprReg::isValid()
{
	for (const auto& ch : m_exprReg)
		if (!isalpha(ch) && !isdigit(ch) && ch != '(' && ch != ')' && ch != '.' && ch != '|' && ch != '*')
			return false;

	std::stack<char> stack;
	for (const auto& ch : m_exprReg)
	{
		switch (ch)
		{
		case '(':
			stack.push(ch);
			break;
		case ')':
			if (stack.empty() )
			{
				return false;
			}
			stack.pop();
			break;
		}
	}

	if (!stack.empty())
	{
		return stack.empty();
	}
	

	for (int i = 1; i < m_exprReg.length(); i++)
	{
		if (m_exprReg[i - 1] == m_exprReg[i] || (isalpha(m_exprReg[i - 1]) && isalpha(m_exprReg[i])) || 
			(isdigit(m_exprReg[i - 1]) && isdigit(m_exprReg[i])))
			return false;
	}

}

void ExprReg::setPolishNotation()
{
	std::stack<char> op;

	for (const auto& ch : m_exprReg)
	{

		if (ch == '(')
		{
			op.push('(');
		}
		else
		{
			if (ch == ')')
			{
				while (!op.empty() && op.top() != '(')
				{
					m_polishNotation.push_back(op.top());
					op.pop();
				}
				op.pop();
			}
			else
			{
				if (priority(ch) != -1)
				{
					while (!op.empty() && priority(ch) <= priority(op.top()))
					{
						m_polishNotation.push_back(op.top());
						op.pop();
					}

					op.push(ch);
				}
				else m_polishNotation.push_back(ch);
			}
		}
	}

	while (!op.empty())
	{
		m_polishNotation.push_back(op.top());
		op.pop();
	}

	//for (const auto& ch : m_polishNotation)
	//	std::cout << ch << ' ';
}

FiniteAutomaton ExprReg::getAutomaton()
{
	setPolishNotation();
	int contor = 0;
	std::stack<FiniteAutomaton> automatonStack;

	for (const auto& ch : m_polishNotation)
	{
		if (isalpha(ch))
		{
			FiniteAutomaton a;
			a.setTransition(std::to_string(contor), ch, std::to_string(contor + 1));
			a.setStates();
			a.setFinalStates({ std::to_string(contor + 1) });
			a.setStartState(std::to_string(contor));
			a.setSymbols();
			contor += 2;
			automatonStack.push(a);
		}

		if (ch == '|')
		{
			FiniteAutomaton a, b, c;
			a = automatonStack.top();
			automatonStack.pop();
			b = automatonStack.top();
			automatonStack.pop();
			c.setTransition(std::to_string(contor), lambda,  a.getStartState());
			c.setTransition(std::to_string(contor), lambda, b.getStartState());
			c.setTransition(*a.getFinalStates().begin(), lambda, std::to_string(contor + 1));
			c.setTransition(*b.getFinalStates().begin(), lambda, std::to_string(contor + 1));

			for (const auto& trans : a.getTransitions())
				c.setTransition(trans);

			for (const auto& trans : b.getTransitions())
				c.setTransition(trans);

			c.setStates();
			c.setFinalStates({ std::to_string(contor + 1) });
			c.setStartState(std::to_string(contor));
			c.setSymbols();
			contor += 2;
			automatonStack.push(c);
		}

		if (ch == '.')
		{
			FiniteAutomaton a, b, c;
			b = automatonStack.top();
			automatonStack.pop();
			a = automatonStack.top();
			automatonStack.pop();

			for (const auto& trans : a.getTransitions())
				c.setTransition(trans);

			for (const auto& trans : b.getTransitions())
				c.setTransition(trans);

			FiniteAutomaton copyC = c;

			for (auto& trans : copyC.getTransitions()) // contopim fA = iB
			{
				auto& [initialState, symbol, nextState] = trans;
				if (initialState == b.getStartState())
				{
					c.removeTransition(trans);
					std::tuple<std::string, char, std::string> transition = std::make_tuple(*a.getFinalStates().begin(), symbol, nextState);
					c.setTransition(transition);
				}

				if (nextState == b.getStartState())
				{
					c.removeTransition(trans);
					std::tuple<std::string, char, std::string> transition = std::make_tuple(*a.getFinalStates().begin(), symbol, nextState);
					c.setTransition(transition);
				}
			}

			c.setStates();
			c.setFinalStates({ *b.getFinalStates().begin() });
			c.setStartState(a.getStartState());
			c.setSymbols();
			automatonStack.push(c);
		}

		if (ch == '*')
		{
			FiniteAutomaton a, c;
			a = automatonStack.top();
			automatonStack.pop();

			for (const auto& trans : a.getTransitions())
				c.setTransition(trans);

			c.setTransition(*a.getFinalStates().begin(), lambda, a.getStartState());

			c.setTransition(std::to_string(contor), lambda, a.getStartState());
			c.setTransition(std::to_string(contor), lambda, std::to_string(contor + 1));
			c.setTransition(*a.getFinalStates().begin(), lambda, std::to_string(contor + 1) );
			c.setStates();
			c.setFinalStates( { std::to_string(contor + 1) } );
			c.setStartState(std::to_string(contor));
			c.setSymbols();
			automatonStack.push(c);
			contor += 2;
		}

	}

	return automatonStack.top();
}

int ExprReg::priority(const char& ch)
{
	switch (ch)
	{
	case '|':
	{
		return 1;
		break;
	}

	case '.':
	{
		return 2;
		break;
	}

	case '*':
	{
		return 3;
		break;
	}
	default:
	{
		return -1;
		break;
	}
	}
}

std::istream& operator>>(std::istream& in, ExprReg& regex)
{
	in >> regex.m_exprReg;
	return in;
}

std::ostream& operator<<(std::ostream& out, const ExprReg& regex)
{
	ExprReg copyRegex = regex;
	copyRegex.m_exprReg.erase(remove(copyRegex.m_exprReg.begin(), copyRegex.m_exprReg.end(), '.'), copyRegex.m_exprReg.end());
	out << copyRegex.m_exprReg;
	return out;
}
