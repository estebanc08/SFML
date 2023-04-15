#include "tetris.hpp"
sf::Font font;

tetris::tile::tile(){
    color = sf::Color(200, 230, 255);
    rect.setFillColor(color);
}

tetris::tetris(){
    board.resize(20, vector<tetris::tile>(10));
    for(int y = 0; y < board.size(); y++){
        for(int x = 0; x < board[0].size(); x++){
            board[y][x].rect.setSize(sf::Vector2f(TILE_WIDTH,TILE_WIDTH));
            board[y][x].rect.setPosition(x * TILE_WIDTH, y * TILE_WIDTH);
        }
    }
}

tetris::tetris(int width, int height){
    this->height = height;
    this->width = width;
    board.resize(height, vector<tetris::tile>(width));
    for(int y = 0; y < board.size(); y++){
        for(int x = 0; x < board[0].size(); x++){
            board[y][x].rect.setSize(sf::Vector2f(TILE_WIDTH,TILE_WIDTH));
            board[y][x].rect.setPosition(PADDING + x * TILE_WIDTH, y * TILE_WIDTH);
        }
    }
    nextShape = true;
    loadShapes();
    for(int i = 0; i < 10; i++){
        shapeQueue.push(randomShape());
    }
    font.loadFromFile("arial.ttf");
    direction = Direction::none;
    sidebar.setSize(sf::Vector2f(6*TILE_WIDTH, 12*TILE_WIDTH));
    sidebar.setPosition(PADDING + width*TILE_WIDTH+TILE_WIDTH, TILE_WIDTH*2);
    sidebar.setFillColor(sf::Color(200, 230, 255));
}

void tetris::draw(sf::RenderWindow& window){
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[0].size(); j++){
            window.draw(board[i][j].rect);
        }
    }
    if(!deleting && !gameOver){
        for(auto& it : currShape.blocks){
            window.draw(it);
        }
    }

    //grid lines
    sf::RectangleShape line(sf::Vector2f(width*TILE_WIDTH, 1));
    line.setFillColor(sf::Color(100,100,100));
    for(int i = 1; i < board.size(); i++){
        line.setPosition(PADDING, i*TILE_WIDTH);
        window.draw(line);
    }
    line.setSize(sf::Vector2f(1, window.getSize().y));
    for(int i = 1; i <= board[0].size(); i++){
        line.setPosition(i*TILE_WIDTH+PADDING, 0);
        window.draw(line);
    }
    //Line for sidebar
    window.draw(sidebar);
    queue<shape> tempQueue = shapeQueue;
    for(int i = 0; i < 3; i++){
        shape temp = tempQueue.front();
        tempQueue.pop();
        for(int j = 0; j < temp.blocks.size(); j++){
            float x = sidebar.getPosition().x + TILE_WIDTH+temp.blocks[j].getPosition().x+temp.offset*TILE_WIDTH;
            float y = sidebar.getPosition().y + temp.blocks[j].getPosition().y + TILE_WIDTH*4*i + TILE_WIDTH;
            temp.blocks[j].setPosition(sf::Vector2f(x,y));
            window.draw(temp.blocks[j]);
        }
    }

    line.setSize(sf::Vector2f(sidebar.getGlobalBounds().width, 1));
    for(int i = 0; i <= sidebar.getGlobalBounds().height / TILE_WIDTH; i++){
        line.setPosition(sidebar.getPosition().x, sidebar.getPosition().y + i * TILE_WIDTH);
        window.draw(line);
    }
    line.setSize(sf::Vector2f(1, sidebar.getGlobalBounds().height));
    for(int i = 0; i <= sidebar.getGlobalBounds().width / TILE_WIDTH; i++){
        line.setPosition(sidebar.getPosition().x+i*TILE_WIDTH, sidebar.getPosition().y);
        window.draw(line);
    }

    sf::Text linesClear("LINES CLEARED: " + to_string(linesCleared), font, 18);
    linesClear.setFillColor(sf::Color::White);
    linesClear.setPosition(sf::Vector2f(20, TILE_WIDTH*4));
    window.draw(linesClear);

    if(gameOver){
        sf::RectangleShape gameDone;
        gameDone.setFillColor(sf::Color::White);
        gameDone.setSize(sf::Vector2f(TILE_WIDTH*5, TILE_WIDTH*5));
        gameDone.setPosition(width*TILE_WIDTH/2-gameDone.getGlobalBounds().width/2+PADDING, height*TILE_WIDTH/2-gameDone.getGlobalBounds().height/2);
        window.draw(gameDone);
        sf::Text text("GAME OVER", font, 20);
        text.setFillColor(sf::Color::Black);
        int x = gameDone.getPosition().x + (gameDone.getGlobalBounds().width-text.getGlobalBounds().width)/2;
        int y = gameDone.getPosition().y + (gameDone.getGlobalBounds().height - text.getGlobalBounds().height)/2;
        text.setPosition(x, gameDone.getPosition().y + gameDone.getGlobalBounds().height/3);
        window.draw(text);
    }
}

void tetris::fall(){
    //check if a movement down collides with the bottom
    if(!collisionCheck()){
        nextShape = true;
        for(int i = 0; i < currShape.blocks.size(); i++){
            int y = currShape.blocks[i].getPosition().y/TILE_WIDTH;
            int x = (currShape.blocks[i].getPosition().x-PADDING)/TILE_WIDTH;
            board[y][x].rect.setFillColor(currShape.blocks[i].getFillColor());
            board[y][x].isShape = true;
        }
        clearRow();
        gameOver = checkGameOver();
        return;
    }
    //if no collision, move shape down
    for(auto& it : currShape.blocks){
        int y = it.getPosition().y + TILE_WIDTH;
        it.setPosition(sf::Vector2f(it.getPosition().x, y));
    }
}

void tetris::update(){
    if(gameOver){
        return;
    }
    if(nextShape){
        currShape = shapeQueue.front();
        shapeQueue.pop();
        shapeQueue.push(randomShape());
        for(auto& it : currShape.blocks){
            it.setPosition(it.getPosition().x+(width-1)/2*TILE_WIDTH+PADDING, it.getPosition().y);
        }
        nextShape = false;
        deleting = false;
        return;
    }
    if(direction == Direction::none)
        return;
    else
        fall();
}

void tetris::clearRow(){
    deleting = true;
    int row, col;
    set<int> rowsDelete;
    for(int y = 0; y < board.size(); y++){
        bool delRow = true;
        for(int x = 0; x < board[0].size(); x++){
            delRow &= board[y][x].isShape;
        }
        if(delRow){
            rowsDelete.emplace(y);
            linesCleared++;
            if(FALLING_SPEED > 0)
                FALLING_SPEED -= 0.1f;
            if(FALLING_SPEED < 0)
                FALLING_SPEED = 0;
        }
    }
    if(rowsDelete.empty())
        return;
    for(int it : rowsDelete){
        board.erase(board.begin() + it);
        for(int y = it-1; y >= 0; y--){
            for(int x = 0; x < board[0].size(); x++){
                int i = board[y][x].rect.getPosition().x;
                int j = board[y][x].rect.getPosition().y + TILE_WIDTH;
                board[y][x].rect.setPosition(i, j);
            }
        }
        board.insert(board.begin(), vector<tetris::tile>(width));
        for(int i = 0; i < width; i++){
            board[0][i].rect.setPosition(i * TILE_WIDTH+PADDING, 0);
            board[0][i].rect.setSize(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
        }
    }
}

bool tetris::checkGameOver(){
    for(int i = 0; i < board[0].size(); i++){
        if(board[0][i].isShape)
            return true;
    }
    return false;
}

void tetris::restart(){
    gameOver = false;
    for(int y = 0; y < board.size(); y++){
        for(int x = 0; x < board[0].size(); x++){
            board[y][x].rect.setFillColor(sf::Color(200, 230, 255));
            board[y][x].isShape = false;
        }
    }
    nextShape = true;
    while(!shapeQueue.empty())
        shapeQueue.pop();
    for(int i = 0; i < 10; i++)
        shapeQueue.push(randomShape());
    deleting = false;
    linesCleared = 0;
}

bool tetris::collisionCheck(){
     for(auto& it : currShape.blocks){
        if(it.getPosition().y +TILE_WIDTH >= TILE_WIDTH * height){
            return false;
        }
        for(int y = 0; y < board.size(); y++){
            for(int x = 0; x < board[x].size(); x++){
                if(!board[y][x].isShape)
                    continue;
                auto pos = board[y][x].rect.getPosition();
                if(pos.x == it.getPosition().x && pos.y == it.getPosition().y + TILE_WIDTH)
                    return false;
            }
        }
    }
    return true;
}

bool tetris::validRotate(shape& shape){
     for(auto& it : shape.blocks){
        if(it.getPosition().y +TILE_WIDTH >= TILE_WIDTH * height){
            return false;
        }
        if(it.getPosition().x >= width*TILE_WIDTH+PADDING || it.getPosition().x < PADDING)
            return false;
        for(int y = 0; y < board.size(); y++){
            for(int x = 0; x < board[x].size(); x++){
                if(!board[y][x].isShape)
                    continue;
                auto pos = board[y][x].rect.getPosition();
                if(pos.x == it.getPosition().x && pos.y == it.getPosition().y + TILE_WIDTH)
                    return false;
            }
        }
    }
    return true;
}

void tetris::move(){
    if(deleting)
        return;
    int curr = 0;
    switch(direction){
        case Direction::up:
            if(currShape.type != "O"){
                tetris::shape temp = currShape;
                temp.rotateShape();
                if(validRotate(temp))
                    currShape = temp;
            }
            break;
        case Direction::left:
            curr = width+PADDING+1;
            for(auto& it : currShape.blocks){
                curr = min(curr, (int)it.getPosition().x - TILE_WIDTH);
            }
            if(curr < PADDING)
                return;
            for(auto it : currShape.blocks){
                for(int y = 0; y < board.size(); y++){
                    for(int x = 0; x < board[x].size(); x++){
                        if(!board[y][x].isShape)
                            continue;
                        auto pos = board[y][x].rect.getPosition();
                        if(pos.x == it.getPosition().x - TILE_WIDTH && pos.y == it.getPosition().y)
                            return;
                    }
                }
            }
            for(auto& it : currShape.blocks){
                it.setPosition(it.getPosition().x - TILE_WIDTH, it.getPosition().y);
            }
            break;
        case Direction::right:
            curr = -1;
            for(auto& it : currShape.blocks){
                curr = max(curr, (int)it.getPosition().x + TILE_WIDTH);
            }
            if(curr >= width * TILE_WIDTH+PADDING)
                return;
            for(auto it : currShape.blocks){
                for(int y = 0; y < board.size(); y++){
                    for(int x = 0; x < board[x].size(); x++){
                        if(!board[y][x].isShape)
                            continue;
                        auto pos = board[y][x].rect.getPosition();
                        if(pos.x == it.getPosition().x + TILE_WIDTH && pos.y == it.getPosition().y)
                            return;
                    }
                }
            }
            for(auto& it : currShape.blocks){
                it.setPosition(it.getPosition().x + TILE_WIDTH, it.getPosition().y);
            }
            break;
        case Direction::down:
            fall();
            break;
        default:
            return;
    }
}

int tetris::randomNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, width-1);
    return dis(gen);
}

tetris::shape tetris::randomShape(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,6);
    int num = dis(gen);
    switch(num)
    {
        case 0:
            return shapes["L"];
        case 1:
            return shapes["I"];
        case 2:
            return shapes["O"];
        case 3:
            return shapes["T"];
        case 4:
            return shapes["S"];
        case 5:
            return shapes["Z"];
        case 6:
            return shapes["J"];
        default:
            return shape();
    }
    return shape();
}

void tetris::shape::draw(sf::RenderWindow& window){
    for(int i = 0; i < blocks.size(); i++)
        window.draw(blocks[i]);
}

void tetris::shape::rotateShape(){
     sf::Vector2f pivot = blocks[1].getPosition();

    // Rotate each block around the pivot point
    for (int i = 0; i < 4; i++) {
        // Translate the block so that the pivot point is at the origin
        sf::Vector2f offset = blocks[i].getPosition() - pivot;

        // Apply the rotation matrix to the block
        float x = offset.x;
        float y = offset.y;
        sf::Vector2f pos = sf::Vector2f( y * -1, x * 1) + pivot;
        blocks[i].setPosition(sf::Vector2f(y * -1, x ) + pivot);
    }
}

void tetris::loadShapes(){
    sf::RectangleShape rectangle1(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    sf::RectangleShape rectangle2(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    sf::RectangleShape rectangle3(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    sf::RectangleShape rectangle4(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));

    // set the position and color of each rectangle
    sf::Color lightYellow(250, 237, 203);
    rectangle1.setPosition(sf::Vector2f(0, 0));
    rectangle1.setFillColor(lightYellow);
    rectangle2.setPosition(sf::Vector2f(0, TILE_WIDTH));
    rectangle2.setFillColor(lightYellow);
    rectangle3.setPosition(sf::Vector2f(0, TILE_WIDTH*2));
    rectangle3.setFillColor(lightYellow);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH, TILE_WIDTH*2));
    rectangle4.setFillColor(lightYellow);
    shape L;
    L.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["L"] = L;
    shapes["L"].type = "L";
    shapes["L"].offset = 1;
    
    sf::Color blue(102, 153, 204);
    rectangle1.setPosition(sf::Vector2f(0, 0));
    rectangle1.setFillColor(blue);
    rectangle2.setPosition(sf::Vector2f(TILE_WIDTH, 0));
    rectangle2.setFillColor(blue);
    rectangle3.setPosition(sf::Vector2f(TILE_WIDTH*2, 0));
    rectangle3.setFillColor(blue);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH*3, 0));
    rectangle4.setFillColor(blue);
    shape I;
    I.type = "I";
    I.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["I"] = I;
    shapes["I"].type = "I";


    sf::Color lightBlue(200,255,220, 255);
    rectangle1.setPosition(sf::Vector2f(0, TILE_WIDTH));
    rectangle1.setFillColor(lightBlue);
    rectangle2.setPosition(sf::Vector2f(TILE_WIDTH, 0));
    rectangle2.setFillColor(lightBlue);
    rectangle3.setPosition(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    rectangle3.setFillColor(lightBlue);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH*2, TILE_WIDTH));
    rectangle4.setFillColor(lightBlue);
    shape T;
    T.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["T"] = T;
    shapes["T"].type = "T";


    sf::Color lightPurple(199, 185, 220,255);
    rectangle1.setPosition(sf::Vector2f(0, 0));
    rectangle1.setFillColor(lightPurple);
    rectangle2.setPosition(sf::Vector2f(TILE_WIDTH, 0));
    rectangle2.setFillColor(lightPurple);
    rectangle3.setPosition(sf::Vector2f(0, TILE_WIDTH));
    rectangle3.setFillColor(lightPurple);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    rectangle4.setFillColor(lightPurple);
    shape O;
    O.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["O"] = O;
    shapes["O"].type = "O";
    shapes["O"].offset = 1;

    sf::Color lightPink(242, 198, 222,255);
    rectangle1.setPosition(sf::Vector2f(0, TILE_WIDTH));
    rectangle1.setFillColor(lightPink);
    rectangle2.setPosition(sf::Vector2f(TILE_WIDTH, 0));
    rectangle2.setFillColor(lightPink);
    rectangle3.setPosition(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    rectangle3.setFillColor(lightPink);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH*2, 0));
    rectangle4.setFillColor(lightPink);
    shape S;
    S.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["S"] = S;
    shapes["S"].type = "S";

    sf::Color lightOrange(227, 197, 197,255);
    rectangle1.setPosition(sf::Vector2f(0, 0));
    rectangle1.setFillColor(lightOrange);
    rectangle2.setPosition(sf::Vector2f(TILE_WIDTH, 0));
    rectangle2.setFillColor(lightOrange);
    rectangle3.setPosition(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    rectangle3.setFillColor(lightOrange);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH*2, TILE_WIDTH));
    rectangle4.setFillColor(lightOrange);
    shape Z;
    Z.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["Z"] = Z;
    shapes["Z"].type = "Z";

    sf::Color grape(212, 175, 185,255);
    rectangle1.setPosition(sf::Vector2f(0, 0));
    rectangle1.setFillColor(grape);
    rectangle2.setPosition(sf::Vector2f(0, TILE_WIDTH));
    rectangle2.setFillColor(grape);
    rectangle3.setPosition(sf::Vector2f(TILE_WIDTH, TILE_WIDTH));
    rectangle3.setFillColor(grape);
    rectangle4.setPosition(sf::Vector2f(TILE_WIDTH*2, TILE_WIDTH));
    rectangle4.setFillColor(grape);
    shape J;
    J.blocks = {rectangle1, rectangle2, rectangle3, rectangle4};
    shapes["J"] = J;
    shapes["J"].type = "J";
}