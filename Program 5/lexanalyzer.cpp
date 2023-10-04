//
//  lexanalyzer.cpp
//  program 2
//
//  Created by Ellieanna Ross on 9/20/22.
// read(/Users/ellieannaross/Desktop/test.py)
#include <iostream>
#include "lexanalyzer.h"
#include <vector>
#include <queue>
using namespace std;


//the vector of the program type
typedef vector<string> programType;

//the keywords that the program must recognize
const vector<string> keywords = {"print", "elif", "if", "else", "while", "int", "input", };


//constructor
lexicalAnalyzer::lexicalAnalyzer() {}


//analyzes the whole overall code
void lexicalAnalyzer::analyzeCode(programType program) {
    
    
    
    for (int i = 0; i < program.size(); i++) {          //loops through each line in the program code
        tokenInfo.push_back(analyzeLine(program[i]));      //analyzes each line in the code
    }
    
    
}

//analyzes individual lines of code
lexicalAnalyzer::tokenLineType lexicalAnalyzer::analyzeLine(string line) {
    pairType pair;
    tokenLineType lineType;

    //push the whole line into queue named lineStack
    queue<char> lineStack;
    for (auto x : line) {
        lineStack.push(x);
    }
    

    
    //loop through entire line
    while (lineStack.empty() == false) {

        //if the line is a comment, mark the whole line as a comment and return
        if (lineStack.front() == '#') {
            string temp;
            while (!lineStack.empty()) {
                temp.push_back(lineStack.front());
                lineStack.pop();
            }
            pair.first = temp;                       //first is x
            pair.second = categoryType::COMMENT;    //second is comment
            lineType.push_back(pair);               //push the pair to the lineType
            return lineType;
        }

        
        //check if the first val is a digit
        else if (isdigit(lineStack.front())) {
            string temp;
            while (!lineStack.empty() && isdigit(lineStack.front())) {    //go through and check for value of digit
                temp.push_back(lineStack.front());
                lineStack.pop();
            }
            pair.first = temp;                              //push the number as the first item
            pair.second = categoryType::NUMERIC_LITERAL;    //push the second value as num_literal
            lineType.push_back(pair);                       //push the pair to the lineType
        }
        
        //check if first val is alphanumeric
        else if (isalpha(lineStack.front())) {
            string temp;
            while (!lineStack.empty() && isalpha(lineStack.front())) {            //push alphanumeric into temp
                temp.push_back(lineStack.front());
                lineStack.pop();
            }
            pair.second = categoryType::IDENTIFIER;     //second is identifyer
            
            if (find(keywords.begin(), keywords.end(), temp) != keywords.end()) {
                pair.second = categoryType::KEYWORD;    //second is keyword
            }
            if (temp == "and" || temp == "or" || temp == "not") {
                pair.second = categoryType::LOGICAL_OP;
            }
            
            
            pair.first = temp;                          //first is string
            lineType.push_back(pair);                   //push pair to lineType
        }
        
        //check if it's a string literal
        else if (lineStack.front() == '\"' || lineStack.front() == '\'') {
            char searchingFor = lineStack.front();      //am I looking for a ' or "
            
            
            string temp;
            lineStack.pop();
            temp.push_back(searchingFor);
            while (!lineStack.empty() && lineStack.front() != searchingFor) { //loop through until closing is found
                temp.push_back(lineStack.front());                      //add on to the temp string
                lineStack.pop();
            }
            temp.push_back(searchingFor);
            
            
            pair.first = temp;                                          //add the string as first
            pair.second = categoryType::STRING_LITERAL;                 //second is string_lit
            lineType.push_back(pair);                                   //add pair to lineType
            
            lineStack.pop();                                            //remove the " from the front of the queue
        }
        
        //check if it's a (
        else if (lineStack.front() == '(') {
            pair.first = lineStack.front();
            pair.second = categoryType::LEFT_PAREN;
            lineType.push_back(pair);   //push the pair to the line
            lineStack.pop();
        }
        
        //check if it's a )
        else if (lineStack.front() == ')') {
            pair.first = lineStack.front();
            pair.second = categoryType::RIGHT_PAREN;
            lineType.push_back(pair);   //push the pair to the line

            lineStack.pop();
        }
        
        //check if it's a ,
        else if (lineStack.front() == ',') {
            pair.first = lineStack.front();
            pair.second = categoryType::COMMA;
            lineType.push_back(pair);   //push the pair to the line

            lineStack.pop();
        }
        
        //check if it's a :
        else if (lineStack.front() == ':') {
            pair.first = lineStack.front();
            pair.second = categoryType::COLON;
            lineType.push_back(pair);   //push the pair to the line

            lineStack.pop();
        }
        
        //check if it's a space at the beginning of the line
        else if ((lineStack.front() == ' ' || lineStack.front() == '\t') && lineType.empty()) {
            pair.first = lineStack.front();
            pair.second = categoryType::INDENT;
            lineType.push_back(pair);   //push the pair to the line

            lineStack.pop();
        }
        
        //check for assignment operator
        else if (lineStack.front() == '=' ) {
            lineStack.pop();
            if (lineStack.front() == '=') {     //if its a double ==, then it's a relational operator
                pair.first = "==";
                pair.second = categoryType::RELATIONAL_OP;
                lineStack.pop();
            } else {                        //if it's a single =, then its an assignment op
                pair.first = "=";
                pair.second = categoryType::ASSIGNMENT_OP;
            }
            
            lineType.push_back(pair);       //push the pair to the line
            
        }
        
        //check for relational operator <
        else if (lineStack.front() == '<') {
            pair.first = lineStack.front();
            lineStack.pop();
            if (lineStack.front() == '=') {     //if its <=, then save that as the pair.first
                pair.first = "<=";
                lineStack.pop();
            }
            
            pair.second = categoryType::RELATIONAL_OP;
            
            lineType.push_back(pair);       //push the pair to the line
        }
        
        //check for relational operator >
        else if (lineStack.front() == '>') {
            pair.first = lineStack.front();
            lineStack.pop();
            if (lineStack.front() == '=') {         //if it's >=, then save that as the pair.first
                pair.first = ">=";
                lineStack.pop();
            }
            
            pair.second = categoryType::RELATIONAL_OP;
            
            lineType.push_back(pair);   //push the pair to the line
        }
        
        //check for relational operator !
        else if (lineStack.front() == '!') {
            pair.first = lineStack.front();
            lineStack.pop();
            if (lineStack.front() == '=') {         // if it's != then save that as pair.first
                pair.first = "!=";
                lineStack.pop();
            }
            
            pair.second = categoryType::RELATIONAL_OP;
            
            lineType.push_back(pair);       //push the pair to the stack
        }
        
        //check for arithmetic operator
        else if (lineStack.front() == '+' || lineStack.front() == '-' || lineStack.front() == '/' || lineStack.front() == '*' || lineStack.front() == '%') {
            pair.first = lineStack.front();
            lineStack.pop();
            pair.second = categoryType::ARITH_OP;
            
            lineType.push_back(pair);
        }
        
        //check if it's another character that's unknown
        else if (lineStack.front() != ' '){
            pair.first = lineStack.front();
            pair.second = categoryType::UNKNOWN;
            lineType.push_back(pair);       //if it's unknown, save that to the line type
            lineStack.pop();
        }
        
        //check if it's a space that's not at the beginning
        else {
            lineStack.pop();
        }
        
    }
    
    return lineType;
    
}





//print out the tokens
void lexicalAnalyzer::display() {
    if (tokenInfo.empty()) {
        cout << "\t* There are no tokens to display. Please read in a file, or type \'help\' for more information. *" << endl;
        return;
    }
    cout << "\t***** TOKEN INFORMATION *****" << endl;
    int lineNumber = 0;
    for (auto type : tokenInfo) {
        cout << "Line #" << lineNumber << ":" << endl;
        int tokenNumber = 0;
        for (auto lineType : type) {
            cout << "Token[" << tokenNumber << "]: ";
            cout << lineType.first;
            printEnum(lineType.second);
            tokenNumber++;
        }
        cout << "--------------------" << endl;
        lineNumber++;
    }
}


void lexicalAnalyzer::printEnum(categoryType cat) {
    if (cat == categoryType::KEYWORD) {
        cout << " - KEYWORD" << endl;
    } else if (cat == categoryType::IDENTIFIER) {
        cout << " - IDENTIFIER" << endl;
    }  else if (cat == categoryType::STRING_LITERAL) {
        cout << " - STRING LITERAL" << endl;
    }  else if (cat == categoryType::NUMERIC_LITERAL) {
        cout << " - NUMERIC LITERAL" << endl;
    }  else if (cat == categoryType::ASSIGNMENT_OP) {
        cout << " - ASSIGNMENT OPERATOR" << endl;
    }  else if (cat == categoryType::ARITH_OP) {
        cout << " - ARITHMETIC OPERATOR" << endl;
    }  else if (cat == categoryType::LOGICAL_OP) {
        cout << " - LOGICAL OPERATOR" << endl;
    }  else if (cat == categoryType::LEFT_PAREN) {
        cout << " - LEFT PARENTHESIS" << endl;
    }  else if (cat == categoryType::RIGHT_PAREN) {
        cout << " - RIGHT PARENTHESIS" << endl;
    }  else if (cat == categoryType::COLON) {
        cout << " - COLON" << endl;
    }  else if (cat == categoryType::COMMA) {
        cout << " - COMMA" << endl;
    }  else if (cat == categoryType::COMMENT) {
        cout << " - COMMENT" << endl;
    }  else if (cat == categoryType::INDENT) {
        cout << " - INDENT" << endl;
    } else if (cat == categoryType::RELATIONAL_OP) {
        cout << " - RELATIONAL OPERATOR" << endl;
    } else {
        cout << " - UNKNOWN" << endl;
    }
        
}


//clear all token info
void lexicalAnalyzer::clear() {
    tokenInfo.clear();
}





/*
 Loop through every character, c, in a program line:

   ✅ if c is a digit, collect numeric literal as long as you read digits

    ✅ if c is a letter, collect an identifier as long as you read alphabetic characters (also check for keywords and Logical operators)

   ✅  if c is a single or double quote, collect a string literal as long as you read characters other than a single or double quote

   ✅  if c is a parenthesis, colon, or comma, classify it as LEFT_PAREN, RIGHT_PAREN, COLON, or COMMA as appropriate

   ✅  if c is #, collect comment to end of line

   ✅  if c is space character (i.e. space or tab), ignore it and read next character unless it is the beginning of the line, then store as INDENT

    ✅ if c is an operator, identify whether it is an Assignment, Relational, or Arithmetic operator

    ✅ Otherwise, classify as UNKNOWN
 
        To do:
            ✅ Display the enum type...?
            ✅ return the lineType to the caller & put into tokenType
            ✅ determine if someting is keyword
                ✅ make arr of keywords & then check if identifyer exists in the keywords arr
            ✅ finish commenting
            ✅ check for logic stuff
 */
