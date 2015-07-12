#include <string.h>

#include <iostream>
#include <string>

#include "../TimeLogger.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2) {
		cerr << "File name required!" << endl;
		return 1;
	}

	TimeLogger logger;
	string path(argv[1], strlen(argv[1]));
	logger.open_print(path);

	return 0;
}

