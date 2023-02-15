#include "Grammar.h"
#include <fstream>
#include <random>
#include <algorithm>

void Grammar::readGrammar() {
	std::ifstream f("inputGrammar.txt");
	int nr;

	//neterminale
	std::string readString;
	f >> nr;
	for (int i = 0; i < nr; i++) {
		f >> readString;
		m_nonTerminals.insert(readString);
	}

	//terminale
	f >> nr;
	for (int i = 0; i < nr; i++) {
		f >> readString;
		m_terminals.insert(readString);
	}

	//simbol start
	f >> m_startSymbol >> nr;

	//productii
	Production readProd;
	for (int i = 0; i < nr; i++) {
		f >> readProd;
		m_productions.push_back(readProd);
	}
	f.close();

	//verif regular
	if (isRegular()) {
		std::cout << "Gramatica este regulata\n";
	}
	else {
		std::cout << "Gramatica nu este regulata\n";
	}
}

void Grammar::printGrammar() const {
	std::cout << "Multimea neterminalelor: ";
	printSet(m_nonTerminals);
	std::cout << "\n";
	std::cout << "Multimea terminalelor: ";
	printSet(m_terminals);
	std::cout << "\n";
	std::cout << "Simbol start: " << m_startSymbol << "\n";
	std::cout << "Productii: " << "\n";
	for (const auto& production : m_productions) {
		std::cout << production << "\n";
	}
}

void Grammar::printSet(const std::unordered_set<std::string>& el) {
	for (const auto& element : el) {
		std::cout << element << ' ';
	}
}

bool Grammar::verifyGrammar() const {
	if (doesIntersect()) {
		std::cout << "Multimile se intersecteaza.\n";
		return false;
	}
	if (!startIsNonTerminal()) {
		std::cout << "Simbolul de start nu apartine multimii neterminalelor.";
		return false;
	}
	if (!firstContainsNonTerminal()) {
		std::cout << "Exista un membru stang care nu contine niciun neterminal.";
		return false;
	}
	if (!startExists()) {
		std::cout << "Nu exista nicio productie care contine simbolul de start.";
		return false;
	}
	if (!containsOnlyNonTerminalsAndTerminals()) {
		std::cout << "Productiile nu contin numai terminale si neterminale.";
		return false;
	}
	return true;
}

bool Grammar::doesIntersect() const {
	for (const auto& nonTerminals : m_nonTerminals) {
		for (const auto& terminals : m_terminals) {
			if (nonTerminals == terminals) {
				return true;
			}
		}
	}
	return false;
}

bool Grammar::startIsNonTerminal() const {
	return m_nonTerminals.find(m_startSymbol) != m_nonTerminals.end();
}

bool Grammar::firstContainsNonTerminal() const {
	for (const auto& production : m_productions) {
		for (const auto& nonTerminals : m_nonTerminals) {
			if (production.first().find(nonTerminals)) {
				return true;
			}
		}
	}
	return false;
}

bool Grammar::startExists() const {
	for (const auto& production : m_productions) {
		if (production.first() == m_startSymbol) {
			return true;
		}
	}
	return false;
}

bool Grammar::belongsToNonTerminals(const std::string& chr) const {
	return m_nonTerminals.find(chr) != m_nonTerminals.end();
}

bool Grammar::belongsToTerminals(const std::string& chr) const {
	return m_terminals.find(chr) != m_terminals.end();
}

bool Grammar::containsOnlyNonTerminalsAndTerminals() const {
	for (const auto& production : m_productions) {
		for (int i = 0; i < production.first().size(); i++) {
			std::string chr(1, production.first()[i]);
			if (!belongsToNonTerminals(chr) && !belongsToTerminals(chr)) {
				return false;
			}
		}
		for (int i = 0; i < production.second().size(); i++) {
			std::string chr(1, production.second()[i]);
			if (chr != "L") {
				if (!belongsToNonTerminals(chr) && !belongsToTerminals(chr)) {
					return false;
				}
			}
		}
	}
	return true;
}

bool Grammar::isRegular() const {
	for (const auto& production : m_productions) {
		//verificare lungime membrii
		if (production.first().size() > 1 || production.second().size() > 2) {
			return false;
		}
		//verif daca membru stang este neterminal
		std::string chr(1, production.first()[0]);
		if (!belongsToNonTerminals(chr)) {
			return false;
		}
		//verif membru drept litera 1 este terminala A->aB or A->a
		chr[0] = production.second()[0];
		if (!belongsToTerminals(chr)) {
			return false;
		}
		//verif membru drept litera 2 este neterminala
		chr[0] = production.second()[1];
		if (production.second().size() > 1 && !belongsToNonTerminals(chr)) {
			return false;
		}
	}
	return true;
}

bool Grammar::isContextFree() const
{
	for (const auto& production : m_productions) {
		// ex: AA->abba
		if (production.first().size() > 1)
		{
			return false;
		}
	}
	return true;
}

void Grammar::simplifyGrammar()
{
	std::vector<Production> auxProduction;
	std::unordered_set<std::string> auxNonTerminals;

	//Eliminare stari care nu produc cuvinte
	for (const auto& nonTerminal : m_nonTerminals) {
		bool ok = false;
		for (const auto& production : m_productions) {
			if (production.first() == nonTerminal &&
				production.second().find(nonTerminal) == std::string::npos) {
				ok = true;
				break;
			}
		}
		if (!ok) {
			auxProduction.clear();
			for (const auto& production : m_productions) {
				if (production.second().find(nonTerminal) == std::string::npos) {
					auxProduction.push_back(production);
				}
			}
			m_productions = auxProduction;
		}
		else {
			auxNonTerminals.insert(nonTerminal);
		}
	}

	m_nonTerminals = auxNonTerminals;

	//Eliminare simboluri inaccesibile
	auxNonTerminals.clear();

	for (const auto& nonTerminal : m_nonTerminals) {
		bool ok = false;
		for (const auto& production : m_productions) {
			if (production.second().find(nonTerminal) != std::string::npos &&
				production.first() != nonTerminal) {
				ok = true;
			}
		}
		if (!ok) {
			auxProduction.clear();
			for (const auto& production : m_productions) {
				if (production.first() != nonTerminal) {
					auxProduction.push_back(production);
				}
			}
			m_productions = auxProduction;
		}
		else {
			auxNonTerminals.insert(nonTerminal);
		}
	}

	//Remove unused terminals
	std::unordered_set<std::string> auxTerminals;
	for (const auto& terminal : m_terminals) {
		for (const auto& production : m_productions) {
			if (production.second().find(terminal) != std::string::npos) {
				auxTerminals.insert(terminal);
			}
		}
	}

	m_terminals = auxTerminals;
	m_nonTerminals = auxNonTerminals;
	printGrammar();
}

void Grammar::grammarToGreibach()
{
}

std::vector<Production> Grammar::getProductions()
{
	return m_productions;
}

std::unordered_set<std::string> Grammar::getTerminals()
{
	return m_terminals;
}

std::unordered_set<std::string> Grammar::getNonTerminals()
{
	return m_nonTerminals;
}

std::string Grammar::getStartSymbol()
{
	return m_startSymbol;
}

int generateRandomNumber(int size) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist(0, size - 1);
	return dist(rng);
}

std::vector<int> Grammar::generateApplicablePositions(const std::string& word) {
	std::vector<int> positions;
	for (int i = 0; i < m_productions.size(); i++)
		if (word.find(m_productions[i].first()) != std::string::npos)
			positions.push_back(i);
	return positions;
}

void Grammar::generateWord() {
	std::string word = m_startSymbol;
	std::vector<int> positions = generateApplicablePositions(word);
	std::cout << "Cuvant initial: " << word;
	while (!positions.empty()) {
		int randPosition = generateRandomNumber(positions.size());
		std::string replaced = m_productions[positions[randPosition]].first();
		std::string replacer = m_productions[positions[randPosition]].second();
		if (replacer != "L") {
			word.replace(word.find(replaced), replaced.size(), replacer);
		}
		else {
			word.erase(word.find(replaced), replaced.size());
		}
		if (word.empty()) {
			std::cout << " -> R(" << positions[randPosition] << ") -> " << "L";
		}
		else {
			std::cout << " -> R(" << positions[randPosition] << ") -> " << word;
		}
		positions.clear();
		positions = generateApplicablePositions(word);
	}
	std::cout << "\n\n";
}


