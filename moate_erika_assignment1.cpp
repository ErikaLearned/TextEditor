/*
 * Text Editor
 * 
 * Erika Moate
 * A00954431
 * Set 3O
 */

#include <iostream>
#include <string>

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
 */
bool processConfig(map<String, String> &highlight) {
	
}
