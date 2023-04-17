#ifndef PIANO_HPP
#define PIANO_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>
#include <fstream>
#include <regex>
#include <map>
#include <sstream>
#include <atomic>
#include "tinyxml2.h"
#include <unordered_map>
#define WHITE_KEY_WIDTH 23.5f
#define WHITE_KEY_HEIGHT 136.5f
#define BLACK_KEY_WIDTH 13.5f
#define BLACK_KEY_HEIGHT 85.5f
#define SIDE_PADDING 50
#define VERTICAL_PADDING 40
using namespace tinyxml2;

using namespace std;

class piano{
    bool musicDone = false;
    string currFileName;
    sf::RectangleShape whiteKey, blackKey;
    vector<sf::RectangleShape> whiteKeys;
    vector<sf::RectangleShape> blackKeys;
    vector<sf::Sound> sounds;
    public:
    unordered_map<string, sf::SoundBuffer> notes;
    map<int, vector<string>> notesToPlay;
    void readKeyPressed(ifstream& readFile, streampos& pos);
    void playKey(sf::Vector2f& mousePos);
    void playKey(string keyName);
    void readFile(string name);
    piano();
    void draw(sf::RenderWindow& window);
    atomic_bool quit = false;
};

#endif