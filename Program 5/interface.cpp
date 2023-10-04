//
//  interface.cpp
//  program 1
//
//  Created by Ellieanna Ross on 9/8/22.
//  read(/Users/ellieannaross/Desktop/test.py)

#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"
#include <iostream>
#include <regex>
#include <fstream>
using namespace std;


//helpInterpreter function declaration
void helpInterpreter();


//interface constructer
Interface::Interface() {
}

void Interface::startInterface() {
    string command;
    
    regex helpCommand = regex("help\\([\\s\\S]*\\)");       //check to see if a help command was typed
    regex readCommand = regex("read\\([\\s\\S]*\\)");       //check to see if a fileName was typed
    
    //general pySUB interpreter information
    cout << "pySUB Interpreter 1.0 - Ellieanna Ross" << endl;
    cout << "Enter program lines or read (<filename>.py) at command line interface" << endl;
    cout << "Type 'help' for more information, or 'quit' to exit" << endl;
    
    
    //Read in the command
    cout << endl << ">>> ";
    getline(cin >> ws, command);
    
    //Check for matching command and call that function
    while (command != "quit" && command != "quit()") {
        if (command == "help" || regex_match(command, helpCommand)) {
            help(command);
        } else if (regex_match(command, readCommand)) {
            clear();
            lexAnalysis.clear();
            expEvaluation.clearData();
            read(command);
            lexAnalysis.analyzeCode(programCode);
        } else if (command == "show" || command == "show()") {
            show();
        } else if (command == "clear" || command == "clear()") {
            clear();
            lexAnalysis.clear();
            expEvaluation.clearData();
            pysubi.clear();
        } else if (command == "show(tokens)") {
            lexAnalysis.display();
        } else if (command == "show(variables)") {
            pysubi.showVariables();
        } else if (command == "run" || command == "run()") {
            lexAnalysis.clear();
            expEvaluation.clearData();
            pysubi.clear();
            pysubi.interpret(programCode);
        } else {    //if there is an invalid command, display error
            lexAnalysis.clear();
            //pysubi.clear();
            if (!expEvaluation.inToPost({command})) {
                interpretTheCode(command);
            }
            //cout << "\t*That is an invalid command. Type 'help' to see all valid commands. *" << endl;
        }
        //get new command
        cout << endl << ">>> ";
        getline(cin >> ws, command);
        
    }
    //when user enters 'quit', the program is quit.
    quit();
}






//Qitting message
void Interface::quit() {
    cout << "\t*Thank you for using Ellieanna's pySUB interpreter. *" << endl;
    exit(EXIT_SUCCESS);
}


//read in the file
void Interface::read(string fileName) {
    fileName = fileName.substr(5, fileName.length()-6); //get the filename
    ifstream file;
    file.open(fileName);    //open the file
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            programCode.push_back(line);
        }
        cout << "\n\t* Program successfully read *" << endl;
    } else if (fileName != ""){     //if invalid filename is entered, error is thrown.
        cout << endl << "Error. File does not exist. Please try a new file." << endl;
    } else {    //if no filename is given, ask for the filename.
        cout << endl << "\tPlease enter the filename. " << "\n\nfilename> ";
        cin >> fileName;
        fileName = "read(" + fileName + ")";
        read(fileName); //call the program again with a new filename
    }

}

//show the code stored in programCode
void Interface::show() {
    int i = 0;
    for (auto x : programCode) {    //loops through programCode vector and displays the code
        cout << "[" << i << "] " << x << endl;
        i++;
    }
    
    //if there is no code, then display that
    if (programCode.size() == 0) {
        cout << "\t* There is no program code to be displayed. *" << endl;
    }
}



//clear all the code stored in programCode
void Interface::clear() {
    programCode.clear();
    
}




//help interpreter
void Interface::help(string c) {
    
    //make sure it's in the right format
    if (c == "help") {
        c = "help()";
    }
    
    //get the choice substring
    string choice = c.substr(5, c.length()-6);
    //display propper output
    if (choice != "" && definitions.find(choice) != definitions.end()) {
        cout << definitions[choice] << endl;     //if defined command given, display that command definition
    } else {            //else ask for propper command
        cout << endl << endl << "Welcome to the help utility!" << endl;
        cout << "* To return to the interpreter, type \'exit\'" << endl;
        cout << "* To get a description of any command, type the name of the command at the help> prompt" << endl << endl;
        cout << "clear\thelp\tquit\nread\tshow\tshow(tokens)\nrun\t\t\t\tshow(variables)" << endl << endl;
        cout << endl << endl << "help> ";
        helpInterpreter();
    }
    
    
}



//when in the help menu
void Interface::helpInterpreter() {
    string command;                 //get the help prompt input
    cin >> command;
    if (definitions.find(command) != definitions.end()) {   //if the prompt is defined, show definition
        cout << endl << definitions[command] << endl;
        cout << "To return to the interpreter, type \'exit\'" << endl;
        cout << endl << "help> ";
        helpInterpreter();
    } else if (command != "exit") {     //if prompt isn't defined, display error, else return to interpreter
        cout << "\tThat is not a valid command. Please type in a command listed above." << endl;
        cout << "help> ";
        helpInterpreter();
    }
}

void Interface::interpretTheCode(string code) {
    lexAnalysis.analyzeCode({code});
    vector<string> newCode;
    if (lexAnalysis.tokenInfo[0][0].second == lexicalAnalyzer::categoryType::KEYWORD && lexAnalysis.tokenInfo[0][0].first != "print") {
        string temp = code;
        newCode.push_back(temp);
        while (temp !=  "") {
            cout << "... ";
            getline(cin,temp);
            newCode.push_back(temp);
        }
        lexAnalysis.clear();
        lexAnalysis.analyzeCode(newCode);
        pysubi.interpret(newCode);
    } else {
        pysubi.interpret({code});
    }

    
}


