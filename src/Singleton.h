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

#ifndef TIME_LOGGER_SINGELTON_H_
#define TIME_LOGGER_SINGELTON_H_

template <typename T>
class Singleton
{
public:
	Singleton() : instance(nullptr) {}
	~Singleton() { delete instance; }

	T* get_instance()
	{
		if (!instance)
			instance = new T();

		return instance;
	}

private:
	T* instance;
};

#endif

