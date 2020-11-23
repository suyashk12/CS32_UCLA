#include <iostream>
#include <cassert>
#include "Board.h"
#include "Player.h"
#include "Game.h"

void doBoardTests()
{
    // test cases for Board Class.
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // test cases already provided by the spec
    
    Board b(3, 2);
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
           b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
           b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
           b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // testing for major illegal behavior
    
    Board testnull(0, -1); // bad inputs for constructor parameters are handled as specified in spec.
    
    //   0
    // 0   0
    //   0
    
    assert(testnull.holes() == 1); // non-positive values for holes argument are treated as a 1.
    assert(testnull.totalBeans() == 0); // as negative values for initial beans per hole are treated as 0.
    assert(testnull.beans(Side::SOUTH, 20) == -1); // cannot access beans from a non-existent hole. An illegal call to Board::beans() returns -1.
    assert(!testnull.setBeans(Side::SOUTH, 20, 2)); // illegal calls to Board::setBeans() will return false.
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // testing for legal behavior (and some illegal behavior that might occur on the way)
    
    Board testb1(6, 2); // constructing a usual board.
    
    //   2 2 2 2 2 2
    // 0             0
    //   2 2 2 2 2 2
    
    assert(testb1.holes() == 6); // the Board::holes() function works appropriately. As specified in the constructor, this board has 6 holes on each side.
    assert(testb1.totalBeans() == 24); // the Board::totalBeans() function works appropriately. The total number of beans on the board should remain constant throughout the game and should be equal to twice of number of holes times the number of beans supplied to each hole initially. Note however that the total number of beans can change if the user uses Board::setBeans() anytime.
    
    // the Board::beansInPlay() function works appropriately. To begin with, each side should have the number of total beans in play (note that this quantity doesn't count the number of beans in the respective pots of each side, which is also zero to begin with).
    assert(testb1.beansInPlay(Side::SOUTH) == testb1.totalBeans()/2);
    assert(testb1.beansInPlay(Side::NORTH) == testb1.totalBeans()/2);
    
    for(int i=1; i<=6; i++)
    {
        // the Board::beans() funciton works appropriately. Each hole along each side should have 2 beans.
        assert(testb1.beans(Side::SOUTH, i) == 2);
        assert(testb1.beans(Side::NORTH, i) == 2);
    }
    
    // each pot should have 0 beans to begin with
    assert(testb1.beans(Side::SOUTH, 0) == 0);
    assert(testb1.beans(Side::NORTH, 0) == 0);
    
    // sowing hole number 5 along South, which is a legal move since the number of beans in hole 5 for South is non-zero. The end side for this move should be South and the end hole should be 0 (South's pot). The movement of beans should always be in the counter-clockwise direction.
    
    Side endSide;
    int endHole;
    
    assert(testb1.sow(Side::SOUTH, 5, endSide, endHole));
    
    //   2 2 2 2 2 2
    // 0             1
    //   2 2 2 2 0 3
    
    assert(endSide == Side::SOUTH);
    assert(endHole == 0);
    assert(testb1.beans(Side::SOUTH, 5) == 0); // as all beans from this hole were sowed.
    assert(testb1.beansInPlay(Side::SOUTH) == 11); // the number of beans in South's holes is now one less than before since one of the beans on South's side ended up in its pot.
    
    assert(testb1.beans(Side::SOUTH, 6) == 3); // as the hole 6 in South will now have an additional bean.
    assert(testb1.beans(Side::SOUTH, 0) == 1); // the South side's pot now has one bean.
    assert(testb1.beans(Side::SOUTH, 4) == 2); // anything to the left of hole 5 on South wasn't affected, since movement of beans is always counter-clockwise.
    
    
    assert(!testb1.sow(Side::SOUTH, 5, endSide, endHole)); // hole number 5 for South is empty, so cannot be used to make a move.
    
    assert(testb1.sow(Side::SOUTH, 6, endSide, endHole)); // a move should be legal from South's 6th hole, and the endSide and endHole should respectively be North and 5. The rotation of beans will be counter-clockwise once again.
    
    //   2 2 2 2 3 3
    // 0             2
    //   2 2 2 2 0 0
    
    assert(endSide == Side::NORTH);
    assert(endHole == 5);
    assert(testb1.beans(Side::SOUTH, 6) == 0); // as all beans from this hole were sowed.
    
    assert(testb1.beans(Side::SOUTH, 0) == 2); // South's pot gained another bean.
    assert(testb1.beans(Side::NORTH, 6) == 3); // North's 6th hole gained another bean.
    assert(testb1.beans(Side::NORTH, 5) == 3); // North's 5th hole gained another bean as well.
    
    assert(!testb1.moveToPot(Side::SOUTH, 0, Side::NORTH)); // the Board::moveToPot() function is working appropriately. Cannot move from one pot to another.
    
    //   2 2 2 2 3 3
    // 2             2
    //   0 2 2 2 0 0
    
    assert(testb1.moveToPot(Side::SOUTH, 1, Side::NORTH)); //  Moving over all beans in South's 1st hole to North's pot.
    
    assert(testb1.beans(Side::SOUTH, 1) == 0); // the first hole of South side now has 0 beans.
    assert(testb1.beans(Side::NORTH, 0) == 2); // as North's pot received these beans.
    
    // South can sow its beans in its own pot but not in North's.
    
    assert(testb1.setBeans(Side::SOUTH, 6, 8)); // setting the number of beans in South's 6th hole to be 8 to illustrate our point here.
    
    //   2 2 2 2 3 3
    // 2             2
    //   0 2 2 2 0 8
    
    assert(testb1.beans(Side::SOUTH, 6) == 8); // Board::setBeans() function is working appropriately.
    
    assert(testb1.sow(Side::SOUTH, 6, endSide, endHole)); // sowing all of the beans in South's 6th hole. The endSide and endHole should respectively be Side::SOUTH and 1, as North's pot was skipped along the way.
    
    //   3 3 3 3 4 4
    // 2             3
    //   1 2 2 2 0 0
    
    assert(endSide == Side::SOUTH);
    assert(endHole == 1);
    assert(testb1.beans(Side::SOUTH, 6) == 0); // as all the beans in this hole were sowed.
    
    assert(testb1.beans(Side::SOUTH, 0) == 3); // as South's pot gained another bean (it previously had 2).
    assert(testb1.beans(Side::NORTH, 6) == 4); // North's 6th hole gains another bean (it had 3 before).
    assert(testb1.beans(Side::NORTH, 5) == 4); // North's 5th hole gains another bean (it ahd 3 before).
    
    for(int i=4; i>=1; i--)
        assert(testb1.beans(Side::NORTH, i) == 3); // all of North's rest of the holes gain a bean each (they had 2 before).
    
    assert(testb1.beans(Side::NORTH, 0) == 2); // the number of beans in North's pot isn't affected as it was skipped (it previously had 2, and has the same even now).
    
    assert(testb1.beans(Side::SOUTH, 1) == 1); // South's 1st hole, where we end, gains another bean (it had 0 before as its beans were moved over to North's pot).
    
    assert(testb1.setBeans(Side::NORTH, 1, 8)); // a move from North can sow in its own pot but not in South's pot. This example will illustrate the point.
    
    //   8 3 3 3 4 4
    // 2             3
    //   1 2 2 2 0 0
    
    assert(testb1.sow(Side::NORTH, 1, endSide, endHole)); // for this case, the endSide and endHole should respectively be North and 6, as South's pot was skipped over.
    
    //   0 3 3 3 4 5
    // 3             3
    //   2 3 3 3 1 1
    
    assert(testb1.beans(Side::NORTH, 1) == 0); // as all the beans in this hole were sowed over.
    assert(endSide == Side::NORTH);
    assert(endHole == 6);
    
    assert(testb1.beans(Side::NORTH, 0) == 3); // North's pot gains a bean (it previously had 2).
    assert(testb1.beans(Side::SOUTH, 1) == 2); // South's first hole gains a bean (it previously had 1).
    
    for(int i=2; i<=4; i++)
        assert(testb1.beans(Side::SOUTH, i) == 3); // South's second to fourth hole gaina a bean (they previously had 2).
    
    assert(testb1.beans(Side::SOUTH, 5) == 1); // South's 5th hole has 1 bean now (it previously had 0 because we sowed from it).
    assert(testb1.beans(Side::SOUTH, 6) == 1); // South's 6th hole has 1 bean now (it previously had 0 beacuse we sowed from it).
    assert(testb1.beans(Side::SOUTH, 0) == 3); // South's pot is unaffected, and continues to have 3 beans.
    assert(testb1.beans(Side::NORTH, 6) == 5); // North's 6th hole gains another bean (it previously had 4).
    
    // This is how the board should currently look
    
    //   0 3 3 3 4 5
    // 3             3
    //   2 3 3 3 1 1
    
    // let's test to see if this is true, moving counter-clockwise from South's first hole.
    
    assert(testb1.beans(Side::SOUTH, 1) == 2);
    assert(testb1.beans(Side::SOUTH, 2) == 3);
    assert(testb1.beans(Side::SOUTH, 3) == 3);
    assert(testb1.beans(Side::SOUTH, 4) == 3);
    assert(testb1.beans(Side::SOUTH, 5) == 1);
    assert(testb1.beans(Side::SOUTH, 6) == 1);
    assert(testb1.beans(Side::SOUTH, 0) == 3);
    assert(testb1.beans(Side::NORTH, 6) == 5);
    assert(testb1.beans(Side::NORTH, 5) == 4);
    assert(testb1.beans(Side::NORTH, 4) == 3);
    assert(testb1.beans(Side::NORTH, 3) == 3);
    assert(testb1.beans(Side::NORTH, 2) == 3);
    assert(testb1.beans(Side::NORTH, 1) == 0);
    assert(testb1.beans(Side::NORTH, 0) == 3);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // this comprehensively tests the Board class.
}

void doPlayerTests()
{
    // test cases for the HumanPlayer, BadPlayer, and SmartPlayer classes
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // test cases already provided by the spec.
    
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    
    //   2 2 2
    // 0       0
    //   2 0 2
    
    std::cout << "Please enter a move for a HumanPlayer playing on the South side represented by {2, 0, 2}" << std::endl;
    int n = hp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3); // must be a legal move. In fact, HumanPlayer::chooseMove() will keep demanding an input from the user until they provide a legal one.

    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3); // BadPlayer::chooseMove() will return a move which, although arbitrary and not well-thought of, will still be legal (i.e. not from an empty hole or a non-existent hole).
    
    n = sp.chooseMove(b, SOUTH); // SmartPlayer::chooseMove() will return a move which is both well-thought of and also legal (i.e. not from an empty hole or a non-existent hole).
    assert(n == 1  ||  n == 3);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // further test cases.
    
    HumanPlayer testhuman("HumanTester");
    BadPlayer testbad("BadTester");
    SmartPlayer testsmart("SmartTester");
    
    assert(testhuman.name() == "HumanTester"  && testhuman.isInteractive()); // a human player is interactive.
    assert(testbad.name() == "BadTester" && !testbad.isInteractive());  // a bad player is not interactive.
    assert(testsmart.name() == "SmartTester" && !testsmart.isInteractive()); // a smart player is not interactive.
    
    Board testb2(5, 3);
    
    //   3 3 3 3 3
    // 0           0
    //   3 3 3 3 3
    
    assert(testb2.setBeans(Side::SOUTH, 1, 0));
    assert(testb2.setBeans(Side::SOUTH, 2, 1));
    assert(testb2.setBeans(Side::SOUTH, 4, 1));
    assert(testb2.setBeans(Side::SOUTH, 5, 0));
    assert(testb2.setBeans(Side::NORTH, 1, 0));
    assert(testb2.setBeans(Side::NORTH, 2, 0));
    assert(testb2.setBeans(Side::NORTH, 3, 0));
    
    
    //   0 0 0 3 3
    // 0           0
    //   0 1 3 1 0
 
    std::cout << "Please enter a move for a HumanPlayer playing on the South side represented by {0, 3, 3, 3, 0}" << std::endl;
    
    int testhole = testhuman.chooseMove(testb2, Side::SOUTH);
    
    assert(testhole == 2 || testhole == 3 || testhole == 4); // the way HumanPlayer::chooseMove() is designed, the chosen move by the human will ultimately have to be a legal one.
    
    testhole = testbad.chooseMove(testb2, Side::SOUTH); // a BadPlayer will always choose the a non-empty hole which is equal to or closest to 1 on its side. In this case, it will, this move will be 2.
    
    assert(testhole == 2);
    
    testhole = testsmart.chooseMove(testb2, Side::SOUTH); // a SmartPlayer will choose a move which is the best possible in a given situation. Of course, this move will also be a legal one. In this case, it chooses to move along hole 3, which results in it ending in its own pot, giving it another chance to play.
    
    assert(testhole == 3);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // this comprehensively tests all classes deriving from the Player class. A further analysis for the SmartPlayer class can be found in tests for the Game class, where we consider actual games between two players.
}

void doGameTests()
{
    // test cases for the Game class
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // test cases already provided by the spec.
    
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
    //    Homer
    //   0  1  2
    // 0         0
    //   2  0  0
    //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
           g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   0  1  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   1  0  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   1  0  0
    // 0         3
    //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);
    
    g.move();
    //   0  0  0
    // 1         4
    //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // further test cases
    
    // here is a mid-game situation where SmartPlayer is expected to win at once using a series of multiple moves according to the spec. To test this hypothesis, let's play a SmartPlayer against another SmartPlayer.
    
    //    any player
    //   1 0 0 0 2 0
    // 22            20
    //   0 0 0 1 2 0
    //      Smart
    
    Board testb3(6, 0);
    assert(testb3.setBeans(Side::SOUTH, 4, 1));
    assert(testb3.setBeans(Side::SOUTH, 5, 2));
    assert(testb3.setBeans(Side::SOUTH, 0, 20));
    assert(testb3.setBeans(Side::NORTH, 5, 2));
    assert(testb3.setBeans(Side::NORTH, 1, 1));
    assert(testb3.setBeans(Side::NORTH, 0, 22));
    
    SmartPlayer testSouth("SouthSmart");
    SmartPlayer testNorth("NorthSmart");
    
    Game testgame(testb3, &testSouth, &testNorth);
    
    assert(testgame.beans(Side::SOUTH, 1) == 0);
    assert(testgame.beans(Side::SOUTH, 2) == 0);
    assert(testgame.beans(Side::SOUTH, 3) == 0);
    assert(testgame.beans(Side::SOUTH, 4) == 1);
    assert(testgame.beans(Side::SOUTH, 5) == 2);
    assert(testgame.beans(Side::SOUTH, 6) == 0);
    assert(testgame.beans(Side::SOUTH, 0) == 20);
    assert(testgame.beans(Side::NORTH, 6) == 0);
    assert(testgame.beans(Side::NORTH, 5) == 2);
    assert(testgame.beans(Side::NORTH, 4) == 0);
    assert(testgame.beans(Side::NORTH, 3) == 0);
    assert(testgame.beans(Side::NORTH, 2) == 0);
    assert(testgame.beans(Side::NORTH, 1) == 1);
    assert(testgame.beans(Side::NORTH, 0) == 22);
    
    testgame.move();
    
    //    any player
    //   0 0 0 0 0 0
    // 23            25
    //   0 0 0 0 0 0
    //      Smart
    
    for(int i=1; i<=6; i++)
    {
        assert(testgame.beans(Side::SOUTH, i) == 0);
        assert(testgame.beans(Side::NORTH, i) == 0);
    }

    assert(testgame.beans(Side::SOUTH, 0) == 25);
    assert(testgame.beans(Side::NORTH, 0) == 23);
    
    testgame.status(over, hasWinner, winner);
    
    assert(over && hasWinner && winner == Side::SOUTH);
    
    // thus, the SmartPlayer on the South side is winning this game, as was required by the spec. We can further test out a few games.
    
    // Game::display() function is working appropriately since in between multiple moves, the displaying for another move is a job of Game::move(), and it happened and should be visible in the console.
}

int main()
{
    doBoardTests();
    doPlayerTests();
    doGameTests();
    std::cout << "All tests passed!" << std::endl << std::endl;
    
    // some games to play
    
    std::cout << "BadPlayer vs. BadPlayer!" << std::endl << std::endl;
    
    Board gameboard(6, 4);
    
    BadPlayer bad1("Bad1");
    BadPlayer bad2("Bad2");
    
    Game gbb(gameboard, &bad1, &bad2);
    gbb.play();
    
    std::cout << "SmartPlayer (South) vs. BadPlayer (North)!" << std::endl << std::endl;
    
    SmartPlayer smart1("Smart1");
    
    
    Game gsb(gameboard, &smart1, &bad1);
    gsb.play();
    
    std::cout << "SmartPlayer (North) vs. BadPlayer(South)!" << std::endl << std::endl;
    
    Game gbs(gameboard, &bad1,&smart1);
    gbs.play();
    
    std::cout << "SmartPlayer vs. SmartPlayer!" << std::endl << std::endl;
    
    SmartPlayer smart2("Smart2");
    
    Game gss(gameboard, &smart1, &smart2);
    gss.play();
    
    std::cout << "SmartPlayer (South) vs. HumanPlayer (North)!" << std::endl << std::endl;
    
    HumanPlayer human("Human");
    
    Game gsh(gameboard, &smart1, &human);
    gsh.play();
    
    std::cout << "HumanPlayer (North) vs. SmartPlayer (South)!" << std::endl << std::endl;
    
    Game ghs(gameboard, &human, &smart1);
    ghs.play();

    return 0;
}
