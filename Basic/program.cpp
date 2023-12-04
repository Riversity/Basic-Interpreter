/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"


Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    str_index.clear();
    for (auto i : line_number_set) {
        delete stmt_index.at(i);
    }
    line_number_set.clear();
    stmt_index.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) 
{
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    std::string token;
    scanner.nextToken(); // Skip number
    if(scanner.hasMoreTokens())
    {
        int pos = 0; // Remove number at front
        while (pos < line.length() && isdigit(line[pos])) ++pos;
        while (pos < line.length() && line[pos] == ' ') ++pos;
        std::string processed_line = line.substr(pos);
        token = scanner.nextToken();

        if(line_number_set.count(lineNumber)) // Replacement
        {
            delete stmt_index.at(lineNumber);
        }
        str_index.insert_or_assign(lineNumber, line);

        if(token == "LET")
        {
            setParsedStatement(lineNumber, new Let(processed_line));
        }
        else if(token == "REM")
        {
            setParsedStatement(lineNumber, new Rem);
        }
        else if(token == "PRINT")
        {
            setParsedStatement(lineNumber, new Print(processed_line));
        }
        else if(token == "INPUT")
        {  
            setParsedStatement(lineNumber, new Input(processed_line));       
        }
        else if(token == "IF")
        {
            setParsedStatement(lineNumber, new If(processed_line));
        }    
        else if(token == "GOTO")
        {
            setParsedStatement(lineNumber, new Goto(processed_line));
        }
        else if(token == "END")
        {
            setParsedStatement(lineNumber, new End(processed_line));
        }
        else { //Default
            removeSourceLine(lineNumber);
            error("SYNTAX ERROR");
        }
    }
    else // Only number, delete line!
    {
        removeSourceLine(lineNumber);
    }
}

void Program::removeSourceLine(int lineNumber) 
{
    if(line_number_set.count(lineNumber))
    {
        line_number_set.erase(lineNumber);
        str_index.erase(lineNumber);
        delete stmt_index.at(lineNumber);
        stmt_index.erase(lineNumber);
    }
}

/*std::string Program::getSourceLine(int lineNumber) {

} */

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    line_number_set.insert(lineNumber);
    stmt_index[lineNumber] = stmt;
}

/* Statement *Program::getParsedStatement(int lineNumber) {

} */

int Program::getFirstLineNumber() {
    if (line_number_set.empty()) return -1;
    else return *line_number_set.begin();
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = line_number_set.find(lineNumber);
    if (it == line_number_set.end()) {
        return -1;
    }
    else {
        ++it;
        if (it == line_number_set.end()) {
            return -1;
        }
        return *it;
    }
}

void Program::programList() {
    for (auto it : line_number_set) {
        std::cout << str_index.at(it) << std::endl;
    }
}

void Program::programRun(EvalState &state)
{
    cur = getFirstLineNumber();
    while(cur != -1)
    {
        int temp = cur;
        stmt_index.at(cur)->execute(state, *this);
        // Judge whether GOTO
        if(temp == cur)
        {
            cur = getNextLineNumber(cur);
        }
    }
}
