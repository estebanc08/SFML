#include "piano.hpp"
// #include "soundStream.hpp"
#include <sys/resource.h>
#include <queue>
int main(){
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING), "piano");
    Piano piano;
    string soundPath = "notes";

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

    thread readNotes([&](){
        for (const auto &entry : std::filesystem::directory_iterator(soundPath)) {
            if (entry.is_regular_file()) {
                sf::SoundBuffer buffer;
                buffer.loadFromFile(entry.path().string());
                string path = entry.path().string().substr(soundPath.size()+1);
                path = path.substr(0, path.size()-4);
                piano.notes[path] = buffer;
            }
        }
    });
    thread read([&](){
        ifstream readFile("sheetMusic/norma.txt");
        piano.readKeysPressed(readFile);
        readFile.close();
    });
    read.join();
    readNotes.join();
    unsigned int currMeasure = 0;
    unsigned int start = 0;

    while(window.isOpen() && currMeasure < piano.sheetMusic.size()){
        thread music(&Piano::playMusic, &piano, currMeasure, std::ref(start));
        thread draw(&Piano::draw, &piano, std::ref(window), currMeasure);
        music.join();
        draw.join();
        currMeasure++;
        if(piano.quit)
            window.close();
    }
    return 0;
}
