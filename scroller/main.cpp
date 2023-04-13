#include "scroller.hpp"


int main() {
    int height = 25 * 25, width = 25 * 30;
    sf::RenderWindow window(sf::VideoMode((float)width, (float)height), "Scroller");
    scroller game(width/25,height/25);
    vector<bool> keyVec;
    keyVec.resize(255, false);
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
            // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            // {
            //     cout << "here" << endl;
            //     game.movement.x--;
            //     if(game.canJump()){
            //         game.movement.y += 8;
            //     }
            // }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !keyVec[sf::Keyboard::Up])
            {
                if(game.canJump())
                    game.movement.y += 12;
                keyVec[sf::Keyboard::Up] = true;
            }
            else
                keyVec[sf::Keyboard::Up] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !keyVec[sf::Keyboard::Down])
            {
                game.movement.y--;
                keyVec[sf::Keyboard::Down] = true;
            }
            else
                keyVec[sf::Keyboard::Down] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !keyVec[sf::Keyboard::Left])
            {
                game.movement.x--;
                keyVec[sf::Keyboard::Left] = true;
            }
            else
                keyVec[sf::Keyboard::Left] = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !keyVec[sf::Keyboard::Right])
            {
                game.movement.x++;
                keyVec[sf::Keyboard::Right] = true;
            }
            else
                keyVec[sf::Keyboard::Right] = false;
        }
        game.move();
        game.movement.y--;
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        window.clear(sf::Color::White);
        game.draw(window);
        window.display();
    }

    return 0;

}