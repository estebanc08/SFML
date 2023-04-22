#ifndef EIGHTH_HPP
#define EIGHTH_HPP
#include "Note.hpp"

class Eighth : public Note{
    public:
    Eighth(std::string key, int duration) : Note(key, duration){};
    
    std::string getType() override{
        return "Eighth";
    }
    std::string getKey() override{
        return key;
    }
};

#endif