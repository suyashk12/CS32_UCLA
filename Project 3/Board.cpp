#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole): m_Holes(nHoles)
{
    // treat any improper values for nHoles or nInitialBeansPerHole appropriately as per the spec.
    if(nHoles<=0)
        m_Holes = 1;
    if(nInitialBeansPerHole<0)
        nInitialBeansPerHole = 0;
    
    // each side will have m_Holes+1 holes, one pot, and m_Holes ordinary holes.
    m_NorthSide = new int[m_Holes+1];
    m_SouthSide = new int[m_Holes+1];
    
    // as each pot has zero holes when the game begins.
    m_NorthSide[0] = 0;
    m_SouthSide[0] = 0;
    
    // setting the number of beans to begin with in each hole on each side to nInitialBeansPerHole.
    for(int i=1; i<m_Holes+1; i++)
    {
        m_NorthSide[i] = nInitialBeansPerHole;
        m_SouthSide[i] = nInitialBeansPerHole;
    }
}

// redefining Copy Constructor, Assignment Operator, and Destructor as we are working with dynamic memory.

// Copy Constructor.
Board::Board(const Board& otherBoard) : m_Holes(otherBoard.m_Holes)
{
    m_NorthSide = new int[m_Holes+1];
    m_SouthSide = new int[m_Holes+1];
    
    for(int i=0; i<m_Holes+1; i++)
    {
        m_NorthSide[i] = otherBoard.m_NorthSide[i];
        m_SouthSide[i] = otherBoard.m_SouthSide[i];
    }
}

// Assignment Operator overload.
Board& Board::operator=(const Board &otherBoard)
{
    if(this != &otherBoard)
    {
        m_Holes = otherBoard.m_Holes;
        
        delete [] m_NorthSide;
        delete [] m_SouthSide;
        
        m_NorthSide = new int[m_Holes+1];
        m_SouthSide = new int[m_Holes+1];
        
        for(int i=0; i<m_Holes+1; i++)
        {
            m_NorthSide[i] = otherBoard.m_NorthSide[i];
            m_SouthSide[i] = otherBoard.m_SouthSide[i];
        }
    }

    return(*this);
    
}

// Destructor.
Board::~Board()
{
    delete [] m_NorthSide;
    delete [] m_SouthSide;
}

// returns number of holes on either side.
int Board::holes() const
{
    return m_Holes;
}

// returns number of beans on either side.
int Board::beans(Side s, int hole) const
{
    // treats inappropriate values for hole as mentioned in spec.
    if(hole<0 || hole>m_Holes)
        return -1;
    else
    {
        if(s == Side::NORTH)
            return m_NorthSide[hole];
        else
            return m_SouthSide[hole];
    }
}

// returns the total number of beans in the holes (not the pot) of Side s. This is simply looping through the appropriate data member array.
int Board::beansInPlay(Side s) const
{
    int sideBns = 0;
    
    if(s == Side::NORTH)
    {
        for(int i=1; i<m_Holes+1; i++)
            sideBns+=m_NorthSide[i];
    }
    
    else
    {
        for(int i=1; i<m_Holes+1; i++)
            sideBns+=m_SouthSide[i];
    }
    
    return sideBns;
}

// sums up beans in each hole and each pot on the board. This also involves a simple array looping construct.
int Board::totalBeans() const
{
    int totalBns = 0;
    
    for(int i=0; i<m_Holes+1; i++)
        totalBns += (m_NorthSide[i] + m_SouthSide[i]);
    
    return totalBns;
}

// sows the beans in hole of Side s in a counter clockwise manner, putting a bean in its own pot (if the need arises) but not in its opponent's pot. The Board::sow() function doesn't account for captures or bean transfers after the game ends.
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    // any illegal values for hole are first detected. If hole is legal, it ensures that the indicated hole is non-empty.
    if(hole<=0 || hole>m_Holes || ((s == Side::NORTH) && (m_NorthSide[hole] == 0)) || ((s == Side::SOUTH) && (m_SouthSide[hole] == 0)))
        return false;
  
    else
    {
        // we begin sowing from the Side s, and here, trackSide will help us know which side we are currently on while sowing beans. Similarly, trackHole will keep track of which hole we're currently at in the process of sowing the beans. trackBeans will keep track of how many beans extracted from the appropriate hole are left over in the process of sowing.
        Side trackSide = s;
        int trackHole = hole;
        int trackBeans = 0;
        
        
        // here is what must be done if s is South.
        if(s == Side::SOUTH)
        {
            // store the number of beans in the hole into trackBeans, and set the number of beans in the hole to zero.
            trackBeans = m_SouthSide[hole];
            m_SouthSide[hole] = 0;
            
            // keep moving in the South side as long as you have beans left and as long as you don't hit the last hole (not pot), whichever condition exhausts first. Note that trackHole is appropriately updated so that where the move ended can be exactly found out from the value of trackHole.
            for(; trackHole<m_Holes && trackBeans>0; )
            {
                trackHole++;
                m_SouthSide[trackHole] += 1;
                trackBeans--;
            }
            
            // if one must have beans left after having hit the end of the South side, trackHole must be set to the South side's pot and a bean must be put inside the pot.
            if(trackBeans>0)
            {
                trackHole = 0;
                m_SouthSide[0] += 1;
                trackBeans--;
            }
            
            // if one still has beans left, simply switch to the North side and set trackHole to be the last hole in the North side.
            if(trackBeans>0)
            {
                trackSide = Side::NORTH;
                trackHole = m_Holes+1;
            }
            
            
            // keep rotating counter-clockwise now until beans remain.
            while(trackBeans>0)
            {
                // if beans still remain and the current side you're working on is North, begin from the last hole and keep putting beans as long as you hit the first hole of North (you can't use North's pot as you are playing from the South side) and have beans left.
                if(trackSide == Side::NORTH)
                {
                    // keep moving from North's last hole to first, putting one bean in each hole.
                    for(; trackHole>1 && trackBeans>0; )
                    {
                        trackHole--;
                        m_NorthSide[trackHole] += 1;
                        trackBeans--;
                    }
                    
                    // if you still have beans left, switch over to the first hole of South.
                    if(trackBeans>0)
                    {
                        trackSide = Side::SOUTH;
                        trackHole = 0;
                    }
                    
                }
                
                // if beans still remain and the current side you're working on is South, begin from the first hole of South and keep putting beans as long as you hit the last hole of North and have beans left. If need be, put a bean in the South pot.
                else
                {
                    // move from South's first hole to the last, sowing one bean in each hole.
                    for(; trackHole<m_Holes && trackBeans>0; )
                    {
                        trackHole++;
                        m_SouthSide[trackHole] += 1;
                        trackBeans--;
                    }
                    
                    // if there is still at least a bean left, sow it in the South pot.
                    if(trackBeans>0)
                    {
                        trackHole = 0;
                        m_SouthSide[0] += 1;
                        trackBeans--;
                    }
                    
                    // if there are still beans left, switch over to North's last hole.
                    if(trackBeans>0)
                    {
                        trackSide = Side::NORTH;
                        trackHole = m_Holes+1;
                    }
                    
                }
            }
        }
        
        // here is what must be done if s is North.
        else
        {
            // store the number of beans in the hole into trackBeans, and set the number of beans in the hole to zero.
            trackBeans = m_NorthSide[hole];
            m_NorthSide[hole] = 0;
            
            // move from the hole you're presently at in North's side towards the pot while you still have beans left. Sow a bean in each hole (or the pot, if it is encountered).
            for(; trackHole>0 && trackBeans>0; )
            {
                trackHole--;
                m_NorthSide[trackHole] += 1;
                trackBeans--;
            }
            
            // if there are still beans left over, switch over to the first hole on South's side.
            if(trackBeans>0)
            {
                trackSide = Side::SOUTH;
            }
            
            
            // keep rotating counter-clockwise while beans still remain.
            while(trackBeans>0)
            {
                // if you're currently working on the South side, traverse from the first hole to the last hole (not the pot, since you're not allowed to sow in South's pot being a North player).
                if(trackSide == Side::SOUTH)
                {
                    // move from the first hole of South to the last hole while you still have beans left, sowing a bean each in every hole.
                    for(; trackHole<m_Holes && trackBeans>0; )
                    {
                        trackHole++;
                        m_SouthSide[trackHole] += 1;
                        trackBeans--;
                    }
                    
                    // if you still have beans left, switch over to the last hole of the North side.
                    if(trackBeans>0)
                    {
                        trackSide = Side::NORTH;
                        trackHole = m_Holes+1;
                    }
                    
                }
                
                // if you're currently working on the North side, traverse from the last hole to the pot.
                else
                {
                    // move from the last hole to the pot (since you can sow in the North pot being a North player) on the North side. Sow a bean in each hole/ pot (if the need for the latter arises).
                    for(; trackHole>0 && trackBeans>0; )
                    {
                        trackHole--;
                        m_NorthSide[trackHole] += 1;
                        trackBeans--;
                    }
                    
                    // if you still have beans left over, switch to South's first hole.
                    if(trackBeans>0)
                    {
                        trackSide = Side::SOUTH;
                    }
                }
            }
        }
        
        // the way trackSide and trackHole were updated, they appropriately give where exactly the last bean was sowed, so we set endSide and endHole to these values respectively.
        endSide = trackSide;
        endHole = trackHole;
        
        return true;
    }
}

// treats illegal values for hole as specified in spec. If everything is legal, it determines what s and potOwner might be respecitvely, and appropriately increments the number of beans in the potOwner's side by the number of beans in the hole of s, setting the latter to 0.
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole<=0 || hole>m_Holes)
        return false;
    else
    {
        if(potOwner == Side::NORTH && s == Side::NORTH)
        {
            m_NorthSide[0] += m_NorthSide[hole];
            m_NorthSide[hole] = 0;
        }
        else if(potOwner == Side::NORTH && s == Side::SOUTH)
        {
            m_NorthSide[0] += m_SouthSide[hole];
            m_SouthSide[hole] = 0;
        }
        else if(potOwner == Side::SOUTH && s == Side::NORTH)
        {
            m_SouthSide[0] += m_NorthSide[hole];
            m_NorthSide[hole] = 0;
        }
        else
        {
            m_SouthSide[0] += m_SouthSide[hole];
            m_SouthSide[hole] = 0;
        }
        return true;
    }
}

// treats inappropriate values for hole as specified in the spec. If everything is legal, it simply references the appropriate item in the appropriate array and sets it to beans.
bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole<0 || hole>m_Holes || beans<0)
        return false;
    else
    {
        if(s==Side::NORTH)
            m_NorthSide[hole] = beans;
        else
            m_SouthSide[hole] = beans;
        return true;
    }
}
