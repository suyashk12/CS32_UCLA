#ifndef SIDE_INCLUDED
#define SIDE_INCLUDED

enum Side { NORTH, SOUTH }; // enum used to determine a particular side in a Board object.

// some useful constants
const int NSIDES = 2;
const int POT = 0;

// provided inline function.
inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif
