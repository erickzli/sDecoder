# Use C++ as the programming language
CXX = clang++
# -Wall shows all warning;
# -g shows all debugging info.
CXXFLAGS = -Wall -g -std=c++98 -stdlib=libc++

# Link main.o and decoder.o
main: main.o decoder.o decoder_helper.o json_writer.o fill_decoder.o line_decoder.o marker_decoder.o
	$(CXX) $(CXXFLAGS) -o main main.o decoder.o decoder_helper.o json_writer.o fill_decoder.o line_decoder.o marker_decoder.o

# Compile main.o and decoder.o
main.o: main.cc decoder.hh decoder_helper.hh json_writer.hh fill_decoder.hh line_decoder.hh marker_decoder.hh
	$(CXX) $(CXXFLAGS) -c main.cc
decoder.o: decoder_helper.hh decoder.hh fill_decoder.hh line_decoder.hh marker_decoder.hh
decoder_helper.o: decoder_helper.hh
fill_decoder.o: fill_decoder.hh
line_decoder.o: line_decoder.hh
marker_decoder.o: marker_decoder.hh
json_writer.o: json_writer.hh

# Clean unwanted files.
clean:
	rm main *.o
