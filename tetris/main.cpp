#include "tetris.hpp"

int main(){
    bool released = true;
    int width = 10, height = 20;
    sf::RenderWindow window(sf::VideoMode(width*TILE_WIDTH+TILE_WIDTH*8+PADDING,height*TILE_WIDTH), "Tetris");
    window.setFramerateLimit(60);
    tetris game(width,height);
    sf::Clock keyPressCooldown;
    const float cooldownTime = 0.05f;
    float timer = 0, gamespeed = game.getFallingSpeed();
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
                else if (event.type == sf::Event::KeyPressed) {
                    if(keyPressCooldown.getElapsedTime().asSeconds() > cooldownTime){
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                        {
                            if(released){
                                game.direction = game.Direction::up;
                                gamespeed = MOVING_SPEED;
                                released = false;
                            }
                            else
                                game.direction = game.Direction::none;
                        }
                        else if (event.key.code == sf::Keyboard::Down && !game.deleting)
                        {
                            game.direction = game.Direction::down;
                            gamespeed = MOVING_SPEED;
                        }
                        else if (event.key.code == sf::Keyboard::Left)
                        {
                            game.direction = game.Direction::left;
                            gamespeed = MOVING_SPEED;
                        }
                        else if (event.key.code == sf::Keyboard::Right)
                        {
                            game.direction = game.Direction::right;
                            gamespeed = MOVING_SPEED;
                        }
                        game.move();
                        keyPressCooldown.restart();
                    }
                    if(event.key.code == sf::Keyboard::R){
                        game.direction = game.Direction::none;
                        game.restart();
                        timer = gamespeed + 1;
                    }
                    else if(event.key.code == sf::Keyboard::P){
                        game.direction = game.Direction::none;
                        timer = gamespeed + 1;
                    }
                }
                else if (event.type == sf::Event::KeyReleased) {
                    released = true;
                    gamespeed = game.getFallingSpeed();
                }
            
        }
        window.clear(sf::Color::Black);
        if (timer > gamespeed){
            game.update();
            timer = 0;
        }
        else
            timer++;
        game.draw(window);
        window.display();
    }
    return 0;
}