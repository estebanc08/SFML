CXX= g++
CXXFLAGS= -std=c++20 -g
SFML_LIBS= -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
MUSIC_LIB = -I./midifile/include -L./midifile/lib -lmidifile 

.PHONY: build
build: piano.out run

piano.out: piano.cpp main.cpp *.hpp
	@$(CXX) $(CXXFLAGS) -o piano.out $^ $(SFML_LIBS) $(MUSIC_LIB)

.PHONY: run
run: piano.out
	./piano.out

.PHONY: clean
clean:
	@rm -f piano.out
