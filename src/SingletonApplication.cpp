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

#include "SingletonApplication.h"

#include <iostream>
#include <fstream>

#include <unistd.h>
#include <signal.h>

using namespace std;

constexpr char* SingletonApplication::PID_FILE_PATH;

SingletonApplication::SingletonApplication(const char* name)
: app_name(name)
{
	check_pid();
}

SingletonApplication::SingletonApplication(const string& name)
: app_name(name)
{
	check_pid();
}

SingletonApplication::~SingletonApplication()
{
	// Clear pid file if exists.
	remove(PID_FILE_PATH);
}

void SingletonApplication::check_pid()
{
	// Read PID file if exists.
	ifstream pid_file_in(PID_FILE_PATH);
	if (pid_file_in.is_open()) {
		pid_t running_pid = 0;
		pid_file_in >> running_pid;
		pid_file_in.close();

		// Check PID is valid or not.
		if (running_pid && (kill(running_pid, 0) == 0)) {		// Process exists with the same PID.
			// Check name of found process.
			char buff[250];
			string tmp = "/proc/" + to_string(running_pid) + "/exe";
			if (readlink(tmp.c_str(), buff, sizeof(buff))) {
				tmp.clear();
				tmp.assign(buff);

				if (tmp.find(app_name) != string::npos)
					throw new SingletonException("There is a running application!\n"
							"Only one instance of " + app_name + " is allowed.");
			}
		}
	}
	
	// Create PID file.
	pid_t app_pid = getpid();
	ofstream pid_out(PID_FILE_PATH);
	pid_out << app_pid;
	pid_out.close();
}
