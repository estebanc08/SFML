#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <queue>
#include <chrono>
#include <thread>
#include <cmath>
using namespace std;
#define TILE_SIZE 25

class scroller{
     struct tile{
        sf::RectangleShape tileRect;
        void drawTile(sf::RenderWindow& window);
        bool platform;
        bool block;
        tile();
    };
    sf::RectangleShape player;
    // bool gameLost;
    int blockX, blockY;
    sf::RectangleShape reset;
    sf::Text resetText;
    int width;
    int height;
    pair<int,int> randomNumber();
    vector<vector<tile>> tiles;
    void resetGame();
    public:
    sf::Vector2f movement;
    enum Direction {none = 0, left = 1, down = 2, right = 3, up = 4};
    Direction direction;
    void draw(sf::RenderWindow& window);
    scroller();
    scroller(int width, int height);
    void move();
    void scroll();
    void clickReset(sf::Vector2f mousePos);
    bool canJump();
};