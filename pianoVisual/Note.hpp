 
#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <SFML/Audio.hpp>

 class Note{
    protected:
        std::string key;
        double startTime;
        
    public:
        Note(std::string _key, double startTime) {
            this->key = _key;
            this->startTime = startTime;
        }

        std::string getKey(){
            return key;
        }
        double getStartTime(){
            return startTime;
        }
    };

#endif