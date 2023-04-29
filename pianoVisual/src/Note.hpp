 
#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <SFML/Audio.hpp>

 class Note{
    public:
        Note(std::string _key, double startTime) {
            this->key = _key;
            this->startTime = startTime;
        }

        Note(std::string _key, double startTime, double _durationInSeconds) {
            this->key = _key;
            this->startTime = startTime;
            this->durationInSeconds = _durationInSeconds;
        }

        std::string getKey(){
            return key;
        }
        double getStartTime(){
            return startTime;
        }

        double getDurationInSeconds(){
            return durationInSeconds;
        }
        
    private:
        std::string key;
        double startTime;
        double durationInSeconds;
    };

#endif