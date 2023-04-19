CXX=g++ -o -g
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17
SFML_LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -ltinyxml2

piano: piano.cpp main.cpp *.hpp
	@$(CXX) $(CXXFLAGS) -o piano.out $^ $(SFML_LIBS) && ./piano.out

.PHONY: clean
clean:
	@rm -f piano.out

install-deps:
	sudo apt-get update
    sudo apt-get install libsfml-dev
	git clone https://github.com/leethomason/tinyxml2.git
	cd tinyxml2
	make install
