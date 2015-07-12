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
