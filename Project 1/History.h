#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

// declaration of the new History class, as per project spec.

class History
{
public:
    History(int nRows, int nCols);
    ~History();
    bool record(int r, int c);
    void display() const;
private:
    int ** m_state; // dynamically allocated 2D array to keep track of "state" of each point of the grid. State of a particular position within the grid is defined as the number of zombies that were killed by the player while standing at that particular position. The basic requirement to do so is to have m_state as a pointer to a pointer.
    
    // capturing nRows and nCols arguments into private member variables of History class's constructor for further use. 
    int m_nRows;
    int m_nCols;
    
};

#endif
