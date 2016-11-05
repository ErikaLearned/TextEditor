#include <iostream>
#include <string>

using namespace std;

handleError(String message);

int main(int argc, char *argv[]) {
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
handleError(String message) {
	// do that default mode stuff
	cout << message << endl;
	exit(EXIT_FAILURE);
}
