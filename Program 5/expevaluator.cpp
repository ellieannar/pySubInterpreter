//
//  expevaluator.cpp
//  program 3
//
//  Created by Ellieanna Ross on 10/11/22.
//

#include "expevaluator.h"
#include "lexanalyzer.h"
#include "stack"
#include "iostream"
using namespace std;


double performOperation(double op1, double op2, string op);

expEvaluator::expEvaluator() {
    
    //precidences
    precidence["*"] = 5;
    precidence["/"] = 5;
    precidence["%"] = 5;
    precidence["+"] = 4;
    precidence["-"] = 4;
    precidence["<"] = 3;
    precidence["<="] = 3;
    precidence[">"] = 3;
    precidence[">="] = 3;
    precidence["!="] = 3;
    precidence["=="] = 3;
    precidence["not"] = 2;
    precidence["and"] = 1;
    precidence["or"] = 0;
    
}


vector<pair<string, lexicalAnalyzer::categoryType>> postfix;     //the postfix vector


bool expEvaluator::inToPost(programType infix, string s) {
    error = false;
    //cout << "exp evaluator called into action with the infix of: " << s << endl;
    //tokenize the expression
    lexicalAnalyzer tokenize;
    tokenize.analyzeCode(infix);
    

    
    stack<pair<string, lexicalAnalyzer::categoryType>> st;   //the stack

    
    
    //loop through every element in the tokens
    for (auto x : tokenize.tokenInfo) {
        for (auto y : x) {
            //if a token is a number
            if (y.second == lexicalAnalyzer::categoryType::NUMERIC_LITERAL) {
                postfix.push_back({y.first, y.second}); //push into postfix
                
            }
            else if (y.second == lexicalAnalyzer::categoryType::IDENTIFIER) {
                postfix.push_back({y.first, y.second});
            }
            else if (y.second == lexicalAnalyzer::categoryType::LEFT_PAREN) {
                st.push(y);  //if it's an open parentheses, push onto stack
            } else if (y.second == lexicalAnalyzer::categoryType::RIGHT_PAREN) {
                while (!st.empty() && st.top().first!= "(") {   //if it's a right parenthesis push everything into vector
                    postfix.push_back(st.top());
                    st.pop();
                }
                if (st.empty()) {
                    error = true;
                } else {
                    st.pop();
                }
                
                
            //if it's an arithemetic operator
            } else if (y.second == lexicalAnalyzer::categoryType::ARITH_OP) {
                //following the instructions
                if (!st.empty() && st.top().first != "("  && precidence[y.first] <= precidence[st.top().first]) {
                    postfix.push_back(st.top());    //if the precidence is less than the top of the stack, add top of stack
                    st.pop();                       //to the vector and pop it, pushing the value of lower precidence on
                    st.push(y);
                } else if (!st.empty() && st.top().first != "(" && precidence[y.first] > precidence[st.top().first]) {
                    //postfix.push_back(y);
                    //while (!st.empty() && st.top().first != "(" ) { //if the precidence is greater, take everything off the stack and put
                    //    postfix.push_back(st.top());                //into vector
                    //    st.pop();
                    //}
                    st.push(y);
                } else {        //else, push onto stack
                    st.push(y);
                }
                
            //if it's a logical operator
            } else if (y.second == lexicalAnalyzer::categoryType::LOGICAL_OP || y.second == lexicalAnalyzer::categoryType::RELATIONAL_OP) {
                while (!st.empty() && st.top().first != "(") {      //put all operators currently in stack (to nearest '(') into vector
                    postfix.push_back(st.top());
                    st.pop();
                }
                st.push(y);      //push logical op onto stack
            } else {
                error = true;
                return false;
            }
            
        }
    }
    
    //empty out the stack into the vector
    while (!st.empty()) {
        if (st.top().first != "(") {
            postfix.push_back(st.top());
        } else {
            error = true;
            return false;
        }
        st.pop();
    }
    

    

    if (error) {
        return false;
    }
    
    //call the postfix evaluator to get result
    int result = postEval();
    
    if (error) {
        return false;
    }
    storeValues(s, to_string(result));

    
    if (s == "none provided") {
        cout << "\tThe calculated result is " << result << endl;
    }
    

   
    
    return true;
    
}


//To evaluate an expression, string values can be converted to double values using stod().

double expEvaluator::postEval() {
    stack<double> st;
    
   
    
    double operand2;
    double operand1;
    
    for (auto x : postfix) {
        if (x.second == lexicalAnalyzer::categoryType::NUMERIC_LITERAL) {
            st.push(stod(x.first));
            continue;
        }
        else if (x.second == lexicalAnalyzer::categoryType::IDENTIFIER) {
            if (findValue(x.first) == "Error.\n") {
                //error = true;
            } else {
                st.push(stod(findValue(x.first)));
                continue;
            }

        }
        else if (x.second == lexicalAnalyzer::categoryType::ARITH_OP) {
            if (st.size() >= 2) {
                operand2 = st.top();
                st.pop();
                operand1 = st.top();
                st.pop();
                st.push(performOperation(operand1, operand2, x.first));
            } else {
                error = true;
                break;
            }
        } else if (x.second == lexicalAnalyzer::categoryType::LOGICAL_OP || x.second == lexicalAnalyzer::categoryType::RELATIONAL_OP) {
            if (x.first != "not" && st.size() >= 2) {
                if (st.size() >= 2) {
                    operand2 = st.top();
                    st.pop();
                    operand1 = st.top();
                    st.pop();
                    st.push(performOperation(operand1, operand2, x.first));
                } else {
                    error = true;
                    break;
                }
            }
            
            
            else {
                if (st.size() >= 1) {
                    operand1 = st.top();
                    st.pop();
                    st.push(!operand1);
                } else {
                    error = true;
                    st.push(-1);
                    break;
                }
            }
        }
        
            
        
    }
    
    
    if (st.empty()) {
        return -1;
    }
    return st.top();
}


double performOperation(double op1, double op2, string op) {
    if (op == "*") {
        return op1*op2;
    } else if (op == "/") {
        return op1/op2;
    }  else if (op == "%") {
        return int(op1)%int(op2);
    }  else if (op == "-") {
        return op1-op2;
    } else if (op == "+") {
        return op1+op2;
    } else if (op == "and") {
        return op1 && op2;
    } else if (op == "or") {
        return op1 || op2;
    }  else if (op == "<") {
        return op1 < op2;
    } else if (op == "<=") {
        return op1 <= op2;
    }  else if (op == ">") {
        return op1 > op2;
    } else if (op == ">=") {
        return op1 >= op2;
    }  else if (op == "==") {
        return op1 == op2;
    } else if (op == "!=") {
        return op1 != op2;
    }else {
        return op1;
    }
}

//clear the data from expEvaluator
void expEvaluator::clearData() {
    error = false;
    postfix.clear();
    symbolTable.clear();
}

//Find the value in the symbol table
string expEvaluator::findValue(string name) {
    if (symbolTable.find(name) != symbolTable.end()) {
        return symbolTable[name];
    }
    return "Error.\n";
}


//store the value 'value' in name in symbol table
void expEvaluator::storeValues(string name, string value) {
    symbolTable[name] = value;
}

//return precidence
int expEvaluator::rtnPrecidence(string op) {
    if (precidence.find(op) != precidence.end()) {
        return precidence[op];
    } else {
        return -1;  //if invalid precidence, return -1
    }
}

void expEvaluator::remove(string name) {
    symbolTable.erase(name);
}



