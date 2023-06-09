#ifndef PIANO_HPP
#define PIANO_HPP

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Note.hpp"
#include "MidiFile.h"
#include "MidiEvent.h"
#include "MidiEventList.h"
#include "Options.h"
#define BLACK_KEY_WIDTH 13.5f
#define WHITE_KEY_WIDTH 23.5f
#define BLACK_KEY_HEIGHT 85.5f
#define WHITE_KEY_HEIGHT 136.5f
#define SIDE_PADDING 50
#define VERTICAL_PADDING 40

class Piano{
    bool musicDone = false;
    std::string currFileName;
    sf::RectangleShape whiteKey, blackKey;
    
    public:
    std::vector<sf::RectangleShape> whiteKeys;
    std::vector<sf::RectangleShape> blackKeys;
    std::map<unsigned int, float> tempos;
    std::vector<Note*> notes;
    Piano();
    Piano(unsigned int width, unsigned int height);
    void readMidi(const std::string path);

};

#endif