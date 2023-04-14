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
#include <algorithm>
#include <set>
#define PI 3.14159265359
using namespace std;
static int TILE_WIDTH = 30;


class tetris{
    struct tile{
        sf::RectangleShape rect;
        sf::Color color;
        void clear();
        tile();
        tile(sf::Color color);
        bool isShape = false;
    };
    struct shape{
        sf::Color color;
        vector<sf::RectangleShape> blocks;
        void rotateShape();
        shape& operator=(const shape& RHS){
            this->blocks = RHS.blocks;
            return *this;
        }
        bool operator==(const shape& RHS) const {
            if (blocks.size() != RHS.blocks.size()) {
                return false;
            }
            for (int i = 0; i < blocks.size(); i++) {
                if (blocks[i].getPosition() != RHS.blocks[i].getPosition()) {
                    return false;
                }
            }
            return true;
        }
        bool operator!=(const shape& RHS) const {
            return !(*this == RHS);
        }
        void draw(sf::RenderWindow& window);
    };
    vector<vector<tile>> board;
    void clearRow();
    void loadShapes();
    sf::Vector2f pivot;
    unordered_map<string, shape> shapes;
    int randomNumber();
    shape randomShape(int min, int max);
    int height, width;
    void fall();
    string shapeType;
    shape currShape;
    bool nextShape;
    bool collisionCheck();
    bool validRotate(shape& shape);
    bool deleting = false;
    bool gameOver = false;
    bool checkGameOver();

    public:
    void restart();
    void move();
    enum Direction {none = 0, down = 1, left = 2, right = 3, up = 4};
    Direction direction;
    int size;
    tetris();
    tetris(int width, int height);
    void update();
    void clickType(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
};