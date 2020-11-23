#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include "Side.h"

class Board
{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    Board(const Board& otherBoard);
    Board& operator=(const Board& otherBoard);
    ~Board();
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    // storing nHoles parameter of constructor for later use.
    int m_Holes;
    // using two dynamic arrays, one for each side, as the data structure to implement Board and its various functionalities.
    int* m_NorthSide;
    int* m_SouthSide;
};


#endif
