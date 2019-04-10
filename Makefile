# Use C++ as the programming language
CXX = g++
# -Wall shows all warning;
# -g shows all debugging info.
CXXFLAGS = -Wall -g

# Link main.o and parser.o
main: main.o parser.o
	$(CXX) $(CXXFLAGS) -o main main.o parser.o
# Link test.o and parser.o
testing: test.o parser.o
	$(CXX) $(CXXFLAGS) -o testing test.o parser.o

# Compile main.o and parser.o
main.o: main.cc parser.hh
	$(CXX) $(CXXFLAGS) -c main.cc
parser.o: parser.hh

# Clean unwanted files.
clean:
	rm testing main *.o
