#include "Game.h"
#include "Player.h"
#include <iostream>

// constructing all data members within the Game class. Note that m_currSide is initialized to Side::SOUTH because South must always make the first move.
Game::Game(const Board& b, Player* south, Player* north) : m_Board(b), m_South(south), m_North(north), m_currSide(Side::SOUTH)
{}

// the display function takes care of how the interface of the game should look like. The function is generalized to display the board properly for any number of holes, not just 6. The basic way that the display works is that it first displays the current state of the board, and informs whose turn it which might affect the current state of the board. Such a display rubric gives a clear idea of what options this particular player has by displaying the current state of the board which the player may change to its liking with the move it has to make.
void Game::display() const
{
    // to be arguments of the Game::status() function.
    bool isOver = false;
    bool hasWinner = false;
    Side winSide;

    // print North's name centered at the board. The upper bound is just the number of spaces that must come before the North side's name in ordered to keep it perfectly (for odd number of characters in North's name) or almost (for even number of characters in North's name) centered to the board.
    for(int i=0; i<(m_Board.holes() + 3/2 - m_North->name().length()/2); i++)
        std::cout << " ";
    std::cout << m_North->name() << std::endl;
    
    // print the North side's holes with their appropriate bean count. The holes are single spaced.
    std::cout << "  ";
    for(int i=1; i<=m_Board.holes(); i++)
        std::cout << m_Board.beans(Side::NORTH, i) << " ";
    
    std::cout << std::endl;
    
    // print the North and South pots with their bean counts at the extreme left and extreme right of the Board display. The number of spaces (which explains the upper bound for the loop below) can once again be figured out using simple arithmetic.
    
    std::cout << m_Board.beans(Side::NORTH, 0) << " ";
    
    for(int i=0; i<2*m_Board.holes(); i++)
        std::cout << " ";
    
    std::cout << m_Board.beans(Side::SOUTH, 0);
    
    std::cout << std::endl;
    
    // print the South side's holes with their respective bean counts, all single spaced.
    std::cout << "  ";
    for(int i=1; i<=m_Board.holes(); i++)
        std::cout << m_Board.beans(Side::SOUTH, i) << " ";
    
    std::cout << std::endl;
    
    // print the South player's name perfectly or almost centered with resepct to the Board display.
    for(int i=0; i<(m_Board.holes() + 3/2 - m_South->name().length()/2); i++)
        std::cout << " ";
    std::cout << m_South->name() << std::endl;
    std::cout << std::endl;
    
    // check whether or not the game is already over.
    status(isOver, hasWinner, winSide);
    
    // if the game isn't already over, print whose turn it is next.
    if(!isOver)
    {
        // depending on the value of m_currSide, print the appropriate message.
        if(m_currSide == SOUTH)
            std::cout << "It is " << m_South->name() << "'s turn!" << std::endl << std::endl;
        else
            std::cout << "It is " << m_North->name() << "'s turn!" << std::endl << std::endl;
    }
    
    // if the game is over, inform the user that the game is indeed over.
    else
        std::cout << "Game Over!" << std::endl << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    // if either of the two sides has zero beans in play, it means that the game is over.
    if(m_Board.beansInPlay(Side::SOUTH) == 0 || m_Board.beansInPlay(Side::NORTH) == 0)
    {
        over = true; // over is set to true as the game is certainly over now.
        
        // if the number of beans on each side's pot are equal, it means that the game is a tie.
        if(m_Board.beans(Side::NORTH, 0) == m_Board.beans(Side::SOUTH, 0))
            hasWinner = false; // we thus set hasWinner to false, as a time means that there is no winner, although the game is over.
        
        else
        {
            // if it wasn't a tie, it means that there certainly has to be a winner.
            hasWinner = true;
            // we set winner to the side the pot of which has a greater number of beans.
            if(m_Board.beans(Side::NORTH, 0) > m_Board.beans(Side::SOUTH, 0))
                winner = Side::NORTH;
            else
                winner = Side::SOUTH;
        }
    }
    
    // if both of the sides have a non-zero count for beans in play, the game isn't yet over, so we set over to false and don't affect hasWinner and winner.
    else
        over = false;
}

bool Game::move()
{
    // to be arguments for the Game::status() function.
    bool isOver = false;
    bool winnerExists = false;
    Side winSide;
    
    status(isOver, winnerExists, winSide); // determine the status of the game.
    
    // if the game is already over, make the function return false because a move cannot be made.
    if(isOver)
        return false;
    
    // keep making moves as long as the game isn't over. 
    while(true)
    {
        // if South is currently playing.
        if(m_currSide == Side::SOUTH)
        {
            // read an appropriate hole to make a move on by calling *m_South's chooseMove() function and perform a sow on m_Board using this hole.
            int hole = m_South->chooseMove(m_Board, Side::SOUTH);
            m_Board.sow(Side::SOUTH, hole, m_currSide, hole);
            
            // if the side you sowed your last bean was on the North, then there is no chance for making another move or shifting beans in any way.
            if(m_currSide == Side::NORTH)
            {
                // if the game is now over and the South side is empty, move North's beans over to its pot and return true because you made the move.
                if(m_Board.beansInPlay(Side::SOUTH) == 0)
                {
                    for(int i=1; i<m_Board.holes()+1; i++)
                        m_Board.moveToPot(Side::NORTH, i, Side::NORTH);
                    return true;
                }
                
                // if the game is now over and the North side is empty, move South's beans over to its pot and return true because you made the move.
                else if(m_Board.beansInPlay(Side::NORTH) == 0)
                {
                    for(int i=1; i<m_Board.holes()+1; i++)
                        m_Board.moveToPot(Side::SOUTH, i, Side::SOUTH);
                    return true;
                }
                
                // if the game didn't end, simply return true because you successfully made a move.
                return true;
            }
            
            // if however you ended on the same side that you began with, additional steps might need to be performed before the current turn is ended.
            else if(m_currSide == Side::SOUTH)
            {
                // if you didn't end up in a pot or a hole from which capture can be initiated, simply switch m_currSide to the opponent's and return true because the current move was successfully made.
                if(hole!=0 && m_Board.beans(Side::SOUTH, hole)!=1)
                {
                    m_currSide = Side::NORTH;
                    return true;
                }
                
                // if you end up in a hole from which you might be able to capture, do so, and after checking whether or not the game is now over, since you affected the beans in play further, switch the turn over to the next player and return true.
                else if(hole!=0 && m_Board.beans(Side::SOUTH, hole)==1)
                {
                    if(m_Board.beans(Side::NORTH, hole)>0)
                    {
                        m_Board.moveToPot(Side::NORTH, hole, Side::SOUTH);
                        m_Board.moveToPot(Side::SOUTH, hole, Side::SOUTH);
                    }
                    
                    // if the game is now over and South's side is empty, move North's beans over to its pot and return true since the current move was successfully made.
                    if(m_Board.beansInPlay(Side::SOUTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::NORTH, i, Side::NORTH);
                        return true;
                    }
                    
                    // if the game is now over and North's side is empty, move South's beans over to its pot and return true since the current move was successfully made.
                    else if(m_Board.beansInPlay(Side::NORTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::SOUTH, i, Side::SOUTH);
                        return true;
                    }
                    
                    // switch over the turn to the opponent because the game isn't yet over and return true even if the game didn't end because the current move was successfully performed.
                    m_currSide = Side::NORTH;
                    return true;
                }
                
                // if you end up in South's pot, take another move if the game isn't already over.
                else
                {
                    // if the game is now over and South's side is empty, move North's beans over to its pot and return true since the current move was successfully made.
                    if(m_Board.beansInPlay(Side::SOUTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::NORTH, i, Side::NORTH);
                        return true;
                    }
                    
                    // if the game is now over and North's side is empty, move South's beans over to its pot and return true since the current move was successfully made.
                    else if(m_Board.beansInPlay(Side::NORTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::SOUTH, i, Side::SOUTH);
                        return true;
                    }
                    display(); // display to show that the player took another turn.
                    continue; // have the current player take another turn.
                }
            }
        }
        
        // if North is currently playing.
        else
        {
            // read an appropriate hole to make a move on by calling *m_North's chooseMove() function and perform a sow on m_Board using this hole.
            int hole = m_North->chooseMove(m_Board, Side::NORTH);
            m_Board.sow(Side::NORTH, hole, m_currSide, hole);
            
            // if the side you sowed your last bean was on the South, then there is no chance for making another move or shifting beans in any way.
            if(m_currSide == Side::SOUTH)
            {
                // if the game is now over and North's side is empty, move South's beans over to its pot and return true since the current move was successfully made.
                if(m_Board.beansInPlay(Side::NORTH) == 0)
                {
                    for(int i=1; i<m_Board.holes()+1; i++)
                        m_Board.moveToPot(Side::SOUTH, i, Side::SOUTH);
                    return true;
                }
                
                // if the game is now over and South's side is empty, move North's beans over to its pot and return true since the current move was successfully made.
                else if(m_Board.beansInPlay(Side::SOUTH) == 0)
                {
                    for(int i=1; i<m_Board.holes()+1; i++)
                        m_Board.moveToPot(Side::NORTH, i, Side::NORTH);
                    return true;
                }
                
                // return true even if the game didn't end because the current move was successfully performed.
                return true;
            }
            
            
            // if however you ended on the same side that you began with, additional steps might need to be performed before the current turn is ended.
            else
            {
                // if you didn't end up in a pot or a hole from which capture can be initiated, simply switch m_currSide to the opponent's and return true because the current move was successfully made.
                if(hole!=0 && m_Board.beans(Side::NORTH, hole)!=1)
                {
                    m_currSide = Side::SOUTH;
                    return true;
                }
                
                
                // if you end up in a hole from which you might be able to capture, do so, and after checking whether or not the game is now over, since you affected the beans in play further, switch the turn over to the next player and return true.
                else if(hole!=0 && m_Board.beans(Side::NORTH, hole)==1)
                {
                    // the opposite side's hole has positive number of beans, so perform a capture, else don't move beans.
                    if(m_Board.beans(Side::SOUTH, hole)>0)
                    {
                        m_Board.moveToPot(Side::SOUTH, hole, Side::NORTH);
                        m_Board.moveToPot(Side::NORTH, hole, Side::NORTH);
                    }
                    
                    // if the game is now over and North's side is empty, move South's beans over to its pot and return true since the current move was successfully made.
                    if(m_Board.beansInPlay(Side::NORTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::SOUTH, i, Side::SOUTH);
                        return true;
                    }
                    
                    // if the game is now over and South's side is empty, move North's beans over to its pot and return true since the current move was successfully made.
                    else if(m_Board.beansInPlay(Side::SOUTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::NORTH, i, Side::NORTH);
                        return true;
                    }
                    
                    // switch over the turn to the opponent because the game isn't yet over and return true even if the game didn't end because the current move was successfully performed.
                    m_currSide = Side::SOUTH;
                    return true;
                }
                
                // if you end up in South's pot, take another move if the game isn't already over.
                else
                {
                    // if the game is now over and North's side is empty, move South's beans over to its pot and return true since the current move was successfully made.
                    if(m_Board.beansInPlay(Side::NORTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::SOUTH, i, Side::SOUTH);
                        return true;
                    }
                    
                    // if the game is now over and South's side is empty, move North's beans over to its pot and return true since the current move was successfully made.
                    else if(m_Board.beansInPlay(Side::SOUTH) == 0)
                    {
                        for(int i=1; i<m_Board.holes()+1; i++)
                            m_Board.moveToPot(Side::NORTH, i, Side::NORTH);
                        return true;
                    }
                    display(); // display to show that the player took another turn.
                    continue; // have the current player take another turn.
                }
            }
        }
    }
}

void Game::play()
{
    std::cout << "Welcome to Kalah!" << std::endl << std::endl; // welcome message.
    
    while(true) // keep displaying the game as long as it isn't over.
    {
        display(); // display the current state of the Board, indicating who is to move next.
      
        // to prevent instant scrolldown which might result if both players are non-interactive, we prompt the user to hit enter to command the game to move forward.
        if(!m_South->isInteractive() && !m_North->isInteractive())
        {
            std::cout << "Press enter to continue." << std::endl;
            std::cin.ignore(10000, '\n');
        }
        
        // actually making the move by the player which was indicated at the end of Game::display() function.
        bool flag = move();
        
        // checking in whether or not the game is already over, and if it is, we break out of the loop to display the game's results.
        if(!flag)
            break;

        std::cout << std::endl;
    }
    
    // calling Game::status() once again to find the game's results and print an appropriate message.
    bool isOver = true;
    bool winnerExists = false;
    Side winSide;
    
    status(isOver, winnerExists, winSide);
    
    // if it is a tie, print a message saying so.
    if(!winnerExists)
        std::cout << "It's a tie!" << std::endl;
    
    // if it isn't a tie, print the winner's name.
    else
    {
        if(winSide == Side::SOUTH)
            std::cout << m_South->name() << " won!" << std::endl;
        else
            std::cout << m_North->name() << " won!" << std::endl;
    }
    
    std::cout << std::endl;
}

// for a legal hole (excluding a pot), this function uses Board class's Board::beans() function to return the number of beans in that hole.
int Game::beans(Side s, int hole) const
{
    if(hole<0 || hole>m_Board.holes())
        return -1;
    else
        return m_Board.beans(s, hole);
}
