# Use C++ as the programming language
CXX = clang++
# -Wall shows all warning;
# -g shows all debugging info.
CXXFLAGS = -Wall -g -std=c++98 -stdlib=libc++

# Link main.o and parser.o
main: main.o version_seeker.o
	$(CXX) $(CXXFLAGS) -o main main.o version_seeker.o

# Compile main.o and parser.o
main.o: main.cpp version_seeker.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Clean unwanted files.
clean:
	rm main *.o
