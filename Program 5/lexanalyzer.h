//
//  lexanalyzer.h
//  program 2
//
//  Created by Ellieanna Ross on 9/20/22.
//

#ifndef lexanalyzer_h
#define lexanalyzer_h
#include <vector>
#include <string>




typedef std::vector<std::string> programType;

class lexicalAnalyzer {
    friend class Interpreter;
    friend class Interface;
public:
    enum class categoryType
    {
         KEYWORD,
         IDENTIFIER,
         STRING_LITERAL,
         NUMERIC_LITERAL,
         ASSIGNMENT_OP,
         ARITH_OP,
         LOGICAL_OP,
         RELATIONAL_OP,
         LEFT_PAREN,
         RIGHT_PAREN,
         COLON,
         COMMA,
         COMMENT,
         INDENT,
         UNKNOWN
    };
    
    lexicalAnalyzer();
    void analyzeCode(programType programCode);
    void display();
    void clear();
    friend class expEvaluator;
    
private:
    typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
    typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
    typedef std::pair<std::string, categoryType> pairType;
    tokenType tokenInfo;
    
    tokenLineType analyzeLine(std::string line);
    void printEnum(categoryType cat);

};



#endif /* lexanalyzer_h */
