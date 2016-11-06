/*
 * Text Editor
 * 
 * Erika Moate
 * A00954431
 * Set 3O
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

#define DEFAULT_MODE ""

/*
 * Prototypes
 */
void handleError(string message,
                 int &lineCount);
void handleError(string message);
bool processConfig(map<string, string> &highlight,
                   ifstream &is);
void whiteAfterStart(char &nextChar,
                     int &lineCount);
char escapedChar(bool &foundListFlag,
                 char &nextChar);
bool validCommand(string &command,
                  map<string,string> &commands);
void processEnd(vector<string> &commands,
                char &nextChar);
void checkWord(char &nextChar,
               string &command);
char processText(char &nextChar,
                 int &lineCount);
string fromMap(string &command,
               map<string, string> &commands);
void skipWhiteSpace(char &nextChar);
bool escapeChar(char &nextChar,
                char &prev);

/******************************************************************************
 * Main
 *****************************************************************************/
int main(int argc, char *argv[]) {
    map<string, string> highlight;
    vector<string> commands;
    string openThis = "config"; // default file to open
    char nextChar, check;
    string value;
    string command = " ";
    int lineCount = 1;
    if(argc == 2) {
        openThis = argv[1];
    }

    ifstream ifs(openThis, ifstream::in);
    
    if(!ifs || !ifs.is_open()) {
        handleError("File " + openThis + " failed to open");
    }

    if(!processConfig(highlight, ifs)) {
        handleError("Could not import " + openThis);
    }
    
    #ifdef DEBUG
    map<string, string>::iterator it = highlight.begin();
    while(it != highlight.end()) {
        cout << it->second << "(" << it->first << ")" << endl;
        it++;
    }
    
    return 0;
    #endif

    cin.get(nextChar);
    skipWhiteSpace(nextChar);
    
    if(nextChar != '(') {
        handleError("No command. First character was: " + nextChar, lineCount);
    }
    
    cin.get(nextChar);
    whiteAfterStart(nextChar, lineCount);
    checkWord(nextChar, command);
    string command2 = command.substr(1, command.size());
    if(validCommand(command2, highlight)) {
        value = highlight.at(command2);
        commands.push_back(move(value));
    } 
    skipWhiteSpace(nextChar);
    cout << commands.back();
    while(commands.size() > 0) {
        command = " ";
        command2 = "";
        check = processText(nextChar, lineCount);
        if(check == '(') {
            cin.get(nextChar);
            whiteAfterStart(nextChar, lineCount);
            command2 = nextChar;
            if (!escapeChar(nextChar, check)) {
                checkWord(nextChar, command);
                command2 += command.substr(2, command.size());
                if(validCommand(command2, highlight)) {
                    skipWhiteSpace(nextChar);
                    value = highlight.at(command2);
                    commands.push_back(value);
                    cout << commands.back();
                } else {
                    handleError("Invalid command " + command, lineCount);
                }
            } else {
                cin.get(nextChar);
            }
        } else {
            processEnd(commands, nextChar);
        }
    } 
    while(!cin.eof()) {
        cin.get(nextChar);
        if(!isspace(nextChar)) {
            handleError("Characters after format ends", lineCount);
        }
    }
}


/*
 * Handles errors.
 * Reverts to default mode, prints error message with line, and exits the program.
 */
void handleError(string message,
                 int &lineCount) {
    cout << DEFAULT_MODE << message << " at Line " << lineCount << endl;
    exit(EXIT_FAILURE);
}

/*
 * Handles errors.
 * Reverts to default mode, prints error message, and exits the program.
 */
void handleError(string message) {
    cout << DEFAULT_MODE << message << endl;
    exit(EXIT_FAILURE);
}

/*
 * Is the command valid?
 */
bool validCommand(string &command,
                  map<string, string> &commands) {
    if(commands.find(command) != commands.end()) {
        return true;
    }
    return false;
}

/*
 * Return map value.
 */
string fromMap(string &command,
               map<string, string> &commands) {
    map<string, string>::const_iterator pos = commands.find(command);
    return pos->second;
}

/*
 * Prints out white space characters until no white space character is found.
 */
void whiteAfterStart(char &nextChar,
                     int &lineCount) {
    while (isspace(nextChar)) {
        if(nextChar == '\n') {
            lineCount++;
        }
        cout << nextChar;
        cin.get(nextChar);
    }
}

/*
 * Skips white space.
 */
void skipWhiteSpace(char &nextChar) {
    while(isspace(nextChar)) {
        cin.get(nextChar);
    }
}

/*
 * Handles possible escaped characters.
 * Returns true if there is an escaped character.
 */
bool escapeChar(char &nextChar, char &prev) {
    if(nextChar == prev) {
        cout << nextChar;
        return true;
    }
    return false;
}

/*
 * Processes possible command end.
 */
void processEnd(vector<string> &commands,
                char &nextChar) {
    //cout << endl << "start " << nextChar << endl;
    cin.get(nextChar);
    //cout << "next " << nextChar << endl;
    if (nextChar == ')') {
        cout << nextChar;
        cin.get(nextChar);
    } else {
        commands.pop_back();
        if(commands.size() > 0) {
            cout << commands.back();
        }
    }
}

/*
 * Finds command after white space.
 * Ensures all escaped characters are included in the command.
 * When the function is over, @param nextChar will be the character after the
 * command. 
 */
void checkWord(char &nextChar,
               string &command) {
    char next;
    while (!isspace(nextChar)) {
        if(nextChar == '(' || nextChar == ')') {
            cin.get(next);
            if(next == nextChar) {
                command += next;
            } else {
                return;
            }
        } else {
            command += nextChar;
        }
        cin.get(nextChar);
    }
}

/*
 * Processes the configuration file for text highlighting.
 *
 * Reads in line by line. If line has at least two words, adds them to the map.
 * If the key exists in the map, nothing is inserted.
 * The second string must start with \e. \e is replaced with \003.
 *
 * Example input line:
 * bold  \e[0;31m  #red
 */
bool processConfig(map<string, string> &highlight,
                   ifstream &is) {
    string line, key, color;
    stringstream ss;

    highlight.clear();

    while (getline(is, line)) {
        ss.str(line);
        if (ss >> key) {
            if (ss >> color) {
                highlight.insert(pair<string, string>(key, color.replace(0, 2, "\033")));
            }
        }
    }    
    
    if (highlight.empty()) {
         return false;
    }
    return true;
}

/*
 * Displays text according to last format in vector. 
 * Function ends when a command may start or end.
 */
char processText(char &nextChar,
                 int &lineCount) {
    while(nextChar != '(' && nextChar != ')') {
	if(nextChar == '\n') {
            lineCount++;
        }
        cout << nextChar;
        cin.get(nextChar);
    }
    return nextChar;
}

