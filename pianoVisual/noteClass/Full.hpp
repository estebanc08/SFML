#ifndef FULL_HPP
#define FULL_HPP
#include "Note.hpp"

class Full : public Note{
    public:
    Full(std::string key,  int duration) : Note(key, duration){};

    std::string getType() override{
        return "Full";
    }
    
    std::string getKey() override{
        return key;
    }
};

#endif