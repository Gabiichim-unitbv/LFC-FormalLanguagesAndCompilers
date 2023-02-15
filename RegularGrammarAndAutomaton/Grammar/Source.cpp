#include <fstream>
#include <vector>
#include "Grammar.h"
#include <conio.h>

void initMenu(Grammar gr)
{
	std::cout << "***GRAMMAR & AUTOMATON***\n";
	char x;
	do {
		std::cout << "Choose what do you want to happen to your Grammar or press X to quit:\n" << "1 - show the grammar\n"
			<< "2 - generate words in grammar\n" << "3 - build and show the equivalent automaton for your grammar\n"
			<< "4 - verify if your builded automaton is accepting words\n" << "5 - generate a word in in grammar and verify it with automaton\n"
			<< "6 - display the already generated words";
		x = _getch();
		switch (x)
		{
		case '1':
		{
			
			std::cout << "\nYour grammar is:\n\n" << gr << std::endl;
		
			break;
		}

		case '2':
		{
			std::cout << "\nEnter the number of words do you want to generate: ";
			int wordsNum;
			std::cin >> wordsNum;
			for (int i = 0; i < wordsNum; i++)
				gr.generateWord();
			std::cout << std::endl;
			break;
		}

		case '3':
		{
			
			std::cout << gr.getAutomaton() << std::endl;
			break;
		}

		case '4':
		{
			FiniteAutomaton automaton = gr.getAutomaton();
			char quit;
			do
			{
				
				std::cout << "\nYou are into the word checker app!\nPress any key to enter a word or press Q to escape: \n";
				quit = _getch();
				if (quit == 'q')
				{
					std::cout << std::endl;  
					break;
				}
				std::cout << "Please enter a word formed from (";
				automaton.showAlphabet();
				std::cout << "): ";

				std::string word;
				std::cin >> word;
				if (automaton.checkWord(word))
					std::cout << "The word is accepted!\n";
				else std::cout << "The word isn't accepted!\n";

			} while (quit != 'q');

			break;
		}

		case '5':
		{
			
			FiniteAutomaton automaton = gr.getAutomaton();
			char quit;
			do
			{

				std::cout << "\nYou are into the word generator app!\nPress any key to generate and verify a word or press Q to escape: \n";
				quit = _getch();
				if (quit == 'q')
				{
					std::cout << std::endl;
					break;
				}

				std::string word = gr.generateWord();

				if (automaton.checkWord(word))
					std::cout << "The word is accepted!\n";
				else std::cout << "The word isn't accepted!\n";

			} while (quit != 'q');

			break;
		}

		case '6':
		{

			gr.displayGenerated();
			break;
		}

		default:
			break;
		}

	} while (x != 'x');

	std::cout << "\n\nExiting...\n";
}

int main()
{
	
	std::ifstream in("input.txt");
	Grammar gr;
	in >> gr;
	if (gr.verify())
		if (gr.isRegular())
			initMenu(gr);
		else std::cout << "Your grammar isn't valid/regular!";
}