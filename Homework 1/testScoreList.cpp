#include <iostream>
#include <cassert>
#include "ScoreList.h"

int main()
{
    ScoreList sl;
    
    for(int i=0; i<100; i++)
    {
        sl.add(i);
    }
    
    assert(sl.size() == 100);
    assert(sl.minimum() == 0);
    assert(sl.maximum() == 99);
    
    for(int i=50; i<100; i++)
    {
        sl.remove(i);
    }
    
    assert(sl.size() == 50);
    assert(sl.minimum() == 0);
    assert(sl.maximum() == 49);
    
    sl.remove(0);
    
    assert(sl.size() == 49);
    assert(sl.minimum() == 1);
    assert(sl.maximum() == 49);
    
    sl.remove(49);
    
    assert(sl.size() == 48);
    assert(sl.minimum() == 1);
    assert(sl.maximum() == 48);
    
    sl.add(48);
    
    assert(sl.size() == 49);
    assert(sl.minimum() == 1);
    assert(sl.maximum() == 48);
    
    sl.remove(48);
    
    assert(sl.size() == 48);
    assert(sl.minimum() == 1);
    assert(sl.maximum() == 48);
    
    sl.remove(48);
    
    assert(sl.size() == 47);
    assert(sl.minimum() == 1);
    assert(sl.maximum() == 47);
    
    std::cout << "Passed all tests" << std::endl;
    return(0);
}
