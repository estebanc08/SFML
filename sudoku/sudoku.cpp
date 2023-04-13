#include "sudoku.hpp"
namespace fs = std::filesystem;
sf::Font font;

sudoku::sudoku(){
    font.loadFromFile("arial.ttf");
    windowHeight = 0;
    windowWidth = 0;
    reset();
}

sudoku::sudoku(int width, int height){
    font.loadFromFile("arial.ttf");
    windowWidth = width;
    windowHeight = height;
    currCell.first = -1;
    currCell.second = -1;
    readAllBoards("grids.txt");
    reset();
}

void sudoku::addNum(int num){
    if(solved)
        return;
    int i = currCell.first;
    int j = currCell.second;
    if(!tiles[i][j].clickable)
        return;
    if(leftClick){
        tiles[i][j].num = num;
        for(int row = 0; row < 9; row++)
            for(int col = 0; col < 9; col++)
                if(tiles[row][col].clickable && tiles[row][col].num > 0)
                    tiles[row][col].valid = checkValid(row, col, num);
        nums[num-1]++;
    }
    else{
        tiles[i][j].pencilMarks[num-1] = !tiles[i][j].pencilMarks[num-1];
        return;
    }
    checkSolved();
}

void sudoku::deleteNum(){
    if(solved)
        return;
    int i = currCell.first;
    int j = currCell.second;
    if(tiles[i][j].clickable){
        nums[tiles[i][j].num-1]--;
        tiles[i][j].num = -1;
        tiles[i][j].valid = true;
         for(int row = 0; row < 9; row++){
            for(int col = 0; col < 9; col++){
                if(tiles[row][col].clickable && tiles[row][col].num > 0){
                    tiles[row][col].valid = checkValid(row, col, tiles[row][col].num);
                }
            }
         }
    }
}

void sudoku::setCurrCell(sf::Vector2f mousePos){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            tiles[i][j].tile.setFillColor(sf::Color::White);
            if(tiles[i][j].tile.getGlobalBounds().contains(mousePos)){
                currCell.first = i;
                currCell.second = j;
                return;
            }
        }
    }
    for(auto button : buttons){
        if(button.second.getGlobalBounds().contains(mousePos)){
            if(button.first == "reset")
                reset();
            else if(button.first == "solve"){
                solveSudoku(0,0);
                fill(nums.begin(), nums.end(), 9);
                solved = true;
            }
        }
    }

}
void sudoku::checkSolved(){
    int used1[9][9] = {0}, used2[9][9] = {0}, used3[9][9] = {0};
    for(int i = 0; i < 9; ++ i)
    {
        for(int j = 0; j < 9; ++ j)
        {
            if(tiles[i][j].num > 0)
            {
                int num = tiles[i][j].num - 1, k = i / 3 * 3 + j / 3;
                if(used1[i][num] || used2[j][num] || used3[k][num])
                {
                    solved = false;
                    return;
                }
                used1[i][num] = used2[j][num] = used3[k][num] = 1;
            }
            else{
                solved = false;
                return;
            }
        }
    }
    solved = true;
}

void sudoku::drawBoard(sf::RenderWindow& window){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(currCell.first >= 0 && tiles[i][j].num > 0 && tiles[currCell.first][currCell.second].num == tiles[i][j].num){
                tiles[i][j].tile.setFillColor(sf::Color(128, 128, 128, 150));
            }
            else if(currCell.first >= 0 && (i/3 == currCell.first/3 && j/3 == currCell.second/3) || (i == currCell.first || j == currCell.second)){
                if(i == currCell.first && j == currCell.second)
                   tiles[i][j].tile.setFillColor(sf::Color(173, 216, 230, 200));
                else
                    tiles[i][j].tile.setFillColor(sf::Color(128, 128, 128, 60));
            }
            else{
                tiles[i][j].tile.setFillColor(sf::Color::White);
            }
            tiles[i][j].drawTile(window);
        }
    }
    for (int i = 0; i <= 9; ++i) {
        sf::RectangleShape line(sf::Vector2f(50 * 9, i % 3 == 0 ? 4 : 2.f));
        line.setFillColor(sf::Color::Black);
        line.setPosition(sf::Vector2f((windowWidth / 9 / 2), 50 * i + (windowHeight / 9 / 2)));
        window.draw(line);

        sf::RectangleShape line2(sf::Vector2f(i % 3 == 0 ? 4 : 2.f, 50 * 9));
        line2.setFillColor(sf::Color::Black);
        line2.setPosition(sf::Vector2f(50 * i + (windowWidth / 9 / 2), (windowHeight / 9 / 2)));
        window.draw(line2);
    }

   for(auto button : buttons)
        window.draw(button.second);
    for(auto text : buttonLabels)
        window.draw(text);
    for(int i = 0; i < nums.size(); i++){
        if(nums[i] < 9){
            sf::Text numLeft(to_string(i+1), font, 24);
            numLeft.setPosition(50 + i * 50, windowHeight - 50);
            numLeft.setFillColor(sf::Color::Blue);
            window.draw(numLeft);
        }
    }
    if(solved){
         sf::Text gameOver("Game Won! Reset to start a new map", font, 24);
        gameOver.setPosition(50, windowHeight - 50);
        gameOver.setFillColor(sf::Color::Black);
        window.draw(gameOver);
    }
}

bool sudoku::isSolved(){
    return solved;
}

void sudoku::reset(){
    currCell.first = -1;
    currCell.second = -1;
    tiles.resize(9, vector<Tile>(9));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, allBoards.size()-1);
    int currBoard = dis(gen);
    tiles = allBoards[currBoard];
    nums.clear();
    nums.resize(9,0);
    for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                int x = i * 50 + (windowWidth / 9 / 2);
                int y = j * 50 + (windowHeight / 9 / 2);
                sf::RectangleShape tile(sf::Vector2f(50, 50));
                tile.setPosition(sf::Vector2f(x, y));
                tile.setFillColor(sf::Color::White);
                tile.setOutlineColor(sf::Color::Black);
                if(tiles[i][j].num > 0){
                    tiles[i][j].clickable = false;
                    nums[tiles[i][j].num-1]++;
                }
                tiles[i][j].tile = tile;
                tiles[i][j].text.setPosition(x + 17.f, y+12.5);
                tiles[i][j].text.setCharacterSize(25);
                tiles[i][j].text.setFont(font);
            }
        }
    solved = false;
}

bool sudoku::checkValid(int row, int col, int num){
     for (int j = 0; j < 9; j++) {
        if (tiles[row][j].num == num && j != col) {
            return false;
        }
        if (tiles[j][col].num == num && j != row) {
            return false;
        }
    }

    int box_row = row - row % 3;
    int box_col = col - col % 3;
    for (int i = box_row; i < box_row + 3; i++) {
        for (int j = box_col; j < box_col + 3; j++) {
            if (tiles[i][j].num == num && (i != row && j != col)) {
                return false;
            }
        }
    }
    return true;
}

bool sudoku::solveSudoku(int i, int j){
    if(i==9) 
        return true;
    if(j==9) 
        return solveSudoku(i+1, 0);
    if(tiles[i][j].num > 0) 
        return solveSudoku(i, j+1);

    for(int num = 1; num <= 9; num++)
    {
        if(checkValid(i, j, num))
        {
            tiles[i][j].num = num;
            if(solveSudoku(i, j+1))
                 return true;
            tiles[i][j].num = 0;
        }
    }
        
    return false;

}
void sudoku::readAllBoards(const string path){
    ifstream inFile(path);
    string currLine;
    while(getline(inFile, currLine)){
        stringstream stream(currLine);
        string num = "";
        stream >> num >> num;
        vector<vector<Tile>> temp(9, vector<Tile>(9));
        for(int i = 0; i < 9; i++){
            getline(inFile, currLine);
            for(int j = 0; j < 9; j++){
                temp[i][j].num = currLine[j] - '0';
            }
        }
        allBoards[stoi(num)] = temp; 
    }
    sf::RectangleShape reset(sf::Vector2f(100.0f, 50.0f));
    reset.setPosition(windowWidth - 125, 50);
    reset.setFillColor(sf::Color(122,122,122, 100));

    sf::Text resetText("Reset", font, 24);
    resetText.setFillColor(sf::Color::Black);
    float textWidth = resetText.getGlobalBounds().width;
    textWidth = (100.f - textWidth)/2;

    float textHeight = resetText.getGlobalBounds().height;
    textHeight = (40.f - textHeight)/2;
    resetText.setPosition(reset.getPosition().x+textWidth, reset.getPosition().y+textHeight);
    buttons["reset"] = reset;
    buttonLabels.push_back(resetText);

     sf::RectangleShape solve(sf::Vector2f(100.0f, 50.0f));
    solve.setPosition(windowWidth - 125, 150);
    solve.setFillColor(sf::Color(122,122,122, 100));

    sf::Text solveText("Solve", font, 24);
    solveText.setFillColor(sf::Color::Black);
    textWidth = solveText.getGlobalBounds().width;
    textWidth = (100.f - textWidth)/2;

    textHeight = solveText.getGlobalBounds().height;
    textHeight = (40.f - textHeight)/2;
    solveText.setPosition(solve.getPosition().x+textWidth, solve.getPosition().y+textHeight);
    buttons["solve"] = solve;
    buttonLabels.push_back(solveText);
}

sudoku::Tile::Tile(){
    valid = true;
    clickable = true;
    num = -1;
    pencilMarks.resize(9,false);
    sf::Text num("", font, 25);
}
void sudoku::Tile::drawTile(sf::RenderWindow& window){
    string number = "";
    if(num > 0){
        number = to_string(num);
    }
    text.setString(number);
    text.setFillColor(sf::Color::Black);
    if(clickable && num > 0)
        text.setFillColor(sf::Color::Blue);
    if(clickable && !valid)
        tile.setFillColor(sf::Color(255,0,0, 80));
    window.draw(tile);
    window.draw(text);

    if(clickable && num < 1){
        int x = tile.getPosition().x, y = tile.getPosition().y;
        for(int i = 0; i < 9; i++){
            if(pencilMarks[i]){
                sf::Text pencil(to_string(i+1), font, 14);
                pencil.setPosition(x + (i%3 * 14) + 5, y + (i/3*14)+4);
                pencil.setFillColor(sf::Color(0,0,0,100));
                window.draw(pencil);
            }
        }
    }
}