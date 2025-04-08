# Compiler
CXX = g++

# Source and header files
SRCS = main.cpp headers/nimbus.cpp headers/database.cpp headers/table.cpp headers/parser.cpp headers/executer.cpp headers/returnObject.cpp headers/environment.cpp
HEADERS = headers/nimbus.h headers/database.h headers/table.h headers/parser.h headers/executer.h headers/returnObject.h headers/environment.h


# Target executable
TARGET = nimbus

# Build the executable
all:
	$(CXX) $(SRCS) -o $(TARGET) 

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET)
