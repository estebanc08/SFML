#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <filesystem>
#include <random>
#include <algorithm>
#include <set>
using namespace std;

class sudoku{
    struct Tile{
        vector<bool> pencilMarks;
        int xPos, yPos;
        bool valid;
        bool clickable;
        int num;
        sf::RectangleShape tile;
        sf::Text text;
        void drawTile(sf::RenderWindow& window);
        Tile();
    };
    vector<vector<Tile>> tiles;
    vector<int> nums;
    unordered_map<string, sf::RectangleShape> buttons;
    vector<sf::Text> buttonLabels;
    unordered_map<int, vector<vector<Tile>>> allBoards;
    int currBoard = 0;
    void readAllBoards(const string path);
    int windowWidth, windowHeight;
    pair<int,int> currCell;
    bool solved = false;
    void checkSolved();
    bool valid = true;
    bool checkValid(int row, int col, int num);

    public:
    bool leftClick;
    void deleteNum();
    void reset();
    sudoku();
    sudoku(int width, int height);
    void addNum(int num);
    bool isSolved();
    void drawBoard(sf::RenderWindow& window);
    void setCurrCell(sf::Vector2f mousePos);
    bool solveSudoku(int i, int j);
};