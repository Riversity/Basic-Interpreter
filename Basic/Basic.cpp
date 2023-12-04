/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string token;
    token = scanner.nextToken();

    bool isNumber = true;
    int line_number;
    //Judge whether number
    try {
        line_number = stringToInteger(token);
    }
    catch (...) {
        isNumber = false;
    }
    if (isNumber) {
        program.addSourceLine(line_number, line);
        return;
    }
    else {
        if (token == "LET") {
            Let command(line);
            command.execute(state, program);
        }
        else if (token == "PRINT") {
            Print command(line);
            command.execute(state, program);
        }
        else if (token == "INPUT") {
            Input command(line);
            command.execute(state, program);
        }
        else if (token == "RUN") {
            program.programRun(state);
        }
        else if (token == "LIST") {
            program.programList();
        }
        else if (token == "CLEAR") {
            program.clear();
            state.Clear();
        }
        else if (token == "QUIT") {
            program.clear();
            state.Clear();
            exit(0);
        }
        else if (token == "HELP") {
            std::cout << "Yet another basic interpreter" << std::endl;
        }
        else { // Default
            error("SYNTAX ERROR");
        }
    }
    return;
}

