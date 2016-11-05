/*
 * Text Editor
 * 
 * Erika Moate
 * A00954431
 * Set 3O
 */

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

handleError(String message);

int main(int argc, char *argv[]) {
	map<String, String> highlight;
	if (argc == 1) {
		//process my config file
	} else if (argc == 2) {
		//process the input config
	} else {
		handleError("In Main: Invalid command line arguments");
	}

}


/*
 * Handles errors.
 * Reverts to default mode, prints error message, and exits the program.
 */
void handleError(String message) {
	// do that default mode stuff
	cout << message << endl;
	exit(EXIT_FAILURE);
}

/*
 * Processes the configuration file for text highlighting.
 *
 * Reads in line by line. If line has at least two words, adds them to the map.
 * If the key exits in the map, nothing is inserted.
 *
 * Example input line:
 * bold  \e[0;31m  #red
 */
bool processConfig(const map<String, String> &highlight
				   ifstream &is) {
	String line, key, color;
	stringstream ss;

	while (getline(is, line)) {
		ss.str(line);
		if (ss >> key) {
			if (ss >> color) {
				highlight.insert(pair<String, String>(key, color));
			}
	}
}
