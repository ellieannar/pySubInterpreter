//
//  interface.h
//  program 1
//
//  Created by Ellieanna Ross on 9/8/22.
//


#ifndef interface_h
#define interface_h
#include <vector>
#include <string>
#include <map>
#include "interpreter.h"


typedef std::vector<std::string> programType;

class Interface {
public:
    Interface();
    void startInterface();
private:
    
    //vector of the program code
    programType programCode;
    lexicalAnalyzer lexAnalysis;    //lexical analyzer
    expEvaluator expEvaluation;     //expression evaluator
    Interpreter pysubi;             //interpreter
    //command options
    void help(std::string c);
    void quit();
    void read(std::string fileName);
    void show();
    void clear();
    void helpInterpreter();
    void showTokens();
    void interpretTheCode(std::string);
    //definitions of command choices
    std::map<std::string, std::string> definitions {{ "quit", "This command terminates the interface."},
                                        { "read", "This command reads program lines from a file and stores the lines into the program data structure." },
                                        { "show", "This command shows the lines of the program that are stored in program data structure." },
                                        { "clear", "This command clears out or deletes any lines that are stored in the program data structure." },
                                        { "help", "This command displays a helpful description of given commands."},
        { "show(tokens)", "This command will display the tokens and categories for each line of code"}, {"run", "This command will run the program in memory."}, {"show(variables)", "This command will display the variables and corresponding values in your code"}};
    
    
};


#endif /* interface_h */
