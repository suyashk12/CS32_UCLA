#include <chrono>
#include <future>
#include <atomic>
#include "Player.h"
#include "Board.h"

// implementation of the AlarmClock class an instance of which will be used to apply time restrictions to SmartPlayer::chooseMove().
class AlarmClock
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

// a HumanPlayer can make any move which is legal (i.e. sow from a hole from its own side which is non-empty).
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    // the following check ensures that at least one hole on the HumanPlayer's side is non-empty for it to be able to make a legal move.
    bool flag = false;
    for(int i=1; i<b.holes()+1; i++)
    {
        if(b.beans(s, i) != 0)
        {
            flag = true;
            break;
        }
    }
    
    if(!flag)
        return -1;
    
    // if at least one legal move can be made, this is what can be done.
    else
    {
        // continually asking for a legal input from the HumanPlayer until they do not provide one.
        while(true)
        {
            std::cout << std::endl;
            std::cout << "Please enter a move: ";
            int hole = 0;
            std::cin >> hole;
            
            // checking whether the entered hole was non-empty using Board class's member function Board::beans(). If it was non-empty, the hole is simply returned.
            if(hole>0 && hole<=b.holes() && b.beans(s, hole)!=0)
            {
                std::cout << std::endl;
                return hole;
            }
            
            // if the entered move was not a legal one, the user will be prompted to provide a legal input once again.
            else
                std::cout << "Please enter a valid move." << std::endl;
        }
    }
}

// a BadPlayer simply makes a legal move which is closest to the left end of the Board (i.e. a hole is 1 or as close as possible to 1), if such a legal move can be made.
int BadPlayer::chooseMove(const Board& b, Side s) const
{
    // trying to find the very first non-empty hole which can be found equal to 1 or closest to it.
    for(int i=1; i<b.holes()+1; i++)
    {
        if(b.beans(s, i)!=0)
            return i;
    }
    
    // if no such hole was found, the game is now over and we must return -1.
    return -1;
}

// the chosen heuristics for the SmartPlayer::chooseMove() function is the difference between the South's pot and that of North (if the game hasn't been already won by one of the two players/ resulted in a tie).
int SmartPlayer::evaluate(const Board &b) const
{
    // only if both sides are non-empty, this function would return the difference in the number of beans in South's pot and North's pot.
    if(b.beansInPlay(Side::SOUTH)!=0 && b.beansInPlay(Side::NORTH)!=0)
        return(b.beans(Side::SOUTH, 0) - b.beans(Side::NORTH, 0));
    else
    {
        // if it is a tie, simply return 0.
        if(b.beans(Side::SOUTH, 0) == b.beans(Side::NORTH, 0))
            return 0;
        // if South won, return 999 (a highly positive number indicating that South won).
        else if(b.beans(Side::SOUTH, 0)>b.beans(Side::NORTH, 0))
            return 999;
        // if North won, return -999 (a highly negative number indicating that North won).
        else
            return -999;
    }
}

// this is a helper function which traverses a decision tree recursively to return the best hole choice possible according to the minimax algorithm.
int SmartPlayer::decide(const Board& b, Side s, int& value, int depth, AlarmClock& a) const
{
    // checking whether the game is already over or not.
    bool flag = false;
    for(int i=1; i<b.holes()+1; i++)
    {
        if(b.beans(s, i) != 0)
        {
            flag = true;
            break;
        }
    }
    
    // if the game is already over or the AlarmClock has timed out, set value to be the evaluate function called on the board b (as we are currently at decision tree's leaf.
    if(depth == 5 || a.timedOut() || !flag)
    {
        value = evaluate(b);
        return -1;
    }
    
    // if neither of the above two conditions are satisfied, we may continue traversing the decision tree.
    else
    {
        int besthole = -1; // to keep track of the best hole found so far while traversing a particular side. Initialized arbitrarily right now, but will be appropriately modified later.
        int bestvalue; // to keep track of the best hole value obtained from a depth of one layer beneath the current one in the decision tree.
        
        // since the minimax algorithm demands a preference of highly positive values for South, we set bestvalue to be an extremely negative number to begin with if we are traversing the South side of the board.This ensures that the first time a non-empty hole is found, it can be considered as the best hole found so far according to the selection criteria (since its value will be greater than this extremely negative integer here, and that is what we want for South).
        if(s==Side::SOUTH)
            bestvalue = -99999;
        // the exact opposite logic applies while selecting the inital value for bestvalue if traversing the North side currently.
        else
            bestvalue = 99999;
        
        // start traversing the board's Side s.
        for(int i=1; i<b.holes()+1; i++)
        {
            // if a non-empty hole was found, perform certain operations as follows.
            if(b.beans(s, i)!=0)
            {
                Board tempboard(b); // copy over the current board to deduce what moves can be made by making moves on the copy, without affecting the actual board.
                Side endSide;
                int endHole;
                tempboard.sow(s, i, endSide, endHole); // sow at i on the copied tempboard.
                
                // Perform a capture if possible (implementation details vary depending on whether s is South or North).
                if(s==Side::SOUTH && endHole!=0 && tempboard.beans(Side::SOUTH, endHole)==1 && tempboard.beans(Side::NORTH, endHole)!=0)
                {
                    tempboard.moveToPot(Side::NORTH, endHole, Side::SOUTH);
                    tempboard.moveToPot(Side::SOUTH, endHole, Side::SOUTH);
                }
                
                else if(s==Side::NORTH && endHole!=0 && tempboard.beans(Side::NORTH, endHole)==1 && tempboard.beans(Side::SOUTH, endHole)!=0)
                {
                    tempboard.moveToPot(Side::SOUTH, endHole, Side::NORTH);
                    tempboard.moveToPot(Side::NORTH, endHole, Side::NORTH);
                }
                
                // Check if the game got over, and transfer any beans left over on the board into their respective pots (implementation details vary depending on whether South became empty or North). And if the game is over in favor of the current Player, set besthole to this hole and value to the appropriate winning value.
                else if(tempboard.beansInPlay(Side::SOUTH) == 0)
                {
                    for(int i=1; i<tempboard.holes()+1; i++)
                        tempboard.moveToPot(Side::NORTH, i, Side::NORTH);
                    break;
                }
                
                else if(tempboard.beansInPlay(Side::NORTH) == 0)
                {
                    for(int i=1; i<tempboard.holes()+1; i++)
                        tempboard.moveToPot(Side::SOUTH, i, Side::SOUTH);
                    break;
                }
                
                // if you happen to end at your own pot, call decide at the same depth for yourself, since you get another move, and multiple moves are technically considered to be at the same depth. This recursive call will alter value appropriately.
                if(endSide == s && endHole == 0)
                    decide(tempboard, s, value, depth, a);

                // if you didn't end up in your own pot, the next turn will be the opponent's. Since figuring out your best chances needs you to assume that your opponent is going to perform optimally, call decide for your opponent at a depth one greater than yours (since successive moves are one layer beneath each other). The executed recursive call will alter value appropriately.
                else
                {
                    if(s==Side::SOUTH)
                        decide(tempboard, Side::NORTH, value, depth+1, a);
                    else
                        decide(tempboard, Side::SOUTH, value, depth+1, a);
                }
                
                // if the current state of value is better than what has been encountered so far along the board (the meaning of better varies depending whether the current player is South or North).
                if((s == Side::SOUTH && value>=bestvalue) || (s == Side::NORTH && value<=bestvalue))
                {
                    bestvalue = value; // bestvalue will hold the best state of value found so far.
                    besthole = i; // i would be the corresponding besthole.
                }
            }
        }
        value = bestvalue; // set value to bestvalue which recorded what best state value had attained in a chain of recursive calls.
        return besthole; // return besthole which had recorded the hole number corresponding to the bestvalue.
    }
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    // setting up an AlarmClock that goes off at 4900 ms, leaving 100 ms for wrapping up everything.
    AlarmClock ac(4900);
    int value;
    // making the SmartPlayer::decide() helper function return the best move. 
    return decide(b, s, value, 0, ac);
}

