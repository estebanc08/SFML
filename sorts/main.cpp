#include "sort.hpp"


int main() {
    int height = 25 * 25, width = 25 * 30;
    sf::RenderWindow window(sf::VideoMode((float)width, (float)height+150), "Sorts");
    sorts game(width, height);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonPressed){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    auto pos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    game.clickType(pos);
                }
            }
        }
        window.clear(sf::Color::White);
        game.update(window);
        game.draw(window);
        window.display();
    }

    return 0;

}