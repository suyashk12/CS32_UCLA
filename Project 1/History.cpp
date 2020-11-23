#include <iostream>
using namespace std;

#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols)
{
    // retaining arguments of the constructor in separate private member variables for use in the destructor and the display function.
    
    m_nRows = nRows;
    m_nCols = nCols;
    
    // dynamically allocating a 2-D array having rows equal to nRows and columns equal to nCols.
    
    m_state = new int * [nRows]; // as m_state is a pointer to a pointer, we initialize it to a dynamic array of pointers (of integer type). The size of this array is nRows.
    
    for(int i=0; i<nRows; i++)
    {
        m_state[i] = new int [nCols]; // now for each pointer in the dynamic array pointed to at by m_state, we initialize the former to be a dynamic array of integers of size nCols.
    }
    
    // this completes the declaration of a 2-D array of size nRows by nCols in the heap memory.
    
    for(int i=0; i<nRows; i++)
    {
        for(int j=0; j<nCols; j++)
        {
            m_state[i][j] = 0; // we simply initialize each element in m_state to zero, since initially, each spot in the gameplay space has experienced zero kills by the player.
        }
    }
}

History::~History() // using dynamic allocation of memory in the heap makes it neccessary to have a destructor in order to prevent memory leak.
{
    // firstly, we delete each pointer in the dynamic array of pointers pointed to at by m_state via looping through m_state, and its size was provided within. This is why we needed to retain nRows and nCols arguments of History class's constructor to utilize it for the destructor.
    
    for(int i=0; i<m_nRows; i++)
    {
        delete [] m_state[i];
    }
    
    // finally delete m_state to free memory in the heap.
    
    delete [] m_state;
}


bool History::record(int r, int c)
{
    // all we do in the record function is perform bound checks, which if passed, results in the updation the state value of the particular player position in m_state (which is a 2-D array of integers).
    
    if (r <= 0  ||  c <= 0  ||  r > m_nRows  ||  c > m_nCols)
    {
        return(false);
    }
    else
    {
        m_state[r-1][c-1]+= 1; // since r and c will be given to record in the form of the coordinate system of the gameplay grid, which has rows starting from 1 and columns starting from 1 as well, we need to use m_state[r-1][c-1] to ensure that the updation takes place corresponding to the right position.
        return(true);
    }
    
    // we are not performing checks for whether the player killed a zombie or not in here, we simply update m_state according to r and c which will be given by the caller of the function. This function will be called in the Arena class's attackZombieAt() function, where just before a zombie is killed, we will update m_state member variable of Arena's m_history member variable.
}

void History::display() const
{
    // Position (row,col) of the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    
    // For efficient memory utilization, we also dynamically allocate a 2-D array of characters to grid, which is why we declare it as a pointer to a pointer, and go on to declare a 2-D array of size nRows by nCols (provided in History class's constructor and stored in the member variables m_nRows and m_nCols) as we did for m_state in the constructor.
    
    char** grid;
    
    grid = new char* [m_nRows]; // setting grid to be a dynamic array of pointers (of character type), the size of this dynamic array being m_nRows.
    
    // next, we loop through grid and set each element to be a dynamic array of characters, each of size m_nCols.
    for(int i=0; i<m_nRows; i++)
    {
        grid[i] = new char[m_nCols];
    }
    
    // thus, we have grid, a 2-D array of characters ready for utilization in display. It is important to note that grid must be deleted at the end of the display() function so as to prevent memory leakage in the heap.
    
    // We fill only the relevant positions of the grid with '.'s (if the player has made zero kills from that position), 'A's for one kill, 'B's for to, and so on, until 'Z' for 26 or greater kills. 
    for (int r = 0; r < m_nRows; r++)
    {
        for (int c = 0; c < m_nCols; c++)
        {
            
            int state = m_state[r][c]; // We compare each position in grid to m_state to decide the required character that must occupy the relevant position in grid, by using a switch case.
            
            switch(state){
                case 0:
                    grid[r][c] = '.';
                    break;
                case 1:
                    grid[r][c] = 'A';
                    break;
                case 2:
                    grid[r][c] = 'B';
                    break;
                case 3:
                    grid[r][c] = 'C';
                    break;
                case 4:
                    grid[r][c] = 'D';
                    break;
                case 5:
                    grid[r][c] = 'E';
                    break;
                case 6:
                    grid[r][c] = 'F';
                    break;
                case 7:
                    grid[r][c] = 'G';
                    break;
                case 8:
                    grid[r][c] = 'H';
                    break;
                case 9:
                    grid[r][c] = 'I';
                    break;
                case 10:
                    grid[r][c] = 'J';
                    break;
                case 11:
                    grid[r][c] = 'K';
                    break;
                case 12:
                    grid[r][c] = 'L';
                    break;
                case 13:
                    grid[r][c] = 'M';
                    break;
                case 14:
                    grid[r][c] = 'N';
                    break;
                case 15:
                    grid[r][c] = '0';
                    break;
                case 16:
                    grid[r][c] = 'P';
                    break;
                case 17:
                    grid[r][c] = 'Q';
                    break;
                case 18:
                    grid[r][c] = 'R';
                    break;
                case 19:
                    grid[r][c] = 'S';
                    break;
                case 20:
                    grid[r][c] = 'T';
                    break;
                case 21:
                    grid[r][c] = 'U';
                    break;
                case 22:
                    grid[r][c] = 'V';
                    break;
                case 23:
                    grid[r][c] = 'W';
                    break;
                case 24:
                    grid[r][c] = 'X';
                    break;
                case 25:
                    grid[r][c] = 'Y';
                    break;
                default: // corresponding to greater than equal to 26 kills. Note that state will always be a number greater than equal to zero, because of how we have initialized m_state in the constructor and how record() function can only increment a particular element in it by 1.
                    grid[r][c] = 'Z';
                    break;
            }
        }
        
    }
    
    // First clear the screen using clearScreen(), declared globals.h and implemented in utilities.cpp, and then draw the grid
    clearScreen();
    for (int r = 0; r < m_nRows; r++)
    {
        for (int c = 0; c < m_nCols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
    // now that we are done displaying grid, we can delete it safely to free memory space in heap.
    
    // we first loop through grid and delete each character type pointer in grid.
    for(int i=0; i<m_nRows; i++)
    {
        delete [] grid[i];
    }
    
    delete [] grid; // finally, we delete grid itself. 
    
}



