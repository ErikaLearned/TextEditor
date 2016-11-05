/*
 * Text Editor
 * 
 * Erika Moate
 * A00954431
 * Set 3O
 *
 * Note: Looks best with tab set to 4 spaces.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <cctype>
#include <vector>

using namespace std;

#define DEFAULT_MODE ""

/*
 * Prototypes
 */
void handleError(string message);
bool processConfig(map<string, string> &highlight,
				   ifstream &is);
void whiteAfterStart(char &nextChar);
char escapedChar(bool &foundListFlag,
				 char &nextChar);
bool validCommand(string &command,
				  map<string,string> &commands);
void processEnd(vector<string> &commands,
				char &nextChar);
string checkWord(char &nextChar);
char processText(char &nextChar,
				 vector<string> commands);

/******************************************************************************
 * Main
 *****************************************************************************/
int main(int argc, char *argv[]) {
	map<string, string> highlight;
	vector<string> commands;
	string openThis = "config"; // default file to open
	char nextChar, check;
	string command;

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

	cin.get(nextChar);
	while(isspace(nextChar)) {
		cin.get(nextChar);
	}

	if(nextChar != '(') {
		handleError("No command. First character was: " + nextChar);
	}

	cin.get(nextChar);
	whiteAfterStart(nextChar);
	command = checkWord(nextChar);
	if(validCommand(command, highlight)) {
		commands.push_back(highlight[command]);
	}

	while(commands.size() > 0) {
		command = "";
		check = processText(nextChar, commands);
		if(check == '(') {
			cin.get(nextChar);
			whiteAfterStart(nextChar);
			command = checkWord(nextChar);
			if(validCommand(command, highlight)) {
				commands.push_back(highlight[command]);
			} else {
				handleError("Invalid command " + command);
			}
		} else {
			processEnd(commands, nextChar);
		}
	}
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
 * Prints out white space characters until no white space character is found.
 */
void whiteAfterStart(char &nextChar) {
	while (isspace(nextChar)) {
		cout << nextChar;
		cin.get(nextChar);
	}
}

/*
 * Processes possible command end.
 */
void processEnd(vector<string> &commands,
				char &nextChar) {
	cin.get(nextChar);
	if (nextChar == ')') {
		cout << nextChar;
	} else {
		commands.pop_back();
	}
}


/*
 * Reset flag indicating a list start or end character was found.
 */
char escapedChar(bool &foundListFlag,
				 char &nextChar) {
	foundListFlag = false;
	return nextChar;
}

/*
 * Finds command after white space.
 * Ensures all escaped characters are included in the command.
 * When the function is over, @param nextChar will be the character after the
 * command. 
 */
string checkWord(char &nextChar) {
	string command = 0;
	char cur, prev = nextChar;
	
	cin.get(cur);
	cin.get(nextChar);

	while (!isspace(cur)) {
		if (cur == '(' 
			|| cur == ')') {
				if (cur != nextChar || cur != prev) {
					return command;
				} else if (prev == cur) {
					command = command + cur;
					prev = nextChar;
					cin.get(cur);
					cin.get(nextChar);
				}
				else {
					command = command + cur;
					cin.get(prev);
					cin.get(cur);
					cin.get(nextChar);
				}
		} else {
			command = command + cur + nextChar;
			prev = nextChar;
			cin.get(cur);
			cin.get(nextChar);
		}
	}
	return command;
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
				 vector<string> commands) {
	cout << commands.back();
	while(nextChar != '(' || nextChar != ')') {
		cout << nextChar;
		cin.get(nextChar);
	}
	return nextChar;
}
