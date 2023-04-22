#ifndef QUARTER_HPP
#define QUARTER_HPP
#include "Note.hpp"

class Quarter : public Note{
    public:
    Quarter(std::string key,  int duration) : Note(key, duration){};
    
    std::string getType() override{
        return "Quarter";
    }

    std::string getKey() override{
        return key;
    }
};

#endif