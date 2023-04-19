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
#define WHITE_KEY_WIDTH 23.5f
#define WHITE_KEY_HEIGHT 136.5f
#define BLACK_KEY_WIDTH 13.5f
#define BLACK_KEY_HEIGHT 85.5f
#define SIDE_PADDING 50
#define VERTICAL_PADDING 40
using namespace std;
static int noteHold = 120;

struct Piano{
    struct Note{
        int length;
        string key;
        int velocity;
        sf::Clock soundTimer;
        Note(string _key, int _length,  int _velocity = 0) : length(_length), key(_key), velocity(_velocity){};
    };
    // struct Measure{
    //     map<int, vector<Note>> notesToPlay;
    // };
    int currNote = 0;
    bool musicDone = false;
    string currFileName;
    sf::RectangleShape whiteKey, blackKey;
    vector<sf::RectangleShape> whiteKeys;
    vector<sf::RectangleShape> blackKeys;
    unsigned int measure = 0;
    
    public:
    vector<sf::Sound> sounds;
    unordered_map<string, sf::SoundBuffer> notes;
    void readKeysPressed(ifstream& readFile);
    void playKey(sf::Vector2f& mousePos);
    void playKey(string keyName);
    void readFile(string name);
    void playMusic(unsigned int currMeasure, unsigned int &start);
    Piano();
    vector<map<int, vector<Note>>> sheetMusic;
    void draw(sf::RenderWindow& window, unsigned int currMeasure);
    atomic_bool quit = false;
    
};

#endif