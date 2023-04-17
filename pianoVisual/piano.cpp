#include "piano.hpp"

piano::piano(){
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
void piano::draw(sf::RenderWindow& window){
    window.clear();
    // if(musicDone || quit)
    //     return;
    sf::RectangleShape line(sf::Vector2f(1, WHITE_KEY_HEIGHT));
    line.setFillColor(sf::Color::Black);
    sf::Clock animationTimer;

    for(auto it = notesToPlay.begin(); it != notesToPlay.end();it++){
        for(auto keys : it->second){
            sf::Sound sound(notes[keys]);
            // sound.setLoop(true);
            sound.setPlayingOffset(sf::milliseconds(1250));
            sounds.emplace_back(notes[keys]);
        }
    }
    int noteHold = 1000;

    thread playMusic([&](){
        unsigned int i = 0;
        for(auto it : notesToPlay){
            int start = i;
            for(; i < it.second.size()+start; i++){
                sounds[i].play();
            }
            sf::sleep(sf::milliseconds(noteHold));
            // sounds.erase(sounds.begin(), sounds.begin()+it.second.size());
        }
            // sounds[i].play();
            // sf::sleep(sf::milliseconds(800));
            // sounds[i].stop();
    });
    
    // bool play = true;
    // cout << notesToPlay.size() << endl;
    for(auto it = notesToPlay.begin(); it != notesToPlay.end();){
        sf::Event event;
        if(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
                return;
            }
        }
        if(animationTimer.getElapsedTime().asMilliseconds() > 50){
            if(quit)
                return;
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
            for(auto keys : it->second){
                int key = 0;
                if(keys[1] != 'b'){
                    key = keys[0] - 'A' + 7*(keys[1]-'0');
                    whiteKeys[key].setFillColor(sf::Color::Green);
                    window.draw(whiteKeys[key]);
                }
                else{
                    if(keys[0] == 'B'){
                        key = 5*(keys[2]- '0');
                    }
                    else if(keys[0] == 'D'){
                        key = 5*(keys[2]- '0' -1) + 1;
                    }
                    else if(keys[0] == 'E'){
                        key = 5*(keys[2]- '0' -1) + 2;
                    }
                    else if(keys[0] == 'G'){
                        key = 5*(keys[2]- '0' -1) + 3;
                    }
                    else{
                        key = 5*(keys[2]-'0'-1)+4;
                    }
                    blackKeys[key].setFillColor(sf::Color::Blue);
                    window.draw(blackKeys[key]);
                }
            }
            for(unsigned int i = 0; i < blackKeys.size(); i++){
                window.draw(blackKeys[i]);
            }
            window.display();
            auto remainingTime = sf::milliseconds(noteHold) - animationTimer.getElapsedTime();
            if (remainingTime > sf::Time::Zero) {
                sf::sleep(remainingTime);
            }
            animationTimer.restart();
            it++;
        }
        else{
            for(unsigned int i = 0; i < whiteKeys.size(); i++){
                whiteKeys[i].setFillColor(sf::Color::White);
                window.draw(whiteKeys[i]);
            }
            for(unsigned int i = 0; i < blackKeys.size(); i++){
                blackKeys[i].setFillColor(sf::Color::Black);
                window.draw(blackKeys[i]);
            }
            for(int i = 1; i < 56; i++){
                line.setPosition(SIDE_PADDING + i * WHITE_KEY_WIDTH, VERTICAL_PADDING);
                window.draw(line);
            }
        }
        window.display();
    }
    playMusic.join();
    sounds.clear();
}
void piano::readKeyPressed(ifstream& readFile, streampos& pos){
    if(musicDone || quit)
        return;
    readFile.seekg(pos);
    string input = "";
    notesToPlay.clear();
    while (getline(readFile, input)) {
        regex reg("^Measure.*");
         pos = readFile.tellg();
        if (!regex_match(input, reg)) {
            stringstream stream(input);
            string note, defX, length, accidental;
            stream >> note >> defX;
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
                notesToPlay[stoi(defX)].push_back(note);
             }
            pos = readFile.tellg();
        }
        else
            return;
    }
    musicDone = true;
}
void piano::playKey(sf::Vector2f& mousePos){
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

void piano::readFile(string name){
    tinyxml2::XMLDocument doc;
    string newName = "xmlInputs/" + name;
    const char* fileName = newName.c_str();

    doc.LoadFile(fileName);
    newName = name.substr(0, name.size()-4) + ".txt";
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
                XMLElement* rest = note->FirstChildElement("rest");
                if(printObject)
                    continue; 
                if (rest) {
                    int duration = 0;
                    XMLElement* dur = note->FirstChildElement("duration");
                    if (dur != nullptr) {
                        dur->QueryIntText(&duration);
                    }
                    outFile << "rest " << duration <<"\n";
                    continue;
                }   
                XMLElement* pitch = note->FirstChildElement("pitch");
                XMLElement* step = pitch->FirstChildElement("step");
                XMLElement* octave = pitch->FirstChildElement("octave");
                int duration = 0;
                XMLElement* alt = pitch->FirstChildElement("alter");
                XMLElement* dur = note->FirstChildElement("duration");
                int altCount = 0;
                if (dur != nullptr) {
                    dur->QueryIntText(&duration);
                }
                if(alt){
                    altCount = stoi(alt->GetText());
                }
                int defaultX = 0;
                note->QueryIntAttribute("default-x", &defaultX);
                outFile << step->GetText() << octave->GetText() << " " << defaultX << " " << duration  << " " << altCount << "\n";
            }
        }
    }
    outFile.close();
}