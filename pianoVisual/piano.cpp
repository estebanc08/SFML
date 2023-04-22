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

void Piano::readKeysPressed(std::ifstream& readFile){
    std::string input = "";
    //read first measure
    getline(readFile, input);
    std::map<int, std::vector<Note*>> notesPerMeasure;
    while (getline(readFile, input)) {
        std::regex reg("^Measure.*");
        if (!regex_match(input, reg)) {
            std::stringstream stream(input);
            std::string note, posX, duration;
            stream >> note;
            if(note != "rest")
                stream >> posX;
            stream >> duration;
            std::transform(duration.begin(), duration.end(), duration.begin(), [](unsigned char c){ return std::tolower(c); });
            if(note == "rest"){
                stream >> duration;
                Rest currNote(noteHold);
            }
            else{
                //convert to flat note if sharp or flat

                if(duration == "sixteenth" || duration == "16th"){
                    Sixteenth* currNote = new Sixteenth(note, noteHold);
                    notesPerMeasure[std::stoi(posX)].push_back(currNote);
                }
                else if(duration == "eighth" || duration == "8th"){
                    Eighth* currNote = new Eighth(note, noteHold);
                    notesPerMeasure[std::stoi(posX)].push_back(currNote);
                }
                else if(duration == "quarter"){
                    Quarter* currNote = new Quarter(note, noteHold);
                    notesPerMeasure[std::stoi(posX)].push_back(currNote);
                }
                else if(duration == "half"){
                    Half* currNote = new Half(note, noteHold);
                    notesPerMeasure[std::stoi(posX)].push_back(currNote);
                }
                else if(duration == "full"){
                    Full* currNote = new Full(note, noteHold);
                    notesPerMeasure[std::stoi(posX)].push_back(currNote);
                }
            }
            // else{
            //    Rest currNote(duration);
            //    notesPerMeasure[std::stoi(posX)].push_back(currNote);
            // }
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
            std::vector<Note*> temp;
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
            std::cout << "black clicked" << std::endl;
            return;
        }
    }
    for(unsigned int i = 0; i < whiteKeys.size(); i++){
        if(whiteKeys[i].getGlobalBounds().contains(mousePos)){
            std::cout << "white clicked" << std::endl;
            return;
        }
    }
}

void Piano::readFile(std::string name){
    tinyxml2::XMLDocument doc;
    const char* fileName = name.c_str();

    doc.LoadFile(fileName);
    std::size_t lastSlash = name.find_last_of('/');
    std::string newName = name.substr(lastSlash + 1);
    newName = newName.substr(0, newName.size() - 4) + ".txt";
    currFileName  = "sheetMusic/" + newName;
    std::ofstream outFile(currFileName);
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
                    tinyxml2::XMLElement* duration = note->FirstChildElement("type");
                    if (duration == nullptr) {
                        duration = note->FirstChildElement("duration");
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
                    outFile << "rest " << duration->GetText() <<"\n";
                    continue;
                }   
                tinyxml2::XMLElement* pitch = note->FirstChildElement("pitch");
                tinyxml2::XMLElement* step = pitch->FirstChildElement("step");
                tinyxml2::XMLElement* octave = pitch->FirstChildElement("octave");
                tinyxml2::XMLElement* alt = pitch->FirstChildElement("alter");
                tinyxml2::XMLElement* type = note->FirstChildElement("type");
                std::string noteType = step->GetText();
                noteType += octave->GetText();
                int altCount = 0;
                if(alt){
                    altCount = std::stoi(alt->GetText());
                    if(altCount == -1){
                        noteType = noteType.substr(0,1) + "b" + noteType.substr(1);
                    }
                    else if(altCount == 1){
                        char toFlat = noteType[0];
                        toFlat += 1;
                        int octave = noteType[1]-'0';
                        if(toFlat > 'G'){
                            toFlat = 'A';
                            octave++;
                        }
                        noteType = std::string(1, toFlat) + "b" + std::to_string(octave);
                    }
                }
                int defaultX = 0;
                note->QueryIntAttribute("default-x", &defaultX);
                outFile << noteType << " " << defaultX << " " << type->GetText()  << "\n";
            }
        }
    }
    outFile.close();
}