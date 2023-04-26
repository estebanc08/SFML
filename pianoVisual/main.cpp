#include "piano.hpp"

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

    sf::RectangleShape line(sf::Vector2f(1, WHITE_KEY_HEIGHT));
    line.setFillColor(sf::Color::Black);
    sf::Event event;
    bool exit = false;
    bool paused = false;
    std::vector<std::pair<double, bool>> whitePlaying(piano.whiteKeys.size(), {0, false});
    std::vector<std::pair<double, bool>> blackPlaying(piano.blackKeys.size(), {0, false});


    while(window.isOpen()){
        //Play the next song
        std::ofstream outFile("songList.txt", std::ofstream::out | std::ofstream::trunc);
        std::mutex lockFile;

        std::unique_lock<std::mutex> lock(lockFile);
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
        std::cout << "Now Playing: " << musicList[currSong] << "\n";
        piano.notes.clear();
        piano.readMidi(midiName);
        sf::Music music;
        if (!music.openFromFile(wavName)) {
            return 1;
        }
        music.play();
        sf::Clock clock;
        for (unsigned int i = 0; i < piano.notes.size(); i++)
        {
            window.clear();
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
            
            while (clock.getElapsedTime().asSeconds() < piano.notes[i]->getStartTime())
                sf::sleep(sf::microseconds(100));
            for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
                if(clock.getElapsedTime().asSeconds() > whitePlaying[i].first && whitePlaying[i].second || whitePlaying[i].second == false)
                    piano.whiteKeys[i].setFillColor(sf::Color(255,255,245));
                window.draw(piano.whiteKeys[i]);
            }
            for(int i = 1; i < 56; i++){
                line.setPosition(sf::Vector2f(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING));
                window.draw(line);
            }
            for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
                if(clock.getElapsedTime().asSeconds() > blackPlaying[i].first && blackPlaying[i].second || blackPlaying[i].second == false)
                    piano.blackKeys[i].setFillColor(sf::Color::Black);
                window.draw(piano.blackKeys[i]);
            }
            window.display();
            sf::sleep(sf::microseconds(400));
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
                        piano.blackKeys[key].setFillColor(sf::Color::Blue);
                    else
                        piano.blackKeys[key].setFillColor(sf::Color::Green);
                    double timeToEnd = piano.notes[i]->getStartTime() + piano.notes[i]->getDurationInSeconds();
                    blackPlaying[key] = {timeToEnd, true};
                    window.draw(piano.blackKeys[key]);
                }
                i++;
            }
            while(i < piano.notes.size() && piano.notes[i]->getStartTime() == piano.notes[i-1]->getStartTime());
            i--;
            for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
                window.draw(piano.whiteKeys[i]);
            }
            for(int i = 1; i < 56; i++){
                line.setPosition(sf::Vector2f(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING));
                window.draw(line);
            }
            for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
                window.draw(piano.blackKeys[i]);
            }
            window.display();
        }
        while (music.getStatus() == sf::Music::Playing && !exit) {};

        window.clear();
        for(unsigned int i = 0; i < piano.whiteKeys.size(); i++){
            piano.whiteKeys[i].setFillColor(sf::Color(255,255,245));
            window.draw(piano.whiteKeys[i]);
        }
        for(int i = 1; i < 56; i++){
            line.setPosition(sf::Vector2f(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING));
            window.draw(line);
        }
        for(unsigned int i = 0; i < piano.blackKeys.size(); i++){
            piano.blackKeys[i].setFillColor(sf::Color::Black);
            window.draw(piano.blackKeys[i]);
        }
        window.display();

        currSong++;
        if(currSong >= (int)musicList.size()) //if reaches end of loop, go back to start
            currSong = 0;
        else if(currSong < 0) //if rewinded on first song, go to end
            currSong = (int)musicList.size() - 1;
        exit = false;
    }
}