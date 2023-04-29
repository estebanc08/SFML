#include "piano.hpp"
#include "RoundedRectangle.hpp"
#define SCREEN_TIME 4.f
static unsigned int start = 0;

inline int whiteKeyIndex(std::string key);
inline int blackKeyIndex(std::string key);
inline void drawNotes(sf::RenderWindow& window, Piano& piano, sf::Clock& clock, std::vector<double>& whitePlaying, std::vector<double>& blackPlaying, sf::Text& songPlaying);