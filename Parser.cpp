/*
 * Created by Francois W. Nel on 27 Jun 2016.
 */

#include "Parser.h"

using namespace std;

Parser::Parser(string& fileName) {
    fin.open(fileName);

    if (fin.fail()) {
        cout << fileName << " not found." << endl;
        exit(1);
    }

    // Populate the command map table.
    commandTable['@'] = 'A';
    commandTable['A'] = 'C';
    commandTable['D'] = 'C';
    commandTable['M'] = 'C';
    commandTable['0'] = 'C';
    commandTable['1'] = 'C';
    commandTable['-'] = 'C';
    commandTable['!'] = 'C';
    commandTable['('] = 'L';
    commandTable['.'] = 'L';
    
}

bool Parser::hasMoreCommands() {
    return !fin.eof();
}

void Parser::advance(unsigned long& lineNr) {
    string currentLine;
    unsigned long commentPos;
    bool commandFound;

    commandFound = false;

    // Read lines until a command is found.
    while (!commandFound && getline(fin, currentLine)) {
        // Increment the line number corresponding to the source file,
        //  used for error tracking.
        lineNr++;

        // Remove whitespace.
        currentLine.erase(remove_if(currentLine.begin(), currentLine.end(), ::isspace), currentLine.end());

        // Remove comments.
        commentPos = currentLine.find("//");
        if (commentPos != string::npos) {
            currentLine.erase(commentPos, currentLine.length() - commentPos);
        }

        // If the remaining line is not empty, a command has been found.
        commandFound = !currentLine.empty();
    }

    currentCommand = currentLine;
}

char Parser::commandType(unsigned long& lineNr) {

    if (commandTable.find(currentCommand[0]) != commandTable.end()) {
        return commandTable[currentCommand[0]];
    }

    // If an invalid command is found, output an error message and line number.
    
    return ' ';
    
}
string Parser::getBitSet(){
  
  //decimal input
    if (currentCommand.find_first_not_of("0123456789") == string::npos) {
        return symbol();
    }
    else if (currentCommand.find("0x") != string::npos) {
        int getStart = symbol().find("x");
        int x;
        stringstream ss;
        ss << hex << currentCommand.substr(getStart, currentCommand.length() - 1);
        ss >> x;
        if (x > 32767) {
            return " ";
        }
        string ret = to_string(x);
        return ret;
    }
    //hex input
    else if (currentCommand.find("0X") != string::npos) {
        int getStart = symbol().find("X");
        int x;
        stringstream ss;
        ss << hex << currentCommand.substr(getStart, currentCommand.length() - 1);
        ss >> x;
        if (x > 32767) {
            return " ";
        }
        string ret = to_string(x);
            return ret;
    }
    //binary input
    else if (currentCommand.find("0b") != string::npos) {

    int getStart = currentCommand.find("b");

    return currentCommand.substr(getStart + 1, currentCommand.length() - 1);
    }
  else if(currentCommand.find("0B") != string::npos){
    int getStart = currentCommand.find("B");

    return currentCommand.substr(getStart + 1, currentCommand.length() - 1);
  }

  
}



////////////////////////////////////////////
string Parser::symbol() {
    unsigned long openBracketPos, closeBracketPos;
    if (currentCommand.front() == '.') {
        openBracketPos = currentCommand.find('.');
        closeBracketPos = currentCommand.find('U');

    }
    else{
    openBracketPos = currentCommand.find('(');
    closeBracketPos = currentCommand.find(')');
    }

    // A-instruction: return everything after the '@' character.
    if (currentCommand[0] == '@') {

     
        return currentCommand.substr(1, currentCommand.length() - 1);
    }
    // L-instruction: return everything in between the '(' and ')' characters.
    else if (openBracketPos != string::npos && closeBracketPos != string::npos && currentCommand.at(openBracketPos) == '(') {
        return currentCommand.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1);
    }
    else if (openBracketPos != string::npos && closeBracketPos != string::npos && currentCommand.at(openBracketPos) == '.') {

        int value = currentCommand.find_first_of("0123456789");
        int len = value - closeBracketPos -1 ;
      
        string other = currentCommand.substr(closeBracketPos + 1, len);
        if (other.find_first_not_of("qazwsxedcrfvtgbyhnujmikolpQAZWSXEDCRFVTGBYHNUJMIKOLP") != string::npos){
          return " ";
        }
        return other;
    }

    // If the function was called in error, return a blank string.
    return "";
}

string Parser::destM() {
    unsigned long equalSignPos;

    equalSignPos = currentCommand.find('=');

    // Return everything before the '=' character.
    if (equalSignPos != string::npos) {
        return currentCommand.substr(0, equalSignPos);
    }

    // If no destination was specified, return a blank string.
    return "";
}

string Parser::compM() {
    unsigned long equalSignPos, semiColonPos;

    equalSignPos = currentCommand.find('=');
    semiColonPos = currentCommand.find(';');

    // Return the computation mnemonic based on three cases.
    if (equalSignPos != string::npos) {
        if (semiColonPos != string::npos) {
            // Case 1: dest = comp ; jump
            return currentCommand.substr(equalSignPos + 1, semiColonPos - equalSignPos - 1);
        }
        // Case 2: dest = comp
        return currentCommand.substr(equalSignPos + 1, currentCommand.length() - equalSignPos - 1);
    }
    else if (semiColonPos != string::npos) {
        // Case 3: comp ; jump
        return currentCommand.substr(0, semiColonPos);
    }

    // If no computation was specified, return a blank string.
    // This will result in an error message at the line number.
    // The error is handled by CodeTranslator.comp().
    return "";
}

string Parser::jumpM() {
    unsigned long semiColonPos;

     semiColonPos = currentCommand.find(';');

    // Return everything after the ';' character.
    if (semiColonPos != string::npos) {
        return currentCommand.substr(semiColonPos + 1, currentCommand.length() - semiColonPos - 1);
    }

    // If a jump was not specified, return a blank string.
    return "";
}
int Parser::getEQUValue() {
    unsigned long startPos, endPos, midPos;
    int value;
    startPos = currentCommand.find_first_of("0123456789");
    endPos = currentCommand.find_first_not_of("0123456789",startPos);

    int len = endPos - startPos - 1;
    
    string str = currentCommand.substr(startPos , len);
    stringstream stream(str);
    
    stream >> value;
    return value;
}
char Parser::getFirstChar() {
    return currentCommand.at(0);
}
