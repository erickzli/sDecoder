# Use C++ as the programming language
CXX = clang++
# -Wall shows all warning;
# -g shows all debugging info.
CXXFLAGS = -Wall -g -std=c++98 -stdlib=libc++

# Link main.o and parser.o
main: main.o parser.o parser_helper.o json_writer.o fill_parser.o line_parser.o marker_parser.o
	$(CXX) $(CXXFLAGS) -o main main.o parser.o parser_helper.o json_writer.o fill_parser.o line_parser.o marker_parser.o

# Compile main.o and parser.o
main.o: main.cc parser.hh parser_helper.hh json_writer.hh fill_parser.hh line_parser.hh marker_parser.hh
	$(CXX) $(CXXFLAGS) -c main.cc
parser.o: parser_helper.hh parser.hh fill_parser.hh line_parser.hh marker_parser.hh
parser_helper.o: parser_helper.hh
fill_parser.o: fill_parser.hh
line_parser.o: line_parser.hh
marker_parser.o: marker_parser.hh
json_writer.o: json_writer.hh

# Clean unwanted files.
clean:
	rm main *.o
