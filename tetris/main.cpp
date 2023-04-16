#include "tetris.hpp"
#include <atomic>
#include <SFML/Audio.hpp>
#include <thread>

int main(){
    bool released = true;
    int width = 10, height = 20;
    sf::RenderWindow window(sf::VideoMode(width*TILE_WIDTH+TILE_WIDTH*8+PADDING,height*TILE_WIDTH), "Tetris");
    window.setFramerateLimit(60);
    tetris game(width,height);
    sf::Clock keyPressCooldown;
    const float cooldownTime = 0.05f;
    float timer = 0, gamespeed = game.getFallingSpeed();
    std::atomic<bool> skipSong = false, backSong = false;
    vector<sf::Music*> playlist;
    int numSongs = 6;
    for(int i = 0; i < numSongs; i++){
        string name = "music/tetris" + to_string(i) + ".wav";
        sf::Music* music = new sf::Music();
        music->openFromFile(name);
        playlist.push_back(music);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numSongs-1);
    int currSong = dis(gen);
    bool stopMusic = false;

    thread musicThread([&]() {
        while(!stopMusic) {
            if(currSong >= numSongs){
                currSong = 0;
            }
            else if(currSong < 0){
                currSong = numSongs - 1;
            }
            
            playlist[currSong]->play();
            while (playlist[currSong]->getStatus() == sf::Music::Playing) {
                sf::sleep(sf::milliseconds(100));
                if(stopMusic){
                    playlist[currSong]->stop();
                    break;
                }
                else if(skipSong){
                    playlist[currSong]->stop();
                    skipSong = false;
                    break;
                }
                else if(backSong){
                    playlist[currSong]->stop();
                    currSong -= 2;
                    backSong = false;
                    break;
                }
            }
            currSong++;
        }
        for(int i = 0; i < numSongs; i++){
            delete playlist[i];
        }
    });
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                stopMusic = true;
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if(keyPressCooldown.getElapsedTime().asSeconds() > cooldownTime){
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        if(released){
                            game.direction = game.Direction::up;
                            gamespeed = MOVING_SPEED;
                            released = false;
                            game.move();
                        }
                        else{
                            game.direction = game.Direction::none;
                            game.pause = false;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Down && !game.deleting)
                    {
                        game.direction = game.Direction::down;
                        gamespeed = MOVING_SPEED;
                        game.move();
                    }
                    else if (event.key.code == sf::Keyboard::Left)
                    {
                        game.direction = game.Direction::left;
                        gamespeed = MOVING_SPEED;
                        game.move();
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        game.direction = game.Direction::right;
                        gamespeed = MOVING_SPEED;
                        game.move();
                    }
                    keyPressCooldown.restart();
                }
                if(event.key.code == sf::Keyboard::R){
                    game.direction = game.Direction::none;
                    game.restart();
                    timer = gamespeed + 1;
                }
                else if(event.key.code == sf::Keyboard::P){
                    game.direction = game.Direction::none;
                    game.pause = true;
                    timer = gamespeed + 1;
                }
                else if(event.key.code == sf::Keyboard::F7){
                    skipSong = true;
                }
                else if(event.key.code == sf::Keyboard::F5){
                    backSong = true;
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
        if(!game.rowsDelete.empty())
            game.animateRowClear(window);
        else
            game.draw(window);
        window.display();
    }
    musicThread.join();
    return 0;
}