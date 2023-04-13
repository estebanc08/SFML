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
    for(int i = 0; i < width / TILE_WIDTH; i++){
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
    toSwap = -1;
}
void sorts::changeSort(string type){
    sortType = type;
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
    if(sortType == "bubble"){
        if(bubbleSort(nums, i, j, toSwap))
            done = true;
    }
    else if(sortType == "insertion"){
        if(insertionSort(nums, i, j, toSwap))
            done = true;
    }
    else if(sortType == "selection"){
        if(selectionSort(nums, i, toSwap))
            done = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    else if(sortType == "shuffle"){
        shuffle();
        sortType = "";
        done = false;
        toSwap = -1;
    }
}

void sorts::clickType(sf::Vector2f mousePos){
    for(auto button : buttons){
        if(button.second.first.getGlobalBounds().contains(mousePos)){
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
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    float lineWidth = TILE_WIDTH >= 5 ? 1 : (float)TILE_WIDTH / 5;
    sf::RectangleShape line(sf::Vector2f(lineWidth, height));
    line.setFillColor(babyBlue);
    for(int i = 0; i < nums.size(); i++){
        line.setPosition(TILE_WIDTH*i, 0);
        window.draw(line);
    }

    for(auto button : buttons){
        if(button.first == sortType)
            button.second.first.setFillColor(sf::Color::Green);
        else
            button.second.first.setFillColor(babyBlue);
        window.draw(button.second.first);
        window.draw(button.second.second);
    }
}

void sorts::loadButtons(){
   font.loadFromFile("arial.ttf");
    sf::RectangleShape bubble;
    bubble.setFillColor(babyBlue);
    bubble.setSize(sf::Vector2f(150, 75));
    bubble.setPosition(30, height + 40);
    sf::Text bubbleText("Bubble sort", font, 20);
    int x = (bubble.getGlobalBounds().width - bubbleText.getGlobalBounds().width)/2 + bubble.getPosition().x;
    int y = (bubble.getGlobalBounds().height - bubbleText.getGlobalBounds().height)/2 + bubble.getPosition().y-5;
    bubbleText.setPosition(x, y);
    bubbleText.setFillColor(sf::Color::Black);
    buttons["bubble"] = {bubble, bubbleText};

    sf::RectangleShape selection;
    selection.setFillColor(babyBlue);
    selection.setSize(sf::Vector2f(150, 75));
    selection.setPosition(210, height + 40);
    sf::Text selectionText("Selection sort", font, 20);
    x = (selection.getGlobalBounds().width  - selectionText.getGlobalBounds().width)/2 + selection.getPosition().x;
    selectionText.setPosition(x, y);
    selectionText.setFillColor(sf::Color::Black);
    buttons["selection"] = {selection, selectionText};

     sf::RectangleShape insertion;
    insertion.setFillColor(babyBlue);
    insertion.setSize(sf::Vector2f(150, 75));
    insertion.setPosition(390, height + 40);
    sf::Text insertionText("Insertion sort", font, 20);
    x = (selection.getGlobalBounds().width  - insertionText.getGlobalBounds().width)/2 + insertion.getPosition().x;
    insertionText.setPosition(x, y);
    insertionText.setFillColor(sf::Color::Black);
    buttons["insertion"] = {insertion, insertionText};

     sf::RectangleShape shuffle;
    shuffle.setFillColor(babyBlue);
    shuffle.setSize(sf::Vector2f(150, 75));
    shuffle.setPosition(width - 180, height+40);
    sf::Text shuffleText("New numbers", font, 20);
    x = (shuffle.getGlobalBounds().width  - shuffleText.getGlobalBounds().width)/2 + shuffle.getPosition().x;
    shuffleText.setPosition(x, y);
    shuffleText.setFillColor(sf::Color::Black);
    buttons["shuffle"] = {shuffle, shuffleText};

    back.setPosition(0,0);
    back.setFillColor(babyBlue);
    back.setSize(sf::Vector2f(width, height));
}   