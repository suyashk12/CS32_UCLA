#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include <iostream>
#include "Side.h"
#include "Board.h"

// forward declaration for AlarmClock to satisfy reference to AlarmClock included as a parameter for decide(), a helper function for the SmartPlayer class.
class AlarmClock;

// Player is an abstract class due to the chooseMove() function being pure virtual as the latter will be implemented uniquiely by each class deriving from Player.
class Player
{
public:
    Player(std::string name) : m_name(name)
    {}
    std::string name() const // does not need to be overridden by any derived classes, so doesn't need to be virtual.
    {
        return m_name;
    }
    // isInteractive() is set to virtual with this default implementation as most Players are not interactive. However if a certain kind of Player needs to be interactive, it can easily override this funciton implementation with its own and return true instead. This facilitates code reuse.
    virtual bool isInteractive() const
    {
        return false;
    }
    virtual int chooseMove(const Board& b, Side s) const = 0; // a pure virtual function to be implemented uniquely by each derived class.
    virtual ~Player() // as a good practice, we declare the virtual destructor for Player, which is a base class.
    {}
private:
    std::string m_name; // chosen as a private member as it is common to all kinds of Players.
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name) : Player(name) // constructing the base part of HumanPlayer and doing nothing else, since a HumanPlayer just has a name, nothing special.
    {}
    virtual bool isInteractive() const // overriding Player's isInteractive() to return true as a HumanPlayer is interactive.
    {
        return true;
    }
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name) : Player(name) // constructing the base part of BadPlayer and doing nothing else, since a BadPlayer just has a name, nothing special.
    {}
    virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name) : Player(name) // constructing the base part of SmartPlayer and doing nothing else, as it has no special attribute except the name.
    {}
    virtual int chooseMove(const Board& b, Side s) const; // will require help from SmartPlayer::evaluate() and SmartPlayer::decide() functions.
private:
    int evaluate(const Board& b) const; // for returning value of leaves of the decision tree.
    int decide(const Board&b, Side s, int& value, int depth, AlarmClock& a) const; // a helper function which performs the actual work on behalf of SmartPlayer::chooseMove() function.
};

#endif
