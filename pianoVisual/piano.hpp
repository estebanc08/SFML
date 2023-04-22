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
#include <algorithm>
#include <atomic>
#include "tinyxml2.h"
#include <unordered_map>
#include <queue>
#include "noteClass/includes.hpp"
#define WHITE_KEY_WIDTH 23.5f
#define WHITE_KEY_HEIGHT 136.5f
#define BLACK_KEY_WIDTH 13.5f
#define BLACK_KEY_HEIGHT 85.5f
#define SIDE_PADDING 50
#define VERTICAL_PADDING 40
static int noteHold = 120;

struct Piano{
    // struct Measure{
    //     map<int, vector<Note>> notesToPlay;
    // };
    int currNote = 0;
    bool musicDone = false;
    std::string currFileName;
    sf::RectangleShape whiteKey, blackKey;
    std::vector<sf::RectangleShape> whiteKeys;
    std::vector<sf::RectangleShape> blackKeys;
    unsigned int measure = 0;
    
    public:
    std::vector<std::vector<Note*>> sheetNotes;
    std::vector<sf::Sound> sounds;
    std::unordered_map<std::string, sf::SoundBuffer> notes;
    void readKeysPressed(std::ifstream& readFile);
    void playKey(sf::Vector2f& mousePos);
    void playKey(std::string keyName);
    void readFile(std::string name);
    Piano();
    std::vector<std::map<int, std::vector<Note*>>> sheetMusic;
    std::atomic_bool quit = false; 
};

#endif