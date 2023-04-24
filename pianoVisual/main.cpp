#include "piano.hpp"

int main(int argc, char const *argv[]){
    Piano piano;

    piano.readMidi("outputMidi/norma.mid");
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING), "piano");
    sf::Music music;

    if (!music.openFromFile("outputWav/norma.wav")) {
        return 1;
    }

    sf::RectangleShape line(sf::Vector2f(1, WHITE_KEY_HEIGHT));
    line.setFillColor(sf::Color::Black);
    sf::Event event;
    music.play();
    sf::Clock clock;
    bool exit = false;
    for (unsigned int i = 0; i < piano.notes.size(); i++)
    {
        window.clear();
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
                exit = true;
                break;
            }
        }
        if(exit)
            break;
        for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
            piano.whiteKeys[i].setFillColor(sf::Color::White);
            window.draw(piano.whiteKeys[i]);
        }
        for(int i = 1; i < 56; i++){
            line.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
            window.draw(line);
        }
        for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
            piano.blackKeys[i].setFillColor(sf::Color::Black);
            window.draw(piano.blackKeys[i]);
        }
        while (clock.getElapsedTime().asSeconds() < piano.notes[i]->getStartTime())
            sf::sleep(sf::milliseconds(1));
        do{
            int key = 0;
            if(piano.notes[i]->getKey()[1] != 'b'){
                if(piano.notes[i]->getKey()[0] == 'A' || piano.notes[i]->getKey()[0] == 'B')
                    key = piano.notes[i]->getKey()[0] - 'A' + 7*(piano.notes[i]->getKey()[1]-'0');
                else
                    key = piano.notes[i]->getKey()[0] - 'A' + 7*(piano.notes[i]->getKey()[1]-'0'-1);
                piano.whiteKeys[key].setFillColor(sf::Color::Green);
                window.draw(piano.whiteKeys[key]);
            }
            else{
                if(piano.notes[i]->getKey()[0] == 'B'){
                    key = 5*(piano.notes[i]->getKey()[2]- '0');
                }
                else if(piano.notes[i]->getKey()[0] == 'D'){
                    key = 5*(piano.notes[i]->getKey()[2]- '0' -1) + 1;
                }
                else if(piano.notes[i]->getKey()[0] == 'E'){
                    key = 5*(piano.notes[i]->getKey()[2]- '0' -1) + 2;
                }
                else if(piano.notes[i]->getKey()[0] == 'G'){
                    key = 5*(piano.notes[i]->getKey()[2]- '0' -1) + 3;
                }
                else{
                    key = 5*(piano.notes[i]->getKey()[2]-'0'-1)+4;
                }
                piano.blackKeys[key].setFillColor(sf::Color::Red);
                window.draw(piano.blackKeys[key]);
            }
            i++;
        }
        while(i < piano.notes.size() && piano.notes[i]->getStartTime() == piano.notes[i-1]->getStartTime());
            
        for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
            window.draw(piano.whiteKeys[i]);
        }
        for(int i = 1; i < 56; i++){
            line.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
            window.draw(line);
        }
        for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
            window.draw(piano.blackKeys[i]);
        }
        window.display();
    }

    while (music.getStatus() == sf::Music::Playing && !exit) {};

}