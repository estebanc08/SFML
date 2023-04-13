#include "sort.hpp"


int main() {
    // int height = 25 * 25, width = 25 * 30;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Sorts");
    int width = window.getSize().x;
    int height = window.getSize().y - 200;
    sorts game(width, height);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    string numString = to_string(game.size);
    sf::Cursor hand;
    hand.loadFromSystem(sf::Cursor::Text);
    sf::Cursor arrow;
    arrow.loadFromSystem(sf::Cursor::Arrow);
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
            else if (event.type == sf::Event::KeyPressed && game.isChangingSize()) {
                    if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
                        int digit = event.key.code - sf::Keyboard::Num0;
                        numString += to_string(digit);
                        game.updateSize(numString);
                    }
                    else if(event.key.code == sf::Keyboard::BackSpace){
                        numString = numString.substr(0, numString.size()-1);
                        game.updateSize(numString);
                    }
            }
            else if(event.type == sf::Event::MouseMoved){
                if(game.getButtons()["numbers"].second.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                    window.setMouseCursor(hand);
                else
                    window.setMouseCursor(arrow);
            }
        }
        window.clear(sf::Color::White);
        game.update(window);
        game.draw(window);
        window.display();
    }

    return 0;

}