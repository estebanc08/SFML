#include "piano.hpp"

int main(int argc, char const *argv[]){
    Piano piano;
    std::string name = "mazeppa";
    if(argc > 1)
        name = std::string(argv[1]);
    std::string midiName = "outputMidi/" + name + ".mid";
    std::string wavName = "outputWav/" + name + ".wav";

    piano.readMidi(midiName);
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING+15.f), "piano");
    sf::Music music;

    if (!music.openFromFile(wavName)) {
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
            piano.whiteKeys[i].setFillColor(sf::Color(255,255,245));
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
            sf::sleep(sf::microseconds(100));
        do{
            int key = 0;
            if(piano.notes[i]->getKey()[1] != 'b'){
                if(piano.notes[i]->getKey()[0] == 'A' || piano.notes[i]->getKey()[0] == 'B')
                    key = piano.notes[i]->getKey()[0] - 'A' + 7*(piano.notes[i]->getKey()[1]-'0');
                else
                    key = piano.notes[i]->getKey()[0] - 'A' + 7*(piano.notes[i]->getKey()[1]-'0'-1);
                if(key < piano.whiteKeys.size()/2)
                    piano.whiteKeys[key].setFillColor(sf::Color(84, 148, 218)); //LHS piano
                else
                    piano.whiteKeys[key].setFillColor(sf::Color(124,252,0)); //RHS piano
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
                if(key < piano.blackKeys.size() / 2)
                    piano.blackKeys[key].setFillColor(sf::Color::Blue);
                else
                    piano.blackKeys[key].setFillColor(sf::Color::Green);
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