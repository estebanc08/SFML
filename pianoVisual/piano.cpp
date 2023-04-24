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


void Piano::readMidi(const std::string path){
    smf::MidiFile midifile;
    if (!midifile.read(path)) {
        std::cerr << "Failed to read MIDI file: " << path << std::endl;
        return;
    }

    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    for (int track_num = 0; track_num < midifile.getTrackCount(); track_num++) {
        smf::MidiEventList track = midifile[track_num];
        for (int i = 0; i < track.size(); i++) {
            const smf::MidiEvent& event = track[i];
           if (event.isNoteOn()) {
                int pitch = event.getP1();
                int velocity = event.getP2();
                pitch -= 21;
                int noteNum;
                std::string key;
                switch (pitch % 12) {
                    case 0: key = "A"  + std::to_string((pitch / 12)); 
                    break; 
                    case 1: key = "Bb" + std::to_string((pitch / 12));
                    break; 
                    case 2: key = "B" + std::to_string((pitch / 12)); 
                    break; 
                    case 3: key = "C" + std::to_string((pitch / 12) + 1); 
                    break; 
                    case 4: key = "Db" + std::to_string((pitch / 12) + 1);
                    break; 
                    case 5: key = "D" + std::to_string((pitch / 12) + 1); 
                    break;
                    case 6: key = "Eb" + std::to_string((pitch / 12) + 1);
                    break;
                    case 7: key = "E" + std::to_string((pitch / 12) + 1); 
                    break;
                    case 8: key = "F" + std::to_string((pitch / 12) + 1); 
                    break;
                    case 9: key = "Gb" + std::to_string((pitch / 12) + 1);
                    break;
                    case 10: key = "G" + std::to_string((pitch / 12) + 1); 
                    break;
                    case 11: key = "Ab" + std::to_string((pitch / 12 + 1));
                    break;
                }
                Note* note = new Note(key, event.seconds);
                notes.push_back(note);
            }
        }
    }
    std::sort(notes.begin(), notes.end(), [](Note* a, Note* b){
            return a->getStartTime() < b->getStartTime();
    });
}