#include "piano.hpp"
#include <mutex>
#include <sys/resource.h>
std::mutex notes_mutex;

int main(){
    sf::RenderWindow window(sf::VideoMode(WHITE_KEY_WIDTH*56+2*SIDE_PADDING, WHITE_KEY_HEIGHT+VERTICAL_PADDING), "piano");
    piano Piano;
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
    // thread readNotes([&](){
        for (const auto &entry : std::filesystem::directory_iterator(soundPath)) {
            if (entry.is_regular_file()) {
                sf::SoundBuffer buffer;
                buffer.loadFromFile(entry.path().string());
                string path = entry.path().string().substr(soundPath.size()+4);
                path = path.substr(0, path.size()-4);
                Piano.notes[path] = buffer;
            }
        }
    // }});
    // sf::SoundBuffer buffer;
    // buffer.loadFromFile("notesTemp/mf.G2.wav");
    // Piano.notes["G2"] = buffer;
    // sf::Sound sound(Piano.notes["G2"]);
    // sound.play();
    ifstream readFile("sheetMusic/liebestraum.txt");
    streampos pos = readFile.tellg();
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text loadingMessage("Waiting for piano sounds to load\nPlease be Patient", font, 24);
    loadingMessage.setPosition(window.getSize().x/2 - loadingMessage.getGlobalBounds().width/2, window.getSize().y/2 -loadingMessage.getGlobalBounds().height/2);
    loadingMessage.setFillColor(sf::Color::Black);
    // while(readingNotes.joinable()){
    //         this_thread::sleep_for(chrono::milliseconds(500));
    //         window.clear(sf::Color::White);
    //         window.draw(loadingMessage);
    //         window.display();
    // }
    // readingNotes.join();

    while(window.isOpen()){
        Piano.readKeyPressed(readFile, pos);
        Piano.draw(window);
    }
    readFile.close();
    return 0;
}
