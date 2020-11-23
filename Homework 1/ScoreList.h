#ifndef SCORELIST_INCLUDED
#define SCORELIST_INCLUDED

#include "Sequence.h"
#include <limits>

const unsigned long NO_SCORE = std::numeric_limits<unsigned long>::max();

class ScoreList
{
public:
    ScoreList();
    bool add(unsigned long score);
    bool remove(unsigned long score);
    int size() const;
    unsigned long minimum() const;
    unsigned long maximum() const;

private:
    Sequence m_list;
};


#endif
