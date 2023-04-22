 
#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <SFML/Audio.hpp>

 class Note{
    protected:
        std::string key;
        int duration;
        sf::Clock soundTimer;
        
    public:
        Note(std::string _key, int _duration) {
            this->key = _key;
            this->duration = _duration;
        }
        virtual std::string getType() = 0;
        virtual std::string getKey() = 0;
    };

#endif