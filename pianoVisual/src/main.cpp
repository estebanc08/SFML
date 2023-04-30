#include "main.hpp"


int main(int argc, char const *argv[]){
    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(videoMode, "piano");
    Piano piano(window.getSize().x, window.getSize().y);

    //start the song from current position
    std::ifstream musicIn("songList.txt");
    std::vector<std::string> musicList;
    std::string name;

    while(getline(musicIn, name))
        musicList.push_back(name);
    int currSong = 0;

    sf::Event event;
    bool exit = false;
    bool paused = false;
    std::vector<double> whitePlaying(piano.whiteKeys.size(), 0.f);
    whitePlaying.shrink_to_fit();
    std::vector<double> blackPlaying(piano.blackKeys.size(), 0.f);
    blackPlaying.shrink_to_fit();
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
        start = 0; //reset note counter
        piano.notes.shrink_to_fit();


        sf::Music music;
        if (!music.openFromFile(wavName)) {
            return 1;
        }
        music.play();


        sf::myClock clock;
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
                        if(paused){
                            clock.stop();
                            music.pause();
                        }
                        else{
                            clock.start();
                            music.play();
                        }
                        break;
                    }
                }
                else if(event.type == sf::Event::MouseButtonPressed){
                    if(event.mouseButton.button == sf::Mouse::Left){
                        paused = !paused; 
                        if(paused){
                            clock.stop();
                            music.pause();
                        }
                        else{
                            clock.start();
                            music.play();
                        }
                        break;
                    }
                }
            }
            if(exit)
                break;
            
            drawNotes(window, piano, clock, whitePlaying, blackPlaying, songPlaying);

            if (clock.getElapsedTime().asSeconds() < piano.notes[i]->getStartTime() || paused){
                i--; //dont play note yet, go back to current note and check again
                continue;
            }
            do{
                int key = 0;
                if(piano.notes[i]->getKey()[1] != 'b'){
                    key = whiteKeyIndex(piano.notes[i]->getKey());
                    if(key < piano.whiteKeys.size()/2)
                        piano.whiteKeys[key].setFillColor(sf::Color(84, 148, 218)); //LHS piano
                    else
                        piano.whiteKeys[key].setFillColor(sf::Color(124,252,0)); //RHS piano
                    double timeToEnd = piano.notes[i]->getStartTime() + piano.notes[i]->getDurationInSeconds() - 2.f/100; //to make repeated notes have small gap between them
                    if(piano.notes[i]->getDurationInSeconds() < 20.f/1000){
                        timeToEnd = piano.notes[i]->getStartTime() + 20.f/1000; //if note too short to see
                    }
                    whitePlaying[key] = timeToEnd;
                    window.draw(piano.whiteKeys[key]);
                }
                else{
                    key = blackKeyIndex(piano.notes[i]->getKey());
                    if(key < piano.blackKeys.size() / 2)
                        piano.blackKeys[key].setFillColor(sf::Color(59, 107, 153));
                    else
                        piano.blackKeys[key].setFillColor(sf::Color(89,176,0));
                    double timeToEnd = piano.notes[i]->getStartTime() + piano.notes[i]->getDurationInSeconds() - 2.f/100;
                    if(piano.notes[i]->getDurationInSeconds() < 20.f/1000){
                        timeToEnd = piano.notes[i]->getStartTime() + 20.f/1000;
                    }
                    blackPlaying[key] = timeToEnd;
                    window.draw(piano.blackKeys[key]);
                }
                i++;

            }
            while(i < piano.notes.size() && piano.notes[i]->getStartTime() == piano.notes[i-1]->getStartTime());
            i--; //for loop will increment one too many without this and skip notes
            
            drawNotes(window, piano, clock, whitePlaying, blackPlaying, songPlaying); //need this to play the last notes or else will exit loop
        }

        while (music.getStatus() == sf::Music::Playing && !exit) {
            drawNotes(window, piano, clock, whitePlaying, blackPlaying, songPlaying);
            sf::sleep(sf::microseconds(100));
        };

        currSong++;
        if(currSong >= (int)musicList.size()) //if reaches end of loop, go back to start
            currSong = 0;
        else if(currSong < 0) //if rewinded on first song, go to end
            currSong = (int)musicList.size() - 1;
        
        exit = false;
        whitePlaying = std::vector<double>(piano.whiteKeys.size(), 0.f);
        whitePlaying.shrink_to_fit();
        blackPlaying = std::vector<double>(piano.blackKeys.size(), 0.f);
        blackPlaying.shrink_to_fit();
    }
    
    return 0;
}


inline void drawNotes(sf::RenderWindow& window, Piano& piano, sf::myClock& clock, std::vector<double>& whitePlaying, std::vector<double>& blackPlaying, sf::Text& songPlaying){
    window.clear();
    
    double currTime = clock.getElapsedTime().asSeconds();
    double timeToShow = currTime + SCREEN_TIME;
    float screenPercent = (window.getSize().y - piano.whiteKeys[0].getGlobalBounds().height) / SCREEN_TIME;
    for(unsigned int index = start; index < piano.notes.size(); index++){
        if(piano.notes[index]->getStartTime() < currTime - SCREEN_TIME){
            start++; //note offically out of scope
            continue;
        }
        if(piano.notes[index]->getStartTime() < timeToShow){
            int key = 0;
            bool whiteKey = true;
            sf::RoundedRectangleShape note;
            note.setCornersRadius(3.f);
            note.setCornerPointCount(2);
            float width = 0;
            if(piano.notes[index]->getKey()[1] != 'b'){

                key = whiteKeyIndex(piano.notes[index]->getKey());
                width = piano.whiteKeys[0].getGlobalBounds().width - 6;
                if(key < piano.whiteKeys.size()/2)
                    note.setFillColor(sf::Color(84, 148, 218)); //LHS piano
                else
                    note.setFillColor(sf::Color(124,252,0)); //RHS piano
            }
            else{
                key = blackKeyIndex(piano.notes[index]->getKey());
                if(key < piano.blackKeys.size()/2)
                    note.setFillColor(sf::Color(59, 107, 153)); //LHS piano
                else
                    note.setFillColor(sf::Color(89,176,0)); //RHS piano
                whiteKey = false;
                width = piano.blackKeys[0].getGlobalBounds().width;
            }
            float noteHeight = piano.notes[index]->getStartTime() - currTime + piano.notes[index]->getDurationInSeconds() * screenPercent;
            float x = whiteKey ? piano.whiteKeys[key].getPosition().x + 3: piano.blackKeys[key].getPosition().x;
            float y = window.getSize().y - piano.whiteKeys[0].getGlobalBounds().height  - (piano.notes[index]->getStartTime() - currTime) * screenPercent - noteHeight - 10;
            noteHeight -= 15; //want there to be more separation between notes being played back to back
            if(noteHeight < 10)
                noteHeight = 10;
            note.setSize(sf::Vector2f(width, noteHeight));
            note.setPosition(x, y);
            window.draw(note);
        }
        else
            break;
    }

    sf::RectangleShape blackBar(sf::Vector2f(window.getSize().x, 10));
    blackBar.setFillColor(sf::Color::Black);
    blackBar.setPosition(0, window.getSize().y - 10);
    window.draw(blackBar);
    window.draw(songPlaying);

    sf::RectangleShape redLine(sf::Vector2f(window.getSize().x - 2 * SIDE_PADDING,  1));
    redLine.setPosition(SIDE_PADDING, window.getSize().y - piano.whiteKeys[0].getGlobalBounds().height  - 9);
    redLine.setFillColor(sf::Color::Red);

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text buttons("Press F5 to go back", font, 20);
    buttons.setPosition(10,10);
    window.draw(buttons);
    buttons.setString("Press F7 to skip");
    buttons.setPosition(window.getSize().x - buttons.getGlobalBounds().width - 10, 10);
    window.draw(buttons);
    for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
        float posX = piano.whiteKeys[i].getPosition().x;
        if(clock.getElapsedTime().asSeconds() >= whitePlaying[i]){
            piano.whiteKeys[i].setFillColor(sf::Color(255,255,245));
            window.draw(piano.whiteKeys[i]);
        }
        else{
            sf::RectangleShape pressed(piano.whiteKeys[i]);
            pressed.setSize(sf::Vector2f(pressed.getSize().x, pressed.getSize().y + 2.5)); //make it look like pressed by slightly extending
            window.draw(pressed);
        }
    }

    window.draw(redLine); //draws behind black key

    for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
        if(clock.getElapsedTime().asSeconds() >= blackPlaying[i])
            piano.blackKeys[i].setFillColor(sf::Color::Black);
        window.draw(piano.blackKeys[i]);
    }

    window.display();

}

inline int blackKeyIndex(std::string key){
    if(key[0] == 'B'){
        return (5*(key[2]- '0'));
    }
    else if(key[0] == 'D'){
        return (5*(key[2]- '0' -1) + 1);
    }
    else if(key[0] == 'E'){
        return (5*(key[2]- '0' -1) + 2);
    }
    else if(key[0] == 'G'){
       return (5*(key[2]- '0' -1) + 3);
    }
    else{
        return (5*(key[2]-'0'-1) + 4);
    }
}   

inline int whiteKeyIndex(std::string key){
    if(key[0] == 'A' || key[0] == 'B')
        return (key[0] - 'A' + 7*(key[1]-'0'));
    else
        return (key[0] - 'A' + 7*(key[1]-'0'-1));
}