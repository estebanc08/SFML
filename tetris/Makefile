CXX=g++ -o
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17
SFML_LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

tetris: *.cpp *.hpp
	@$(CXX) $(CXXFLAGS) -o tetris.out $^ $(SFML_LIBS) && ./tetris.out

.PHONY: clean
clean:
	@rm -f tetris.out
