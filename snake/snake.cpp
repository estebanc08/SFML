#include "snake.hpp"
bool gameLost;
sf::Font font;
bool gameWon;
snake::tile::tile(){
    body = false;
    apple = false;
    tileRect.setFillColor(sf::Color::Black);
    tileRect.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
}
void snake::clickReset(sf::Vector2f mousePos){
    if(!gameLost && !gameWon)
        return;
    if(!reset.getGlobalBounds().contains(mousePos))
        return;
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[0].size(); j++){
            tiles[i][j].apple = false;
            tiles[i][j].body = false;
        }
    }
    pair<int,int> num = randomNumber();
    while(!snakeBody.empty())
        snakeBody.pop();
    tiles[num.first][num.second].body = true;
    snakeBody.push({num.first, num.second});
    gameLost = false;
    randApple();
    size = 1;
    direction = Direction::none;
    headPosX = num.first;
    headPosY = num.second;
}
void snake::tile::drawTile(sf::RenderWindow& window){
    if(apple)
        tileRect.setFillColor(sf::Color::Red);
    else if(body && !gameLost)
        tileRect.setFillColor(sf::Color::Green);
    else if(body && gameLost)
        tileRect.setFillColor(sf::Color::Blue);
    else
        tileRect.setFillColor(sf::Color::Black);
    window.draw(tileRect);
}

pair<int,int> snake::randomNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randX(0, width-1);
    std::uniform_int_distribution<> randY(0, height-1);
    int x = randX(gen), y = randY(gen);
    return {x,y};
}
void snake::draw(sf::RenderWindow& window){
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[0].size(); j++){
            tiles[i][j].drawTile(window);
        }
    }
     for (int i = 0; i <= height; i++) {
        sf::RectangleShape line(sf::Vector2f(TILE_SIZE * width,2.f));
        line.setFillColor(sf::Color::Black);
        line.setPosition(sf::Vector2f(0, i * TILE_SIZE));
        window.draw(line);

    }
    for(int j = 0; j <= width; j++){
        sf::RectangleShape line2(sf::Vector2f(2.f, TILE_SIZE * height));
        line2.setFillColor(sf::Color::Black);
        line2.setPosition(sf::Vector2f(TILE_SIZE * j, 0));
        window.draw(line2);
    }
    if(gameLost || gameWon){
        window.draw(reset);
        window.draw(resetText);
    }
}
snake::snake(){
    width = 0;
    height = 0;
    gameLost = false;
}
snake::snake(int width, int height){
    this->width = width;
    this->height = height;
    gameWon = false;
    size = 1;
    tiles.resize(width, vector<tile>(height));
    pair<int,int> temp = randomNumber();
    int x = temp.first, y = temp.second;
    tiles[x][y].body = true;
    snakeBody.push({x,y});
    headPosX = x;
    headPosY = y;
    reset.setPosition((width * TILE_SIZE / 2) - 100, (height * TILE_SIZE / 2) - 50);
    reset.setSize(sf::Vector2f(200.f,100.f));
    reset.setFillColor(sf::Color(173,216,230, 255));
    font.loadFromFile("arial.ttf");
    resetText.setFont(font);
    resetText.setString("Reset Game");
    resetText.setCharacterSize(25);
    resetText.setFillColor(sf::Color::Black);
    resetText.setPosition(reset.getPosition().x + 30, reset.getPosition().y + 35);
    randApple();
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[0].size(); j++){
                int x = i * TILE_SIZE;
                int y = j * TILE_SIZE;
                tiles[i][j].tileRect.setPosition(sf::Vector2f(x, y));
        }
    }
    gameLost = false;
}
void snake::move(){
    if(gameLost)
        return;
    int x =  headPosX,y = headPosY;
    pair<int,int> pos;
    switch (direction)
        {
            case Direction::up:
                y--;
                if(y < 0)
                    y = height-1;
                break;
            case Direction::down:
                y++;
                if(y >= height)
                    y = 0;
                break;
            case Direction::left:
                x--;
                if(x < 0)
                    x = width-1;
                break;
            case Direction::right:
                x++;
                if(x >= width)
                    x = 0;
                break;
            default:
                return;
        }
    if(tiles[x][y].body){
        gameLost = true;
        return;
    }
    if(tiles[x][y].apple){
        tiles[x][y].body = true;
        size++;
        tiles[headPosX][headPosY].body = true;
        snakeBody.push({headPosX,headPosY});
        randApple();
        tiles[x][y].apple = false;
        if(size == height * width)
            gameWon = true;
    }
    else
        tiles[x][y].body = true;
    headPosY = y;
    headPosX = x;
    pos = snakeBody.front();
    tiles[pos.first][pos.second].body = false;
    snakeBody.push({x,y});
    if(snakeBody.size() >= size)
        snakeBody.pop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
void snake::randApple(){
   
    int x = 0,y = 0;
    do{
        pair<int,int> temp = randomNumber();
        x = temp.first, y = temp.second;
    }
    while(tiles[x][y].body);
    tiles[x][y].apple = true;
}