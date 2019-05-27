# Use C++ as the programming language
CXX = clang++
# -Wall shows all warning;
# -g shows all debugging info.
CXXFLAGS = -Wall -g -std=c++11 -stdlib=libc++

# Link main.o and parser.o
main: main.o parser.o parser_helper.o json_writer.o line_parser.o marker_parser.o
	$(CXX) $(CXXFLAGS) -o main main.o parser.o parser_helper.o json_writer.o line_parser.o marker_parser.o
# Link test.o and parser.o
testing: testing.o parser.o parser_helper.o json_writer.o
	$(CXX) $(CXXFLAGS) -o testing testing.o parser.o parser_helper.o json_writer.o

# Compile main.o and parser.o
main.o: main.cc parser.hh parser_helper.hh json_writer.hh line_parser.hh marker_parser.hh
	$(CXX) $(CXXFLAGS) -c main.cc
parser.o: parser_helper.hh parser.hh line_parser.hh marker_parser.hh
parser_helper.o: parser_helper.hh
line_parser.o: line_parser.hh
marker_parser.o: marker_parser.hh
json_writer.o: json_writer.hh

# Clean unwanted files.
clean:
	rm testing main *.o
