#include "piano.hpp"
#include <mutex>
#include <sys/resource.h>
std::mutex notes_mutex;

int main(){
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING), "piano");
    Piano piano;
    string soundPath = "notesTemp";

    // Piano.readFile("liebestraum.xml");
    // string songsPath = "sheetMusic";    
    // unordered_map<string, ifstream&> songs;
    // thread readSongs([&](){
    //     for (const auto &entry : std::filesystem::directory_iterator(soundPath)) {
    //         if (entry.is_regular_file()) {
    //             std::cout << entry.path() << std::endl;
    //         }
    //     }
    // });

    // sf::Clock timer;
    // sf::SoundBuffer buffer;
    thread readNotes([&](){
        for (const auto &entry : std::filesystem::directory_iterator(soundPath)) {
            if (entry.is_regular_file()) {
                sf::SoundBuffer buffer;
                buffer.loadFromFile(entry.path().string());
                string path = entry.path().string().substr(soundPath.size()+4);
                path = path.substr(0, path.size()-4);
                piano.notes[path] = buffer;
            }
        }
    });
    ifstream readFile("sheetMusic/balladeNo2.txt");
    piano.readKeysPressed(readFile);
    readNotes.join();
    // sf::Font font;

    
    unsigned int currMeasure = 0;
    unsigned int start = 0;

    //  thread preload([&](){
        //     if(currMeasure + 1 != piano.sheetMusic.size()){
        //         for(auto measure : piano.sheetMusic[currMeasure+1]){
        //             for(auto note : measure.second){
        //                 sf::Sound sound(piano.notes[note.key]);
        //                 // sound.setLoop(true);
        //                 sound.setPlayingOffset(sf::milliseconds(1250));
        //                 piano.sounds.push_back(sound);
        //             }
        //         }
        //     }
        // });
    while(window.isOpen() && currMeasure < piano.sheetMusic.size()){
        thread music(&Piano::playMusic, &piano, currMeasure, std::ref(start));
        thread draw(&Piano::draw, &piano, std::ref(window), currMeasure);
        music.join();
        draw.join();
        currMeasure++;
    }
    readFile.close();
    return 0;
}
