#include "piano.hpp"
#include <sys/resource.h>
#include <queue>

// namespace fs = std::filesystem;
// void readFile(const fs::path& file_path, Piano& piano) {
//     cout << file_path << endl;

//     piano.readFile(file_path);
// }

// void readDirectory(const fs::path& dir_path, Piano& piano) {
//     for (const auto& entry : fs::directory_iterator(dir_path)) {
//         if (entry.is_directory()) {
//             // Recursively process subdirectories
//             readDirectory(entry.path(), piano);
//         } 
//         else if (entry.is_regular_file()) {
//             // Process regular files
//             readFile(entry.path(), piano);
//         }
//     }
// }

int main(){
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING), "piano");
    Piano piano;
    string soundPath = "notes";

    // readDirectory("xmlInputs", piano);

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
        ifstream readFile("sheetMusic/Op25No12.txt");
        piano.readKeysPressed(readFile);
        readFile.close();
    });
    read.join();
    readNotes.join();
    unsigned int currMeasure = 0;
    unsigned int start = 0;
    sf::Clock musicTimer;
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
