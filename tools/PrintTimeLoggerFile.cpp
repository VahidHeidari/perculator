/**
 * Perculator (Performance Calculator and viewer) is a simple time tracker.
 *
 * Copyright (C) 2015  Vahid Heidari (DeltaCode)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

