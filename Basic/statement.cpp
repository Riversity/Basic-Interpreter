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

Let::Let(std::string &line) {
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

Print::Print(std::string &line) {
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

Input::Input(std::string &line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();

    scanner.nextToken();
    var = scanner.nextToken();
    if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    }
}

void Input::execute(EvalState &state, Program &program) {
    int input_value;
    while(true) {
        bool flag = true; // If legal input

        std::string input;
        std::cout << " ? ";
        getline(std::cin, input);

        try {
            input_value = stringToInteger(input);
        }
        catch (...) {
            std::cout << "INVALID NUMBER" << std::endl;
            flag = false;
        }
        if(flag) break;
    }
    state.setValue(var, input_value);
}

// END

End::End(std::string &line) {
    if (trim(line) != "END") error("SYNTAX ERROR");
}

void End::execute(EvalState &state, Program &program) {
    program.cur = -1;
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

Goto::Goto(int &line_number) { // To be used in IF
    new_line = line_number;
}

void Goto::execute(EvalState &state, Program &program)
{
    if(!program.line_number_set.count(new_line))
    {
        error("LINE NUMBER ERROR");
    }
    program.cur = new_line;
}

// IF

If::If(std::string &line) {
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    
    std::string cmp_symbol;
    scanner.nextToken();
    lep = readE(scanner);
    cmp_symbol = scanner.nextToken();
    if (cmp_symbol == ">") op = std::strong_ordering::greater;
    else if (cmp_symbol == "=") op = std::strong_ordering::equal;
    else if (cmp_symbol == "<") op = std::strong_ordering::less;
    else error("SYNTAX ERROR");
    rep = readE(scanner);
    scanner.nextToken();
    new_line = stringToInteger(scanner.nextToken());
}

void If::execute(EvalState &state, Program &program) {
    //int expected = lep->eval(state) > rep->eval(state) - lep->eval(state) < rep->eval(state);
    auto expected = lep->eval(state) <=> rep->eval(state);
    if (op == expected) {
        Goto temp(new_line);
        temp.execute(state, program);
    }
}

If::~If() {
    delete lep;
    delete rep;
}
