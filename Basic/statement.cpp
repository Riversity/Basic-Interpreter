/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

Statement::Statement() = default;

Statement::~Statement() = default;

// LET

Let::Let(std::string& line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    scanner.nextToken(); // Eat LET
    var = scanner.nextToken();
    if(scanner.nextToken() != "=") { // Eat "="
        error("SYNTAX ERROR");
    }
    exp = readE(scanner);
}

void Let::execute(EvalState &state, Program &program) {
    state.setValue(var, exp->eval(state));
}

Let::~Let() { // Leak Prevention
    delete exp;
}

// PRINT

Print::Print(std::string& line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    std::string token;
    scanner.nextToken(); // Eat PRINT
    exp = readE(scanner); // Minor fallacy, try catch later
    scanner.setInput(token);
}

void Print::execute(EvalState &state, Program &program) {
    std::cout << exp->eval(state) << std::endl;
}

Print::~Print() {
    delete exp;
}

// INPUT

Input::Input(std::string& line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();

    scanner.nextToken();
    var = scanner.nextToken();

    if(scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    }
}

void Input::execute(EvalState &state, Program &program) {
    while(true) {
        std::string input;
        int input_value;
        std::cout << " ? " << std::endl;
        getline(std::cin, input);
        try {
            input_value = stringToInteger(input);
        }
        catch (...) {
            std::cout << "SYNTAX ERROR" << std::endl;
        }
    }
}

Input::~Input() {
    delete exp;
}

// GOTO

Goto::Goto(std::string &line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();

    scanner.nextToken(); // Skip word GOTO
    try {
        new_line = stringToInteger(scanner.nextToken());
    }
    catch (...) {
        error("SYNTAX ERROR");
    }
    if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    }
}

void Goto::execute(EvalState &state, Program &program)
{
    if(!program.line_number_set.count(new_line))
    {
        error("LINE NUMBER ERROR");
    }
    program.cur = new_line;
}