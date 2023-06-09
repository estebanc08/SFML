#include "sort.hpp"
bool done;
int numTotal;
sf::Font font;

sorts::tile::tile(){
    num = 0;
    tileRect.setFillColor(sf::Color::Black);
    tileRect.setSize(sf::Vector2f(0,0));
}
sorts::tile::tile(int num){
    this->num = num;
    tileRect.setFillColor(sf::Color::Black);
    tileRect.setSize(sf::Vector2f(5, num));
}
sorts::sorts(){
    for(int i = 0; i < 5000; i++){
        int num = randomNumber();
        nums.push_back(num);
    }
}
void sorts::shuffle(){
    original.clear();
    nums.clear();
   if(TILE_WIDTH > width)
        return;
    for(int i = 0; i < (int)ceil(width / TILE_WIDTH); i++){
        int num = randomNumber();
        original.push_back(num);
        nums.push_back(num);
    }
}
sorts::sorts(int width, int height){
    this->width = width;
    this->height = height;
    babyBlue = sf::Color(173, 216, 230);
    done = false;
    numTotal = 0;
    this->i = this->j = 0;
    loadButtons();
    shuffle();
    // TILE_WIDTH = 50;
    toSwap = -1;
    size = nums.size();
}
void sorts::changeSort(string type){
    buttonClicked = type;
    nums = original;
    done = false;
    this->i = this->j = 0;
    if(type == "insertion")
        this->i = 1;
    numTotal = 0;
}
int sorts::randomNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, height-1);
    return dis(gen);
}
void sorts::update(sf::RenderWindow& window){
    if(done)
        return;
    if(buttonClicked == "bubble"){
        if(bubbleSort(nums, i, j, toSwap))
            done = true;
    }
    else if(buttonClicked == "insertion"){
        if(insertionSort(nums, i, j, toSwap))
            done = true;
    }
    else if(buttonClicked == "selection"){
        if(selectionSort(nums, i, toSwap))
            done = true;
        int wait = TILE_WIDTH;
        if(TILE_WIDTH < 30)
            wait = 30;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait));
    }
    else if(buttonClicked == "shuffle"){
        shuffle();
        buttonClicked = "";
        done = false;
        toSwap = -1;
    }

}
bool sorts::isChangingSize(){
    return changeNums;
}
void sorts::updateSize(string size){
    done = false;
    if(size == ""){
        this->size = 0;
        TILE_WIDTH = width+1;
    }
    else if(size.size() >= 4)
        return;
    else{
        this->size = stoi(size);
        TILE_WIDTH = (float)width / this->size;
    }
    buttons["numbers"].second.setString(size);
    shuffle();
    toSwap = -1;
    this->i = this->j = 0;
    buttonClicked = "";
    numTotal = 0;
}
void sorts::clickType(sf::Vector2f mousePos){
    changeNums = false;
    for(auto button : buttons){
        if(button.second.first.getGlobalBounds().contains(mousePos)){
            if(button.first == "numbers")
                changeNums = true;
            else
                changeSort(button.first);
            return;
        }
    }
}
void sorts::draw(sf::RenderWindow& window){
    window.draw(back);
    if(done){
        //moves the green bar
        for(int i = 0; i < nums.size(); i++){
            sf::RectangleShape rect;
            rect.setFillColor(sf::Color::Black);
            //green bar takes up 15% of the screen at a time 
            if(abs(numTotal-i) <= ceil(nums.size()*.075))
                rect.setFillColor(sf::Color::Green);
            rect.setSize(sf::Vector2f(TILE_WIDTH, nums[i]));
            rect.setPosition(i*TILE_WIDTH, height-nums[i]);
            window.draw(rect);
        }
        numTotal++;
        if(numTotal >= (nums.size() + ceil(nums.size()/13.3333)))
            numTotal = 0;
        //make the bar move faster for larger inputs to make it equal for large and small sorts. Should probably use timer somehow
        int timeWait = TILE_WIDTH / 0.6;
        if(size < 50)
            timeWait = 50;
        std::this_thread::sleep_for(std::chrono::milliseconds(timeWait));
    }
    else{
        for(int i = 0; i < nums.size(); i++){
            sf::RectangleShape rect;
            rect.setFillColor(sf::Color::Black);
            if(i == toSwap)
                rect.setFillColor(sf::Color::Red);
            rect.setSize(sf::Vector2f(TILE_WIDTH, nums[i]));
            rect.setPosition(i*TILE_WIDTH, height-nums[i]);
            window.draw(rect);
        }
        if(buttonClicked != "")
            std::this_thread::sleep_for(std::chrono::milliseconds((int)TILE_WIDTH));
    }
    //too hard to see the lines after this point
    if(TILE_WIDTH > 2){
        float lineWidth = (float)TILE_WIDTH / 50;
        if(lineWidth < 1)
            lineWidth = 1.f;
        sf::RectangleShape line(sf::Vector2f(lineWidth, height));
        line.setFillColor(babyBlue);
        for(int i = 1; i < nums.size(); i++){
            line.setPosition(TILE_WIDTH*i, 0);
            window.draw(line);
        }
    }

    for(auto button : buttons){
        if(button.first == buttonClicked)
            button.second.first.setFillColor(sf::Color::Green);
        window.draw(button.second.first);
        window.draw(button.second.second);
    }

    sf::Text numElements("Number of Elements: ", font, 20);
    numElements.setPosition(30, height + 175);
    numElements.setFillColor(sf::Color::Black);
    window.draw(numElements);
}

void sorts::loadButtons(){
   font.loadFromFile("arial.ttf");
    sf::RectangleShape bubble;
    bubble.setFillColor(babyBlue);
    bubble.setSize(sf::Vector2f(width/10, 75));
    // int x = width / 4 - bubble.getGlobalBounds().width*2;
    int x = width / 8;
    bubble.setPosition(x, height + 40);
    sf::Text bubbleText("Bubble sort", font, 20);
    x = (bubble.getGlobalBounds().width - bubbleText.getGlobalBounds().width)/2 + bubble.getPosition().x;
    int y = (bubble.getGlobalBounds().height - bubbleText.getGlobalBounds().height)/2 + bubble.getPosition().y-5;
    bubbleText.setPosition(x, y);
    bubbleText.setFillColor(sf::Color::Black);
    buttons["bubble"] = {bubble, bubbleText};

    sf::RectangleShape selection;
    selection.setFillColor(babyBlue);
    selection.setSize(sf::Vector2f(width/10, 75));
    // x = width / 2 - selection.getGlobalBounds().width*2;
    x = width/10 + 2*width/8;
    selection.setPosition(x, height + 40);
    sf::Text selectionText("Selection sort", font, 20);
    x = (selection.getGlobalBounds().width  - selectionText.getGlobalBounds().width)/2 + selection.getPosition().x;
    selectionText.setPosition(x, y);
    selectionText.setFillColor(sf::Color::Black);
    buttons["selection"] = {selection, selectionText};

     sf::RectangleShape insertion;
    insertion.setFillColor(babyBlue);
    insertion.setSize(sf::Vector2f(width/10, 75));
    // x = 3 * width / 4 - insertion.getGlobalBounds().width*2;
    x = 2 * width/10 + 3*width/8;
    insertion.setPosition(x, height + 40);
    sf::Text insertionText("Insertion sort", font, 20);
    x = (selection.getGlobalBounds().width  - insertionText.getGlobalBounds().width)/2 + insertion.getPosition().x;
    insertionText.setPosition(x, y);
    insertionText.setFillColor(sf::Color::Black);
    buttons["insertion"] = {insertion, insertionText};

    sf::RectangleShape shuffle;
    shuffle.setFillColor(babyBlue);
    shuffle.setSize(sf::Vector2f(width/10, 75));
    x = 3 * width/10 + 4*width/8;
    shuffle.setPosition(x, height+40);
    sf::Text shuffleText("New numbers", font, 20);
    x = (shuffle.getGlobalBounds().width  - shuffleText.getGlobalBounds().width)/2 + shuffle.getPosition().x;
    shuffleText.setPosition(x, y);
    shuffleText.setFillColor(sf::Color::Black);
    buttons["shuffle"] = {shuffle, shuffleText};

    back.setPosition(0,0);
    back.setFillColor(babyBlue);
    back.setSize(sf::Vector2f(width, height));

    int num = width / TILE_WIDTH;
    string numTemp = to_string(num);
    sf::Text numText(numTemp, font, 20);
    numText.setPosition(225, height+175);
    numText.setFillColor(sf::Color::Black);
    sf::RectangleShape numTiles;
    numTiles.setSize(sf::Vector2f(numText.getGlobalBounds().width+20, numText.getGlobalBounds().height+15));
    numTiles.setPosition(225, height+175);
    numTiles.setFillColor(sf::Color::White);
    buttons["numbers"] = {numTiles, numText};
}   