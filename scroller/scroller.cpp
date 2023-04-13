#include "scroller.hpp"
sf::Font font;
bool gameOver;
scroller::tile::tile(){
    block = false;
    platform = false;
    tileRect.setFillColor(sf::Color::Black);
    tileRect.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
}
void scroller::clickReset(sf::Vector2f mousePos){
    if(!gameOver)
        return;
    if(!reset.getGlobalBounds().contains(mousePos))
        return;
    resetGame();
    gameOver = false;
    direction = Direction::none;
}
void scroller::tile::drawTile(sf::RenderWindow& window){
    if(platform)
        tileRect.setFillColor(sf::Color::White);
    else
        tileRect.setFillColor(sf::Color::Black);
    window.draw(tileRect);
}

pair<int,int> scroller::randomNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randX(0, width-1);
    std::uniform_int_distribution<> randY(0, height-1);
    int x = randX(gen), y = randY(gen);
    return {x,y};
}
void scroller::draw(sf::RenderWindow& window){
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
    window.draw(player);
    if(gameOver){
        window.draw(reset);
        window.draw(resetText);
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
scroller::scroller(){
    width = 0;
    height = 0;
}
scroller::scroller(int width, int height){
    this->width = width;
    this->height = height;
    tiles.resize(height, vector<tile>(width));
    reset.setPosition((width * TILE_SIZE / 2) - 100, (height * TILE_SIZE / 2) - 50);
    reset.setSize(sf::Vector2f(200.f,100.f));
    reset.setFillColor(sf::Color(173,216,230, 255));
    font.loadFromFile("arial.ttf");
    resetText.setFont(font);
    resetText.setString("Reset Game");
    resetText.setCharacterSize(25);
    resetText.setFillColor(sf::Color::Black);
    resetText.setPosition(reset.getPosition().x + 30, reset.getPosition().y + 35);
    player.setFillColor(sf::Color::Red);
    player.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    player.setPosition(0, (height-1)*TILE_SIZE);
    gameOver = false;
    movement.x = 0, movement.y = 0;
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[0].size(); j++){
                int x = j * TILE_SIZE;
                int y = i * TILE_SIZE;
                tiles[i][j].tileRect.setPosition(sf::Vector2f(x, y));
        }
    }
   resetGame();
}

void scroller::resetGame(){
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[0].size(); j++){
                tiles[i][j].platform = false;
                tiles[i][j].block = false;
        }
    }
    for(int i = 0; i < 5; i++){
        tiles[10][i].platform = true;
        tiles[15][i+10].platform = true;
        tiles[20][i+15].platform = true;
    }
}

void scroller::move(){
    int x = player.getPosition().x, y = player.getPosition().y;
    int row = y / TILE_SIZE;
    int col = x / TILE_SIZE;
    //move up
    if(movement.y > 0 && y > 0 && !tiles[row-1][col].platform){
         movement.y--;
        player.setPosition(x, y-25.f);
    }
    //move down
    else if(movement.y < 0 && y < (height-1)*TILE_SIZE && !tiles[row+1][col].platform){
        movement.y++;
        player.setPosition(x, y+25.f);
    }
    else{
        movement.y = 0;
    }
    //moving right
    if(movement.x > 0 && x < (width-1)*TILE_SIZE && !tiles[row][col+1].platform){
        movement.x--;
        player.setPosition(x+25.f, y);
    }
    //moving left
    else if(movement.x < 0 && x > 0 && !tiles[row][col-1].platform){
        movement.x++;
        player.setPosition(x-25.f, y);
    }
    else
        movement.x = 0;
}
void scroller::scroll(){
    if(gameOver || direction == Direction::none)
        return;
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[0].size(); j++){
            if(tiles[i][j].platform){
                int x = i;
                x--;
                if(x < 0)
                    x = width-1;
                tiles[x][j].platform = true;
                tiles[i][j].platform = false;
            }
        }
    }
}

bool scroller::canJump(){
    // int x = player.getPosition().x / TILE_SIZE, y = player.getPosition().y / TILE_SIZE;
    int x = player.getPosition().y / TILE_SIZE, y = player.getPosition().x / TILE_SIZE;
    if(x == (height -1) || tiles[x+1][y].platform){
        return true;
    }
    else
        return false;
}