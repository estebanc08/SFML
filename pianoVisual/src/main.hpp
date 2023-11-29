#include "piano.hpp"
#include "RoundedRectangle.hpp"
#include "myClock.hpp"
#include <cmath>
#define SCREEN_TIME 4.f
static unsigned int start = 0;
static int offset = 0;

inline int whiteKeyIndex(std::string key);
inline int blackKeyIndex(std::string key);
inline void drawNotes(sf::RenderWindow& window, Piano& piano, sf::myClock& clock, std::vector<double>& whitePlaying, std::vector<double>& blackPlaying, sf::Text& songPlaying);