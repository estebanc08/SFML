#ifndef SIXTEENTH_HPP
#define SIXTEENTH_HPP
#include "Note.hpp"

class Sixteenth : public Note{
    public:
    Sixteenth(std::string key,  int duration) : Note(key, duration){};

    std::string getType() override{
        return "Sixteenth";
    }

    std::string getKey() override{
        return key;
    }
};

#endif