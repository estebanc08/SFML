#include "piano.hpp"


inline void drawNotes(sf::RenderWindow& window, Piano& piano, sf::Clock& clock, std::vector<std::pair<double, bool>>& whitePlaying, std::vector<std::pair<double, bool>>& blackPlaying, sf::Text& songPlaying){
    window.clear();
    window.draw(songPlaying);
    sf::RectangleShape redLine(sf::Vector2f(WHITE_KEY_WIDTH*56, 1));
    redLine.setPosition(SIDE_PADDING, VERTICAL_PADDING);
    redLine.setFillColor(sf::Color::Red);

    for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
        float posX = piano.whiteKeys[i].getPosition().x;
        if(clock.getElapsedTime().asSeconds() >= whitePlaying[i].first && whitePlaying[i].second || whitePlaying[i].second == false){
            piano.whiteKeys[i].setFillColor(sf::Color(255,255,245));
            window.draw(piano.whiteKeys[i]);
        }
        else{
            sf::RectangleShape pressed(piano.whiteKeys[i]);
            pressed.setSize(sf::Vector2f(pressed.getSize().x, pressed.getSize().y + 2.5));
            window.draw(pressed);
        }
    }

    window.draw(redLine);
    for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
        if(clock.getElapsedTime().asSeconds() >= blackPlaying[i].first && blackPlaying[i].second || blackPlaying[i].second == false)
            piano.blackKeys[i].setFillColor(sf::Color::Black);
        window.draw(piano.blackKeys[i]);
    }
    window.display();
}


int main(int argc, char const *argv[]){
    Piano piano;

    //start the song from current position
    std::ifstream musicIn("songList.txt");
    std::vector<std::string> musicList;
    std::string name;
    while(getline(musicIn, name))
        musicList.push_back(name);
    int currSong = 0;

    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING+15.f), "piano");

    sf::Event event;
    bool exit = false;
    bool paused = false;
    std::vector<std::pair<double, bool>> whitePlaying(piano.whiteKeys.size(), {0, false});
    std::vector<std::pair<double, bool>> blackPlaying(piano.blackKeys.size(), {0, false});
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text songPlaying("", font, 20);
    songPlaying.setPosition(sf::Vector2f(window.getSize().x / 2, 10));


    while(window.isOpen()){
        //Play the next song
        std::ofstream outFile("songList.txt", std::ofstream::out | std::ofstream::trunc);

        if(outFile.is_open()){
            int temp = currSong;
            for(unsigned int i = 0; i < musicList.size(); i++, temp++){
                if(temp == musicList.size())
                    temp = 0;
                outFile << musicList[temp] << "\n";
            }
            outFile.flush();
            outFile.close();
        }

        std::string midiName = "outputMidi/" + musicList[currSong] + ".mid";
        std::string wavName = "outputWav/" + musicList[currSong] + ".wav";
        std::string message =  "Now Playing: " + musicList[currSong];
        songPlaying.setString(message);
        songPlaying.setPosition(sf::Vector2f((window.getSize().x -songPlaying.getGlobalBounds().width) / 2, 10));
        piano.notes.clear();
        piano.readMidi(midiName);
        sf::Music music;
        if (!music.openFromFile(wavName)) {
            return 1;
        }
        music.play();
        sf::Clock clock;
        for (int i = 0; i < (int)piano.notes.size(); i++)
        {
            window.draw(songPlaying);
            while(window.pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window.close();
                    exit = true;
                    break;
                }
                if(event.type == sf::Event::KeyPressed){
                    if(event.key.code == sf::Keyboard::F7){
                        exit = true;
                        break;
                    }
                    else if(event.key.code == sf::Keyboard::F5){
                        exit = true;
                        currSong -= 2;
                        break;
                    }
                    else if(event.key.code == sf::Keyboard::F6){
                        paused = !paused;
                        // clock.pause();
                        break;
                    }
                }
            }
            if(exit)
                break;
            
            drawNotes(window, piano, clock, whitePlaying, blackPlaying, songPlaying);


            if (clock.getElapsedTime().asSeconds() < piano.notes[i]->getStartTime()){
                i--;
                continue;
            }
            
            sf::sleep(sf::microseconds(150));

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
                    double timeToEnd = piano.notes[i]->getStartTime() + piano.notes[i]->getDurationInSeconds();
                    whitePlaying[key] = {timeToEnd, true};
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
                        piano.blackKeys[key].setFillColor(sf::Color(84, 148, 218));
                    else
                        piano.blackKeys[key].setFillColor(sf::Color(124,252,0));
                    double timeToEnd = piano.notes[i]->getStartTime() + piano.notes[i]->getDurationInSeconds();
                    blackPlaying[key] = {timeToEnd, true};
                    window.draw(piano.blackKeys[key]);
                }
                i++;
            }
            while(i < piano.notes.size() && piano.notes[i]->getStartTime() == piano.notes[i-1]->getStartTime());
            i--;

           drawNotes(window, piano, clock, whitePlaying, blackPlaying, songPlaying);

        }

        while (music.getStatus() == sf::Music::Playing && !exit) {};

        currSong++;
        if(currSong >= (int)musicList.size()) //if reaches end of loop, go back to start
            currSong = 0;
        else if(currSong < 0) //if rewinded on first song, go to end
            currSong = (int)musicList.size() - 1;
        exit = false;
        whitePlaying = std::vector<std::pair<double, bool>>(piano.whiteKeys.size(), {0, false});
        blackPlaying = std::vector<std::pair<double, bool>>(piano.blackKeys.size(), {0, false});
    }
}