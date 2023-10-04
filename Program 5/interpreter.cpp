//
//  interpreter.cpp
//  program 2
//
//  Created by Ellieanna Ross on 11/14/22.
//  read(/Users/ellieannaross/Desktop/test.py)
//  read(\Users\ellie\source\repos\program 4\program 4 - windows\program 4 - windows\testFile.java)
 
#include "interpreter.h"
#include <iostream>
#include "lexanalyzer.h"
#include "expevaluator.h"
using namespace std;
 
Interpreter::Interpreter() {
 
}
 

/*
    I will now be adding code that evaluates if and while statements
        - I will use a stack for sub-code pertaining to a loop
                ✅ this will use a ifStack and a whileStack in my header file
        - Member functions should be added to clear the stack(s) or check whether the stack(s) are empty
        - I need to ensure that the spacing is being checked properly
 */

 
void Interpreter::interpret(programType programCode) {
    // my lexical analyzer - analyzes program code
    lex.clear();
    lex.analyzeCode(programCode);
 

    // keeps track of if statements
    bool ifStmt = false;
    string expressionSaved;
    bool elifStmt = false;
    bool elseStmt = false;
    bool didPrevious = false;
    
    
    // while loop variables
    bool whileLoop = false;
    int linesToReverse = 0;
    

    // loops through each line in tokenInfo
    for (int i = 0; i < lex.tokenInfo.size(); i++) {
 
        
        string exp; // this variable keeps track of an expression
        string identifier;  // this variable keeps track of variable names
        bool expression = false;    // check if the line is going to be an expression assignment
        string temp;    // keep track of output
        bool print = false; // keeps track of if something should be printed out
        bool input = false; // keeps track of whether or not I need an input
 
        
 
        // loops through each token in line i
        for (int j = 0; j < lex.tokenInfo[i].size(); j++) {
            
            //cout << ": is statement: " << lex.tokenInfo[i][j].first << ", and exp: " << ifFunction(expressionSaved) << endl;
            //cout << lex.tokenInfo[i][j].first << ", while loop? " << whileLoop << endl;
            
            // this code checks for things that are parts of if statements or elif or else
            if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::INDENT && j == 0) {
                // if it's an else of elseif statement...
                if ((ifStmt || elifStmt) && !ifFunction(expressionSaved)) {
                    // the if statement has been proven to be false and now must move on to some other thing
                    break;
                } else if (didPrevious) {
                    break;
                    
                } else if ((whileLoop && widerLoopValue && i == lex.tokenInfo.size()-1 && j == lex.tokenInfo[i].size()-1)) {
                    //cout << "here i can either reverse " << linesToReverse << " times, or continue on" << endl;
                    if (whileFunction()) {
                        //cout << "i shall reverse! " << linesToReverse << " line" << endl;
                        
                        i -= linesToReverse;
                        linesToReverse = 1;
                        
                    }
                }
                else if (whileLoop && widerLoopValue) {
                    linesToReverse++;
                }
                else if (whileLoop) {
                    break;
                }
            } else if ((whileLoop && widerLoopValue && j == 0) || (whileLoop && widerLoopValue && i == lex.tokenInfo.size()-1 && j ==               lex.tokenInfo[i].size()-1)) {
                //cout << "here i can either reverse " << linesToReverse << " times, or continue on" << endl;
                if (whileFunction()) {
                    //cout << "i shall reverse! " << linesToReverse << " line" << endl;
                    
                    i -= linesToReverse;
                    linesToReverse = 1;
                    
                }
            }
//            if (whileFunction()) {
//                i = i - linesToReverse;
//                j = 0;
//            } else {
//                whileLoop = false;
//                widerLoopValue = false;
//            }
            
            
            
            
            
            else if (j == 0){
                ifStmt = false;
                elseStmt = false;
                elifStmt = false;
            }

//            if (whileFunction()) {
//                i = i - linesToReverse;
//                j = 0;
//            } else {
//                whileLoop = false;
//                widerLoopValue = false;
//            }
            
            
            
            // check if the token is a comment
            if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::COMMENT) {
                // if the token is a comment, don't print it out
                break;
            }
            // check if the token is an identifier
            else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::IDENTIFIER) {
                // check if value should be printed
                if (print) {
                    exp += lex.tokenInfo[i][j].first;
                }
                // else flag the line as an expression
                else if (expression) {
                    exp += lex.tokenInfo[i][j].first;
                }
                else {
                    identifier = lex.tokenInfo[i][j].first;
                    expression = true;
                }
 
            }
            
            
 
            // check if the token is an assginment
            else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::ASSIGNMENT_OP) {
                // set the expression flag to true
                expression = true;

            }
            
            
 
            // check if token makes up an expression
            else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::NUMERIC_LITERAL || lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::ARITH_OP) {
                
                // check if it's already been flagged as an expression
                if (expression) {
                    exp += lex.tokenInfo[i][j].first;
                }
                else if (print) {
                    exp += lex.tokenInfo[i][j].first;
                }
 
            }
 
            // check if token is a keyword
            else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::KEYWORD) {
                // check if the keyword is print
                if (lex.tokenInfo[i][j].first == "print") {
                    print = true;
                }
 
                // check if keyword is input
                else if (lex.tokenInfo[i][j].first == "input") {
                    input = true;
                }
                
                // check for ifStmt
                else if (lex.tokenInfo[i][j].first == "if") {
                    ifStmt = true;
                    expression = true;
                    didPrevious = false;
                }
                
                // check for elif
                else if (lex.tokenInfo[i][j].first == "elif") {
                    elifStmt = true;
                    ifStmt = false;
                    expression = true;
                    if (ifFunction(expressionSaved)) {
                        didPrevious = true;
                    }
                }
                
                // check for else
                else if (lex.tokenInfo[i][j].first == "else") {
                    elseStmt = true;
                    expression = true;
                    elifStmt = false;
                    ifStmt = false;
                    if (ifFunction(expressionSaved)) {
                        didPrevious = true;
                    }
                }
                
                // check for while loop
                else if (lex.tokenInfo[i][j].first == "while") {
                    whileLoop = true;
                    expression = true;
                    ifStmt = false;
                    elifStmt = false;
                    elseStmt = false;
                    didPrevious = false;
                }
 
            }
 
            // check if it's a string
            else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::STRING_LITERAL) {
                if (print) {
                    cout << (lex.tokenInfo[i][j].first).substr(1, lex.tokenInfo[i][j].first.size() - 2) << " ";
                }
                else if (input && expression) {
                    cout << (lex.tokenInfo[i][j].first).substr(1, lex.tokenInfo[i][j].first.size() - 2);
                    string in;
                    cin >> in;
                    expEval.storeValues(identifier, in);
                }
                else if (input) {
                    cout << (lex.tokenInfo[i][j].first).substr(1, lex.tokenInfo[i][j].first.size() - 2);
                    cin >> temp;
                }
            }
 
            // check for commas
            else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::COMMA || (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::RIGHT_PAREN && j == lex.tokenInfo[i].size() - 1)) {

                if (print) {
                    if (exp != "") {
                        if (exp == identifier) {
                            if (expEval.findValue(identifier) == "Error.\n") {
                                cout << "\n-------------- ERROR --------------------\n" << endl;
                                cout << "Error on line " << i - 1 << ": " << identifier << " was not found in the symbol table." << endl;
                                break;
                            }
                            else {
                                cout << expEval.findValue(identifier);
                            }
 
                        }
                        else {
                          if (expEval.findValue(exp) != "Error.\n") {
                              cout << expEval.findValue(exp);
                          } else {
                              expEval.inToPost({ exp }, "-");
                              cout << expEval.findValue("-");
                              expEval.remove("-");
                          }
                        }
                    }
                    exp.clear();
                    identifier.clear();
                } else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::RIGHT_PAREN) {
                    exp += lex.tokenInfo[i][j].first;
                }
            } else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::LEFT_PAREN || lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::RIGHT_PAREN) {
                
                if (expression) {
                    exp += lex.tokenInfo[i][j].first;
                }
            } else if (lex.tokenInfo[i][j].second == lexicalAnalyzer::categoryType::RELATIONAL_OP) {
                if (expression) {
                    exp += lex.tokenInfo[i][j].first;
                }
            }
            
 
 
        }
 
        // check if line was flagged as an expression
        
        if (expression && (ifStmt || elifStmt || elseStmt) && lex.tokenInfo[i][0].second == lexicalAnalyzer::categoryType::KEYWORD) {
            expressionSaved = exp;
            if (ifFunction(expressionSaved) && !didPrevious) {
                expressionSaved = "1";
            } else {
                expressionSaved = "0";
            }
        } else if (expression && whileLoop && lex.tokenInfo[i][0].second == lexicalAnalyzer::categoryType::KEYWORD) {
            //cout << "updating while condition to " << exp << endl;
            whileCondition = exp;
            whileFunction();
        } else if (expression && !input) {
            if (!expEval.inToPost({ exp }, identifier)) {
                cout << "\n-------------- ERROR --------------------\n" << endl;
                cout << "Error on line " << i + 1 << endl;
                break;
            }
            
        }
        
        
        
        
        
 
        if (print) {
            cout << endl;
        }
 
        exp.clear();
        identifier.clear();
 
 
 
 
 
    }

 
}



// evaluates if statements
bool Interpreter::ifFunction(string stmt) {
    expEval.inToPost({stmt}, "if");
    string temp = expEval.findValue("if");
    expEval.remove("if");
    if (temp == "1") {
        return 1;
    }
    return 0;
}


// evaluates while loops
bool Interpreter::whileFunction() {
    expEval.inToPost({whileCondition}, "while");
    string temp = expEval.findValue("while");
    expEval.remove("while");
    if (temp == "1") {
        widerLoopValue = true;
        return true;
    }
    widerLoopValue = false;
    return false;
}






// show the variables
void Interpreter::showVariables() {
    if (expEval.symbolTable.size() == 0) {
        cout << "\t* Error. No variables to display. *" << endl;
    }
    else {
        cout << endl << " ---- Symbol Table ---- " << endl;
        for (auto x : expEval.symbolTable) {
            cout << "\t\t" << x.first << " = " << x.second << endl;
        }
    }
 
 
}


//clears data
void Interpreter::clear() {
    lex.clear();
    expEval.clearData();
}
