//
//  interpreter.h
//  program 4
//
//  Created by Ellieanna Ross on 11/14/22.
//

#ifndef interpreter_h
#define interpreter_h

#include <vector>
#include <string>
#include "lexanalyzer.h"
#include "expevaluator.h"
#include <queue>

typedef std::vector<std::string> programType;


class Interpreter {
public:
    Interpreter();
    void interpret(programType programCode);
    void interpretLoop(programType programCode);
    void showVariables();
    void clear();
    // my lexical analyzer - analyzes program code
    lexicalAnalyzer lex;
    // my expression evaluator - will evauluate what I tell it to
    expEvaluator expEval;
private:
    bool widerLoopValue = false;
    std::string whileCondition;
    bool ifFunction(std::string);   // deals with if statements
    bool whileFunction();    // deals with while loops
};


#endif /* interpreter_h */
