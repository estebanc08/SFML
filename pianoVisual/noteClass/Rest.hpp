#ifndef REST_HPP
#define REST_HPP
#include "Note.hpp"

class Rest : public Note{
    public:
    Rest(int duration) : Note("", duration){};
    std::string getType() override{
        return "Rest";
    }

    std::string getKey() override{
        return key;
    }
};

#endif