#ifndef TETRIS_HPP
#define TETRIS_HPP
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
#define MOVING_SPEED 15.f
using namespace std;
static int TILE_WIDTH = 30;
static int PADDING = TILE_WIDTH * 8;

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
        sf::Vector2f pivot;
        sf::Color color;
        int offset = 0;
        vector<sf::RectangleShape> blocks;
        void rotateShape();
        string type;
        shape(){};
        shape(const shape& RHS)
        {
            this->type = RHS.type;
            this->offset = RHS.offset;
            this->blocks = RHS.blocks;
            this->pivot = RHS.pivot;
        }
        shape& operator=(const shape& RHS){
            this->type = RHS.type;
            this->offset = RHS.offset;
            this->blocks = RHS.blocks;
            this->pivot = RHS.pivot;
            return *this;
        }
        bool operator==(const shape& RHS) const {
            if (blocks.size() != RHS.blocks.size()) {
                return false;
            }
            for (unsigned int i = 0; i < blocks.size(); i++) {
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
    bool shadowReady = false;
    float FALLING_SPEED = 20.f;
    int linesCleared = 0;
    unsigned int points = 0;
    sf::RectangleShape sidebar;
    vector<vector<tile>> board;
    void clearRow();
    void loadShapes();
    sf::Vector2f pivot;
    unordered_map<string, shape> shapes;
    int randomNumber();
    shape randomShape();
    int height, width;
    void fall();
    shape currShape;
    bool nextShape;
    bool collisionCheck();
    bool validPosition(shape& shape);
    bool gameOver = false;
    bool checkGameOver();
    queue<shape> shapeQueue;
    std::vector<std::vector<std::pair<int, int>>> standardWallkick;
    std::vector<std::vector<std::pair<int, int>>> reverseWallkick;
    shape makeShadow();
    shape shadow;

    public:
    bool pause = true;
    set<int> rowsDelete;
    void animateRowClear(sf::RenderWindow& window);
    bool deleting = false;
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
    float getFallingSpeed(){return FALLING_SPEED;};
};

#endif