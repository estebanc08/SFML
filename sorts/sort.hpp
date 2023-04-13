#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <queue>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <cmath>
#include "allSorts.hpp"
using namespace std;
#define TILE_WIDTH 4

class sorts{
     struct tile{
        sf::RectangleShape tileRect;
        void drawTile(sf::RenderWindow& window);
        tile();
        tile(int num);
        int num;
    };
    void shuffle();
    void loadButtons();
    sf::Texture backgroundPic;
    sf::RectangleShape back;
    sf::Color babyBlue;
    unordered_map<string, vector<int>> allSorts;
    vector<int> original;
    vector<int> nums;
    unordered_map<string, pair<sf::RectangleShape, sf::Text>> buttons;
    int i, j;
    int toSwap;
    int randomNumber();
    int height, width;
    string sortType;
    public:
    void changeSort(string type);
    sorts();
    sorts(int width, int height);
    void update(sf::RenderWindow& window);
    void clickType(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
};