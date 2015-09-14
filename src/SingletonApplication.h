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

#ifndef SINGLETON_APPLILCATION_H_
#define SINGLETON_APPLILCATION_H_

#include <exception>
#include <string>

class SingletonException : public std::exception
{
public:
	SingletonException() : message("Singleton application exceptin.") {}
	SingletonException(const char* m) : message(m) {}
	SingletonException(const std::string& m) : message(m) {}

	virtual const char* what() throw()
	{
		return message.c_str();
	}

private:
	std::string message;
};

class SingletonApplication
{
public:
	static constexpr char* PID_FILE_PATH = (char*)"perculator.pid";

	SingletonApplication(const char* name = "Application");
	SingletonApplication(const std::string& name);
	~SingletonApplication();

private:
	void check_pid();

	std::string app_name;
};

#endif
