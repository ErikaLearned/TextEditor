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
#include <cctype>

using namespace std;

#define DEFAULT_MODE ""

/*
 * Prototypes
 */
void handleError(string message);
bool processConfig(map<string, string> &highlight,
				   ifstream &is);
void whiteAfterStart(bool &afterStartWhiteFlag,
					 char &nextChar);
char escapedChar(bool &foundListFlag,
				 char &nextChar);

/******************************************************************************
 * Main
 *****************************************************************************/
int main(int argc, char *argv[]) {
	map<string, string> highlight;
	string openThis = "config"; // default file to open

	if (argc == 2) {
		openThis = argv[1];
	}

	ifstream ifs(openThis, ifstream::in);

	if (!ifs || !ifs.is_open()) {
		handleError("File " + openThis + " failed to open");
	}

	processConfig(highlight, ifs);
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
 * Prints out white space characters until no white space character is found.
 *
 * @param afterStartWhiteFlag is a flag indicating there was white space 
 * following the previous character.
 */
void whiteAfterStart(bool &afterStartWhiteFlag,
					 char &nextChar) {
	if (isspace(nextChar)) {
		afterStartWhiteFlag = true;
	}
	
	while (isspace(nextChar)) {
		cout << nextChar;
		cin.get(nextChar);
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
