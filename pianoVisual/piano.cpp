#include "piano.hpp"
sf::Clock musicTimer;

Piano::Piano(){
    whiteKey.setSize(sf::Vector2f(WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT));
    blackKey.setSize(sf::Vector2f(BLACK_KEY_WIDTH,BLACK_KEY_HEIGHT));
    whiteKey.setFillColor(sf::Color::White);
    blackKey.setFillColor(sf::Color::Black);

    for(int i = 0; i < 56; i++){
        whiteKey.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
        whiteKeys.push_back(whiteKey);
    }

    //set first key, base other keys off this one
    blackKey.setPosition(SIDE_PADDING + (2*WHITE_KEY_WIDTH-BLACK_KEY_WIDTH)/2.f, VERTICAL_PADDING);
    blackKeys.push_back(blackKey);
    blackKey.setPosition(blackKey.getPosition().x + WHITE_KEY_WIDTH, VERTICAL_PADDING);

    for(int i = 0; i < 53; i++){
        blackKey.setPosition(blackKey.getPosition().x + WHITE_KEY_WIDTH, VERTICAL_PADDING);
        if(i % 7 == 0 || i % 7 == 1 || i % 7 == 3 || i % 7 == 4 || i % 7 == 5)
            blackKeys.push_back(blackKey);
    }
}


//MOVED TO MAIN... TEMP
void Piano::playMusic(unsigned int currMeasure, unsigned int &start){
    unsigned int currStart = start;
    // if(currMeasure + 1 < sheetMusic.size()){
        for(auto measure : sheetMusic[currMeasure]){
            for(auto note : measure.second){
                sf::Sound sound(notes[note.key]);
                // sound.setLoop(true);
                sound.setPlayingOffset(sf::milliseconds(0));
                sounds.push_back(sound);
            }
        }
    // }
    for(auto currNotes = sheetMusic[currMeasure].begin(); currNotes != sheetMusic[currMeasure].end() && !quit; currNotes++){
        if(quit)
            return;
        unsigned int end = currStart;
        for(; currStart < currNotes->second.size()+end; currStart++){
            sounds[currStart].play();
        }
        sf::sleep(sf::milliseconds(noteHold));
        musicTimer.restart();
    }
    // sounds.clear();
    sounds.erase(std::remove_if(sounds.begin(), sounds.end(), [](const sf::Sound& s){ return s.getStatus() == sf::Sound::Stopped; }), sounds.end());
    start = sounds.size();
}

//MOVED TO MAIN... TEMP
void Piano::draw(sf::RenderWindow& window, unsigned int currMeasure){
    window.clear();
    sf::RectangleShape line(sf::Vector2f(1, WHITE_KEY_HEIGHT));
    line.setFillColor(sf::Color::Black);
    sf::Clock animationTimer;
    for(auto currNotes = sheetMusic[currMeasure].begin(); currNotes != sheetMusic[currMeasure].end() && !quit;){
        sf::Event event;
        if(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                quit = true;
                return;
            }
        }
        if(animationTimer.getElapsedTime().asMilliseconds() > 5){
            for(unsigned int i = 0; i < whiteKeys.size(); i++){
                window.draw(whiteKeys[i]);
            }
            for(unsigned int i = 0; i < blackKeys.size(); i++){
                window.draw(blackKeys[i]);
            }
            for(int i = 1; i < 56; i++){
                line.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
                window.draw(line);
            }
            for(auto keys : currNotes->second){
                string keyStr = keys.key;
                int key = 0;
                if(keyStr == "rest")
                    continue;
                if(keyStr[1] != 'b'){
                    if(keyStr[0] == 'A' || keyStr[0] == 'B')
                        key = keyStr[0] - 'A' + 7*(keyStr[1]-'0');
                    else
                        key = keyStr[0] - 'A' + 7*(keyStr[1]-'0'-1);
                    whiteKeys[key].setFillColor(sf::Color::Green);
                    window.draw(whiteKeys[key]);
                }
                else{
                    if(keyStr[0] == 'B'){
                        key = 5*(keyStr[2]- '0');
                    }
                    else if(keyStr[0] == 'D'){
                        key = 5*(keyStr[2]- '0' -1) + 1;
                    }
                    else if(keyStr[0] == 'E'){
                        key = 5*(keyStr[2]- '0' -1) + 2;
                    }
                    else if(keyStr[0] == 'G'){
                        key = 5*(keyStr[2]- '0' -1) + 3;
                    }
                    else{
                        key = 5*(keyStr[2]-'0'-1)+4;
                    }
                    blackKeys[key].setFillColor(sf::Color::Blue);
                    window.draw(blackKeys[key]);
                }
            }
            for(int i = 1; i < 56; i++){
                line.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
                window.draw(line);
            }
            for(unsigned int i = 0; i < blackKeys.size(); i++){
                window.draw(blackKeys[i]);
            }
            window.display();
            auto remainingTime = sf::milliseconds(noteHold) - animationTimer.getElapsedTime();
            if (remainingTime > sf::Time::Zero) {
                sf::sleep(remainingTime);
            }
            currNotes++;
            animationTimer.restart();
        }
        for(unsigned int i = 0; i < whiteKeys.size(); i++){
            whiteKeys[i].setFillColor(sf::Color::White);
            window.draw(whiteKeys[i]);
        }
        for(int i = 1; i < 56; i++){
            line.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
            window.draw(line);
        }
        for(unsigned int i = 0; i < blackKeys.size(); i++){
            blackKeys[i].setFillColor(sf::Color::Black);
            window.draw(blackKeys[i]);
        }
        window.display();
    }
}

void Piano::readKeysPressed(ifstream& readFile){
    string input = "";
    //read first measure
    getline(readFile, input);
    map<int, vector<Note>> notesPerMeasure;
    while (getline(readFile, input)) {
        regex reg("^Measure.*");
        if (!regex_match(input, reg)) {
            stringstream stream(input);
            string note, posX, length, accidental;
            stream >> note >> posX;
             if(note != "rest"){
                stream  >> length >> accidental;
                if(accidental == "-1"){
                    note = note.substr(0,1) + "b" + note.substr(1);
                }
                else if(accidental == "1"){
                    char toFlat = note[0];
                    toFlat += 1;
                    int octave = note[1]-'0';
                    if(toFlat > 'G'){
                        toFlat = 'A';
                        octave++;
                    }
                    note = string(1, toFlat) + "b" + to_string(octave);
                }
                Note currNote(note, std::stoi(length));
                notesPerMeasure[std::stoi(posX)].push_back(currNote);
             }
            //  else{
            //     Note currNote("rest", 0); //TODO work on how to store rests
            //     notesPerMeasure[std::stoi(posX)].push_back(currNote);
            //  }
        }
        else{
            sheetMusic.push_back(notesPerMeasure);
            notesPerMeasure.clear();
        }
    }
    //last measure
    sheetMusic.push_back(notesPerMeasure);
    
    for(auto measure : sheetMusic){
        for(auto notes : measure){
            vector<Note> temp;
            for(auto note : notes.second)
                temp.push_back(note);
            sheetNotes.push_back(temp);
        }
    }
}

//Not needed for now
void Piano::playKey(sf::Vector2f& mousePos){
    for(unsigned int i = 0; i < blackKeys.size(); i++){
        if(blackKeys[i].getGlobalBounds().contains(mousePos)){
            cout << "black clicked" << endl;
            return;
        }
    }
    for(unsigned int i = 0; i < whiteKeys.size(); i++){
        if(whiteKeys[i].getGlobalBounds().contains(mousePos)){
            cout << "white clicked" << endl;
            return;
        }
    }
}

void Piano::readFile(string name){
    tinyxml2::XMLDocument doc;
    const char* fileName = name.c_str();

    doc.LoadFile(fileName);
    std::size_t lastSlash = name.find_last_of('/');
    std::string newName = name.substr(lastSlash + 1);
    newName = newName.substr(0, newName.size() - 4) + ".txt";
    currFileName  = "sheetMusic/" + newName;
    ofstream outFile(currFileName);
    // Access the root element
    tinyxml2::XMLElement* root = doc.FirstChildElement("score-partwise");

  // Access child elements and their attributes
    for (tinyxml2::XMLElement* part = root->FirstChildElement("part"); part != nullptr; part = part->NextSiblingElement("part")) {
        // const char* id = part->Attribute("id");
        for (tinyxml2::XMLElement* measure = part->FirstChildElement("measure"); measure != nullptr; measure = measure->NextSiblingElement("measure")) {
            const char* number = measure->Attribute("number");
            outFile << "Measure Number: " << number << "\n";
            for (tinyxml2::XMLElement* note = measure->FirstChildElement("note"); note != nullptr; note = note->NextSiblingElement("note")) {
                const char* printObject = note->Attribute("print-object");
                tinyxml2::XMLElement* rest = note->FirstChildElement("rest");
                if(printObject)
                    continue; 
                if (rest) {
                    int duration = 0;
                    tinyxml2::XMLElement* dur = note->FirstChildElement("duration");
                    if (dur != nullptr) {
                        dur->QueryIntText(&duration);
                    }
                    // double prevX = INT_MIN, nextX = INT_MIN;
                    // tinyxml2::XMLElement* prev = note->PreviousSiblingElement();
                    // if (prev != nullptr && prev->Attribute("default-x") != nullptr) {
                    //     prevX = std::stod(prev->Attribute("default-x"));
                    // }

                    // // Get next sibling
                    // tinyxml2::XMLElement* next = note->NextSiblingElement();

                    // // Check if next sibling exists and has a default-x attribute
                    // if (next != nullptr && next->Attribute("default-x") != nullptr) {
                    //     nextX = std::stod(next->Attribute("default-x"));
                    // }
                    
                    // outFile << "rest " << duration << " " << "\n";
                    outFile << "rest " << duration <<"\n";
                    continue;
                }   
                tinyxml2::XMLElement* pitch = note->FirstChildElement("pitch");
                tinyxml2::XMLElement* step = pitch->FirstChildElement("step");
                tinyxml2::XMLElement* octave = pitch->FirstChildElement("octave");
                int duration = 0;
                tinyxml2::XMLElement* alt = pitch->FirstChildElement("alter");
                tinyxml2::XMLElement* dur = note->FirstChildElement("duration");
                int altCount = 0;
                if (dur != nullptr) {
                    dur->QueryIntText(&duration);
                }
                if(alt){
                    altCount = std::stoi(alt->GetText());
                }
                int defaultX = 0;
                note->QueryIntAttribute("default-x", &defaultX);
                outFile << step->GetText() << octave->GetText() << " " << defaultX << " " << duration  << " " << altCount << "\n";
            }
        }
    }
    outFile.close();
}