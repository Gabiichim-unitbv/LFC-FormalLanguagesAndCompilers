#include "DeterministicFiniteAutomaton.h"
#include "FiniteAutomaton.h"
#include "ExprReg.h"
#include <fstream>
#include <conio.h>

void initMenu(const ExprReg& regex, DeterministicFiniteAutomaton automaton)
{
    std::cout << "***REGEX & AUTOMATON***\n";
    char x;
    do {
        std::cout << "\nChoose what do you want to happen or press X to quit:\n" << "A - show the automaton\n"
            << "B - show the regex\n" << "C - verify if your builded automaton is accepting words\n";

        x = _getch();
        switch (x)
        {
        case 'a':
            std::cout << "\nYour automaton:\n" << automaton << std::endl;
            break;
        case 'b':
            std::cout << "\nYour regex is : " << regex << std::endl;
            break;
        case 'c':
            std::string word;
            std::cout << "\nEnter a word to verify: ";
            std::cin >> word;
            try {
                if (automaton.checkWord(word)) {
                    std::cout << "The automaton accepts the input: " << word << std::endl;
                }
                else {
                    std::cout << "The automaton does not accept the input: " << word << std::endl;
                }
            }
            catch (std::exception e)
            {
                std::cout << e.what();
            }
        }
    } while (x != 'x');
}

int main()
{
    ExprReg regex;
    FiniteAutomaton automaton;
    DeterministicFiniteAutomaton deterministicAutomaton;
    std::ifstream in("expresieRegex.txt");
    in >> regex;
    if (regex.isValid())
    {
        automaton = regex.getAutomaton();   
        deterministicAutomaton.convertFromNedeterministic(automaton);
        initMenu(regex, deterministicAutomaton);
    }
    else
        std::cout << "Regex not valid.";

    return 0;
}
