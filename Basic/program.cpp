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
    // Replace this stub with your own code
    //todo
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    //todo
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   //todo
}

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
        return *it;
    }
}

void Program::programList() {
    for (auto it : line_number_set) {
        std::cout << str_index.at(it) << '\n';
    }
}
//more func to add
//todo


