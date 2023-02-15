#pragma once

#include <vector>
#include <unordered_set>
#include "Production.h"

class Grammar {
private:
    std::unordered_set<std::string> m_nonTerminals;
    std::unordered_set<std::string> m_terminals;
    std::string m_startSymbol;
    std::vector<Production> m_productions;

public:
    Grammar() = default;

    ~Grammar() = default;

    void readGrammar();

    void printGrammar() const;

    bool verifyGrammar() const;

    bool doesIntersect() const;

    bool startIsNonTerminal() const;

    bool firstContainsNonTerminal() const;

    bool containsOnlyNonTerminalsAndTerminals() const;

    bool belongsToNonTerminals(const std::string &chr) const;

    bool belongsToTerminals(const std::string &chr) const;

    bool startExists() const;

    bool isRegular() const;

    std::vector<int> generateApplicablePositions(const std::string& word);

    void generateWord();

    static void printSet(const std::unordered_set<std::string>& el);

    //suplimentary methods

    bool isContextFree() const;

    void simplifyGrammar();

    void grammarToGreibach();

    //getters

    std::vector<Production> getProductions();

    std::unordered_set<std::string> getTerminals();

    std::unordered_set<std::string> getNonTerminals();

    std::string getStartSymbol();
};

