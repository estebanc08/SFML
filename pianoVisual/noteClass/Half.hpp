#ifndef HALF_HPP
#define HALF_HPP
#include "Note.hpp"

class Half : public Note{
    public:
    Half(std::string key,  int duration) : Note(key, duration){};

    std::string getType() override{
        return "Half";
    }
    std::string getKey() override{
        return key;
    }
};

#endif