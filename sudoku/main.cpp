#include "sudoku.hpp"


int main() {
    int height = 50 * 11, width = 50 * 13;
    sf::RenderWindow window(sf::VideoMode((float)width, (float)height), "Sudoku");
    unordered_map<string, sf::Sprite> tiles;
    sudoku game(width, height);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonPressed){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    auto pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    game.leftClick = true;
                    game.setCurrCell(pos);
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    auto pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    game.leftClick = false;
                    game.setCurrCell(pos);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                    int digit = event.key.code - sf::Keyboard::Num0;
                    game.addNum(digit);
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) || sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
                    game.deleteNum();
                }
            }
        }

        window.clear(sf::Color::White);
        game.drawBoard(window);
        window.display();
    }

    // return 0;

}