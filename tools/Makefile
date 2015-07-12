#
# Perculator (Performance Calculator and viewer) is a simple time tracker.
#
# Copyright (C) 2015  Vahid Heidari (DeltaCode)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CXXFLAGS=-g3 -Wall -Wextra -pedantic -D__DEBUG__ -Dconstexpr=const
CXX=g++
OBJ=printtimeloggerfile.o ../timelogger.o
OUTPUT=print.exe

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(OUTPUT)

printtimeloggerfile.o: PrintTimeLoggerFile.cpp ../TimeLogger.cpp ../TimeLogger.h
	$(CXX) $(CXXFLAGS) -c PrintTimeLoggerFile.cpp -o printtimeloggerfile.o

../timelogger.o: ../TimeLogger.cpp ../TimeLogger.h
	$(CXX) $(CXXFLAGS) -c ../TimeLogger.cpp -o ../timelogger.o

clean_all:
	rm -rf *.o *.S *.s *.ii $(OUTPUT)
clean_vim:
	rm -rf *~
clean: clean_all clean_vim
