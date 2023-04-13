#include "snake.hpp"


int main() {
    int height = 25 * 25, width = 25 * 30;
    sf::RenderWindow window(sf::VideoMode((float)width, (float)height), "Snake");
    snake game(width/25,height/25);
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
                    game.clickReset(pos);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                 if (event.key.code == sf::Keyboard::Up && game.direction != game.Direction::down)
                {
                    game.direction = game.Direction::up;
                }
                else if (event.key.code == sf::Keyboard::Down&& game.direction != game.Direction::up)
                {
                    game.direction = game.Direction::down;
                }
                else if (event.key.code == sf::Keyboard::Left && game.direction != game.Direction::right)
                {
                    game.direction = game.Direction::left;
                }
                else if (event.key.code == sf::Keyboard::Right&& game.direction != game.Direction::left)
                {
                    game.direction = game.Direction::right;
                }
            }
        }

        window.clear(sf::Color::White);
        game.move();
        game.draw(window);
        window.display();
    }

    return 0;

}