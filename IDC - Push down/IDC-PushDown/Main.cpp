#include "Grammar.h"
#include "PushDownAutomaton.h"

int main() {
	Grammar gr;
	gr.readGrammar();

	if (gr.isContextFree()) {
		int option;
		std::cout << "Your Grammar is valid. The commands are: " << std::endl;
		std::cout << "0 - EXIT" << std::endl;
		std::cout << "1 - PRINT GRAMMAR" << std::endl;
		std::cout << "2 - WORD GENERATOR" << std::endl;
		std::cout << "3 - PRINT SIMPLIFIED GRAMMAR" << std::endl;
		std::cout << "4 - PRINT FNG GRAMMAR" << std::endl;
		std::cout << "5 - PRINT PUSHDOWN AUTOMATON FORM IDC GRAMMAR" << std::endl;
		std::cout << "6 - GENERATE A WORD AND TEST IF IT'S RECOGNIZED BY AUTOMATON" << std::endl;
		std::cout << "7 - TYPE A WORD AND CHECK IF TEST IF IT'S RECOGNIZED BY AUTOMATON" << std::endl;

		do
		{
			std::cout << "Please type comand: ";
			std::cin >> option;
			switch (option)
			{
			case 0:
			{
				std::cout << "EXIT!" << std::endl;
				break;
			}
			case 1:
			{
				gr.printGrammar();
				break;
			}
			case 2:
			{
				int nrOfWords;
				std::cout << "Introdu nr de cuvinte: " << "\n";
				std::cin >> nrOfWords;
				for (int i = 0; i < nrOfWords; i++) {
					gr.generateWord();
				}
				break;
			}
			case 3:
			{
				gr.simplifyGrammar();
				break;
			}
			case 4:
			{
				break;
			}
			case 5:
			{
				std::vector<Production> transitions = gr.getProductions();
				std::unordered_set<string> alphabet = gr.getTerminals();
				std::unordered_set<string> stackAlphabet = gr.getNonTerminals();
				std::string startSymbol = gr.getStartSymbol();

				PushDownAutomaton automaton(transitions, alphabet, stackAlphabet, startSymbol);
				
				automaton.displayAutomaton();

				if (automaton.isDeterministic()) {
					std::cout << "Is deterministic" << std::endl;
				}
				else {
					std::cout << "Is not deterministic" << std::endl;
				}
				break;
			}
			case 6:
			{
				break;
			}
			case 7:
			{
				break;
			}
			default:
				break;
			}

		} while (option != 0);
	}
	else {
		std::cout << "Grammar is not context free";
	}
	return 0;
}