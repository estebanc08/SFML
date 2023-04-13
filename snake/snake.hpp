#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <queue>
#include <chrono>
#include <thread>
using namespace std;
#define TILE_SIZE 25


class snake{
    struct tile{
        sf::RectangleShape tileRect;
        bool body;
        bool apple;
        void drawTile(sf::RenderWindow& window);
        tile();
        // tile(bool isBody, bool isApple);
    };
    // bool gameLost;
    int headPosX, headPosY;
    queue<pair<int,int>> snakeBody;
    sf::RectangleShape reset;
    sf::Text resetText;
    int size;
    int width;
    int height;
    pair<int,int> randomNumber();
    void eatApple();
    void randApple();
    vector<vector<tile>> tiles;
    public:
    enum Direction {none = 0, left = 1, down = 2, right = 3, up = 4};
    Direction direction;
    void draw(sf::RenderWindow& window);
    snake();
    snake(int width, int height);
    void move();
    void clickReset(sf::Vector2f mousePos);

};