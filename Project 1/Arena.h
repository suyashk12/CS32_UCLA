#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED

#include "globals.h"
#include "History.h"

class Player;
class Zombie;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     zombieCount() const;
    int     numZombiesAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    
    // Mutators
    bool   addZombie(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackZombieAt(int r, int c, int dir);
    bool   moveZombies();
    
    History& history(); // accessor to access newly declared m_history private member variable.
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Zombie* m_zombies[MAXZOMBIES];
    int     m_nZombies;
    History m_history; // newly added member variable of type History. The m_state member variable of m_history will be accordingly updated whenever the player kills a zombie from a particular position. This will be done under the attackZombieAt() function of Arena class. 
};


#endif
