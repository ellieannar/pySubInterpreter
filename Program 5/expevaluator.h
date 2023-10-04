//
//  expevaluator.h
//  program 3
//
//  Created by Ellieanna Ross on 10/11/22.
//

#ifndef expevaluator_h
#define expevaluator_h

#include <map>
#include <string>
#include <vector>




class expEvaluator {
private:
    typedef std::vector<std::string> programType;
    typedef std::map<std::string, std::string> symbolTableType;
    symbolTableType symbolTable;
    bool error = false;
    std::map<std::string, int> precidence;
    
public:
    expEvaluator();
    bool inToPost(programType, std::string s = "none provided");
    double postEval();
    void storeValues(std::string name, std::string value);
    std::string findValue(std::string name);
    void clearData();
    int rtnPrecidence(std::string op);
    friend class Interpreter;
    void remove(std::string name);
};

#endif /* expevaluator_h */

    /*
     
     Following are the precedence levels for the relational, logical, and arithmetic operators defined for the PySub language, with 5 has the highest precedence:

     5     *, /, %
     4     +, -
     3     <, <=, >, >=, !=, ==
     2     not
     1     and
     0     or
     
     */
