#include "piano.hpp"
#include <sys/resource.h>
#include <queue>

namespace fs = std::filesystem;
void readFile(const fs::path& file_path, Piano& piano) {
    std::cout << file_path << std::endl;

    piano.readFile(file_path);
}

void readDirectory(const fs::path& dir_path, Piano& piano) {
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        if (entry.is_directory()) {
            // Recursively process subdirectories
            readDirectory(entry.path(), piano);
        } 
        else if (entry.is_regular_file()) {
            // Process regular files
            readFile(entry.path(), piano);
        }
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING), "piano");
    Piano piano;
    std::string soundPath = "notes";

    // readDirectory("xmlInputs", piano);

    std::thread readNotes([&](){
        for (const auto &entry : std::filesystem::directory_iterator(soundPath)) {
            if (entry.is_regular_file()) {
                sf::SoundBuffer buffer;
                buffer.loadFromFile(entry.path().string());
                std::string path = entry.path().string().substr(soundPath.size()+1);
                path = path.substr(0, path.size()-4);
                piano.notes[path] = buffer;
            }
        }
    });


    std::thread read([&](){
        std::ifstream readFile("sheetMusic/Op25No12.txt");
        piano.readKeysPressed(readFile);
        readFile.close();
    });
    read.join();
    readNotes.join();

    sf::RectangleShape line(sf::Vector2f(1, WHITE_KEY_HEIGHT));
    line.setFillColor(sf::Color::Black);
    sf::Event event;
    std::vector<std::unique_ptr<sf::Sound>> sounds;
    unsigned int start = 0;
    auto notes = piano.sheetNotes.begin();

    
    while(window.isOpen()){
         while(window.pollEvent(event)){
             if(event.type == sf::Event::Closed){
                window.close();
                break;
             }
         }
        if(notes != piano.sheetNotes.end()){
            window.clear();
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
            window.display();
            for(auto note = notes->begin(); note != notes->end(); note++){
                auto sound = std::make_unique<sf::Sound>(piano.notes[(*note)->getKey()]);
                sounds.emplace_back(std::move(sound));
                std::string keyStr = (*note)->getKey();
                int key = 0;
                if(keyStr == "rest")
                    continue;
                if(keyStr[1] != 'b'){
                    if(keyStr[0] == 'A' || keyStr[0] == 'B')
                        key = keyStr[0] - 'A' + 7*(keyStr[1]-'0');
                    else
                        key = keyStr[0] - 'A' + 7*(keyStr[1]-'0'-1);
                    piano.whiteKeys[key].setFillColor(sf::Color::Green);
                    window.draw(piano.whiteKeys[key]);
                }
                else{
                    if(keyStr[0] == 'B'){
                        key = 5*(keyStr[2]- '0');
                    }
                    else if(keyStr[0] == 'D'){
                        key = 5*(keyStr[2]- '0' -1) + 1;
                    }
                    else if(keyStr[0] == 'E'){
                        key = 5*(keyStr[2]- '0' -1) + 2;
                    }
                    else if(keyStr[0] == 'G'){
                        key = 5*(keyStr[2]- '0' -1) + 3;
                    }
                    else{
                        key = 5*(keyStr[2]-'0'-1)+4;
                    }
                    piano.blackKeys[key].setFillColor(sf::Color::Blue);
                    window.draw(piano.blackKeys[key]);
                }
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
            }
            for(; start < sounds.size(); start++){
                sounds[start]->play();
            }
            window.display();
            sf::sleep(sf::milliseconds(noteHold));
            sounds.erase(std::remove_if(sounds.begin(), sounds.end(), [](const std::unique_ptr<sf::Sound>& s){ return s->getStatus() == sf::Sound::Status::Stopped; }), sounds.end());
            start = sounds.size();
            notes++;
        }
    }
    return 0;
}