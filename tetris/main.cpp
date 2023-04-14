#include "tetris.hpp"
#define MOVING_SPEED 15
#define FALLING_SPEED 20

int main(){
    bool released = true;
    unsigned int timer = 0, gamespeed = FALLING_SPEED;
    int width = 10, height = 20;
    sf::RenderWindow window(sf::VideoMode(width*TILE_WIDTH,height*TILE_WIDTH), "Tetris");
    window.setFramerateLimit(60);
    tetris game(width,height);
    sf::Clock keyPressCooldown;
    const float cooldownTime = 0.05f;
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
                        else if (event.key.code == sf::Keyboard::Down)
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
                    }
                }
                else if (event.type == sf::Event::KeyReleased) {
                    released = true;
                    gamespeed = FALLING_SPEED;
                }
            
        }
        window.clear(sf::Color::White);
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