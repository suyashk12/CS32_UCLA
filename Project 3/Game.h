#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Side.h"
#include "Board.h"

// forward declaration for class Player for creating Player pointers as data members of the Game class.
class Player;

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
private:
    Board m_Board; // to keep track of what state the current Game Board is in.
    Player* m_South; // a pointer to South Player (instead of an actual player itself).
    Player* m_North; // a pointer to North Player (instead of an actual player itself).
    Side m_currSide; // to keep track of which Player is currently playing, to print appropriate messages within the Game::display() function.
};

#endif
