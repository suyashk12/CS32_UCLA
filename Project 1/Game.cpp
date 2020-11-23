#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

#include "Game.h"
#include "Arena.h"
#include "Player.h"
#include "History.h"
#include "globals.h"

int decodeDirection(char dir)
{
    switch (dir)
    {
        case 'u':  return UP;
        case 'd':  return DOWN;
        case 'l':  return LEFT;
        case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

Game::Game(int rows, int cols, int nZombies)
{
    if (nZombies < 0)
    {
        cout << "***** Cannot create Game with negative number of zombies!" << endl;
        exit(1);
    }
    if (nZombies > MAXZOMBIES)
    {
        cout << "***** Trying to create Game with " << nZombies
        << " zombies; only " << MAXZOMBIES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nZombies > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the zombies!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with zombies
    while (nZombies > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a zombie where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_arena->addZombie(r, c);
        nZombies--;
    }
}

Game::~Game()
{
    delete m_arena;
}

void Game::play()
{
    m_arena->display();
    Player* p = m_arena->player();
    if (p == nullptr)
        return;
    while ( ! m_arena->player()->isDead()  &&  m_arena->zombieCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r/h/q): "; // added the new option of displaying history by entering the letter 'h'
        string action;
        getline(cin,action);
        if(action[0] == 'h')  // handling history request separately to ensure that it isn't counted as a player move.
        {
            m_arena->history().display(); // dereferencing m_arena member variable to call its history() function, which returns a reference to a History object, over which we call the display() function to display the grid representing an alphabetized key for how many zombies have been killed at each position in the grid.
            cout << "Press enter to continue."; // we print the required message.
            cin.ignore(10000,'\n'); // prompts and waits for response from the user.
            m_arena->display(); // to continue the game normally and give the player a chance to choose a desired action.
        }
        else
        {
            if (action.size() == 0)  // player stands
                p->stand();
            else
            {
                switch (action[0])
                {
                    default:   // if bad move, nobody moves
                        cout << '\a' << endl;  // beep
                        continue;
                    case 'q':
                        return;
                    case 'u':
                    case 'd':
                    case 'l':
                    case 'r':
                        p->moveOrAttack(decodeDirection(action[0]));
                        break;
                }
            }
            m_arena->moveZombies();
            m_arena->display();
        }
    }
}
