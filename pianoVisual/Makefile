CXX= g++
CXXFLAGS= -std=c++20 -g
SFML_LIBS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -I/usr/include/python3.10/ -L/usr/lib/python3.10/config-3.10-x86_64-linux-gnu -lpython3.10
MUSIC_LIB = -L./midifile/lib -lmidifile

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
