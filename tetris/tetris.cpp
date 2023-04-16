#include "tetris.hpp"
sf::Font font;
sf::Color background;

tetris::tile::tile(){
    rect.setFillColor(background);
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
    background = sf::Color(200, 230, 255, 155);
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
    // sidebar.setFillColor(sf::Color(200, 230, 255));
    sidebar.setFillColor(sf::Color::Black);
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
    if(!deleting && shadowReady){
        shadow = makeShadow();
        for(auto& it : shadow.blocks){
            int r = it.getFillColor().r, g = it.getFillColor().g, b = it.getFillColor().b;
            it.setFillColor(sf::Color(r,g,b,100));
            window.draw(it);
        }
    }
        
    //grid lines
    sf::RectangleShape line(sf::Vector2f(width*TILE_WIDTH, 1));
    line.setFillColor(sf::Color(100,100,100, 200));
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
    line.setFillColor(sf::Color::Black);
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

    sf::Text pointText("POINTS\n  " + to_string(points), font, 18);
    pointText.setFillColor(sf::Color::White);
    pointText.setPosition(sf::Vector2f(20, TILE_WIDTH*10));
    window.draw(pointText);

    sf::Text nextBlocks("NEXT SHAPES", font, 18);
    nextBlocks.setFillColor(sf::Color::White);
    nextBlocks.setPosition(sf::Vector2f(sidebar.getPosition().x + (sidebar.getGlobalBounds().width-nextBlocks.getGlobalBounds().width)/2, TILE_WIDTH));
    window.draw(nextBlocks);

    if(gameOver){
        sf::RectangleShape gameDone;
        gameDone.setFillColor(sf::Color::White);
        gameDone.setSize(sf::Vector2f(TILE_WIDTH*5, TILE_WIDTH*5));
        gameDone.setPosition(width*TILE_WIDTH/2-gameDone.getGlobalBounds().width/2+PADDING, height*TILE_WIDTH/2-gameDone.getGlobalBounds().height/2);
        window.draw(gameDone);
        sf::Text text("GAME OVER\n\tPRESS R\nTO RESTART", font, 20);
        text.setFillColor(sf::Color::Black);
        int x = gameDone.getPosition().x + (gameDone.getGlobalBounds().width-text.getGlobalBounds().width)/2;
        int y = gameDone.getPosition().y + (gameDone.getGlobalBounds().height - text.getGlobalBounds().height)/2;
        text.setPosition(x, gameDone.getPosition().y + gameDone.getGlobalBounds().height/3);
        window.draw(text);
    }
}

void tetris::animateRowClear(sf::RenderWindow& window){
    for(int i = 0; i < width; i++){
        window.clear();
        for(int it : rowsDelete){
            board[it][i].rect.setFillColor(sf::Color::White);
        }
        this_thread::sleep_for(std::chrono::milliseconds(5));
        draw(window);
        window.display();
    }

    for(int i = 0; i < width; i++){
        window.clear();
        for(int it : rowsDelete){
            board[it][i].rect.setFillColor(background);
        }
        this_thread::sleep_for(std::chrono::milliseconds(25));
        draw(window);
        window.display();
    }

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
    rowsDelete.clear();
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
    currShape.pivot.y += TILE_WIDTH;
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
        currShape.pivot.x += (width-1)/2*TILE_WIDTH+PADDING;
        nextShape = false;
        deleting = false;
        shadowReady = true;
        shadow = makeShadow();
        return;
    }
    // shadow = makeShadow();
    if(direction == Direction::none && pause){
        return;
    }
    else
        fall();
}

void tetris::clearRow(){
    deleting = true;
    int row, col;
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
    switch(rowsDelete.size()){
        case 1:
            points += 40;
            break;
        case 2:
            points += 100;
            break;
        case 3:
            points += 300;
            break;
        case 4:
            points += 1200;
            break;
        default:
            break;
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
    shadowReady = false;
    points = 0;
    pause = true;
    for(int y = 0; y < board.size(); y++){
        for(int x = 0; x < board[0].size(); x++){
            board[y][x].rect.setFillColor(background);
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

bool tetris::validPosition(shape& shape){
     for(auto& it : shape.blocks){
        if(it.getPosition().y + TILE_WIDTH >= TILE_WIDTH * height || it.getPosition().y <= 0){
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
    pause = false;
    switch(direction){
        case Direction::up:
            if(currShape.type != "O"){
                tetris::shape temp = currShape;
                temp.rotateShape();
                if(validPosition(temp))
                    currShape = temp;
                else
                {
                    int dx = 0;
                    int dy = 0;
                    for (int i = 0; i < standardWallkick.size(); i++)
                    {
                            for(int j = 0; j < standardWallkick[i].size(); j++){
                                dx = standardWallkick[i][j].first;
                                dy = standardWallkick[i][j].second;
                                temp = currShape;
                                for(int i = 0; i < temp.blocks.size(); i++)
                                    temp.blocks[i].setPosition(temp.blocks[i].getPosition().x + dx*TILE_WIDTH, temp.blocks[i].getPosition().y + dy*TILE_WIDTH);
                                temp.rotateShape();
                                if (validPosition(temp)){
                                    currShape = temp;
                                    return;
                                }
                        }
                    }

                    for (int i = 0; i < reverseWallkick.size(); i++)
                    {
                            for(int j = 0; j < reverseWallkick[i].size(); j++){
                                dx = reverseWallkick[i][j].first;
                                dy = reverseWallkick[i][j].second;
                                temp = currShape;
                                for(int i = 0; i < temp.blocks.size(); i++)
                                    temp.blocks[i].setPosition(temp.blocks[i].getPosition().x + dx*TILE_WIDTH, temp.blocks[i].getPosition().y + dy*TILE_WIDTH);
                                temp.rotateShape();
                                if (validPosition(temp)){
                                    currShape = temp;
                                    return;
                                }
                        }
                    }
                }
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
            currShape.pivot.x -= TILE_WIDTH;
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
            currShape.pivot.x += TILE_WIDTH;
            break;
        case Direction::down:
            points++;
            fall();
            break;
        default:
            return;
    }
}

tetris::shape tetris::makeShadow(){
    int y = height*TILE_WIDTH;
    for(auto& block : currShape.blocks){
        y = min(y, (int)block.getPosition().y);
    }
    if(y !=0 && !validPosition(currShape))
        return currShape;
    shape ret = currShape;
    bool touching = false;
    while(true){
        shape temp = ret;
        for(auto& block : temp.blocks){
            block.setPosition(block.getPosition().x, block.getPosition().y + TILE_WIDTH);
        }
        if(!validPosition(temp)){
            return temp;
        }
        ret = temp;
    }
    return ret;
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
    for (auto& block : blocks) {
        sf::Vector2f pos = block.getPosition();
        int x = pos.x - pivot.x;
        int y = pos.y - pivot.y;
        block.setPosition(sf::Vector2f((int)(-y + pivot.x)/TILE_WIDTH*TILE_WIDTH, (int)(x + pivot.y)/TILE_WIDTH*TILE_WIDTH));
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


    //set pivots
    shapes["S"].pivot.x =  1.5f* TILE_WIDTH;
    shapes["S"].pivot.y = TILE_WIDTH;

    shapes["Z"].pivot.x =  1.5f* TILE_WIDTH;
    shapes["Z"].pivot.y = TILE_WIDTH;

    shapes["J"].pivot.x =  TILE_WIDTH;
    shapes["J"].pivot.y = TILE_WIDTH*1.5f;


    shapes["L"].pivot.x =  TILE_WIDTH;
    shapes["L"].pivot.y = TILE_WIDTH*1.5f;

    shapes["T"].pivot.x =  TILE_WIDTH*1.5f;
    shapes["T"].pivot.y = TILE_WIDTH;


    shapes["I"].pivot.x =  TILE_WIDTH*2.5f;
    shapes["I"].pivot.y = TILE_WIDTH/2.f;

    standardWallkick = {{{-1, 0}, {-1,  1}, {0, -2}, {-1, -2}}, 
                        {{ 1, 0}, { 1, -1}, {0,  2}, { 1,  2}}, 
                        {{ 1, 0}, { 1,  1}, {0, -2}, { 1, -2}}, 
                        {{-1, 0}, {-1, -1}, {0,  2}, {-1,  2}}};

    reverseWallkick =  {{{ 1, 0}, { 1,  1}, { 1, -2}},
                        {{ 1, 0}, { 1, -1}, { 1,  2}},
                        {{-1, 0}, {-1,  1}, {-1, -2}},
                        {{-1, 0}, {-1, -1}, {-1,  2}}};
}