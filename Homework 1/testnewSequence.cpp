#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s(1);
    assert(s.empty());
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1  &&  s.find(42) == 0);
    
    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 5;
    
    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v) != -1);
    
    // Failure if we try to insert a sixth item into b
    assert(b.insert(v) == -1);
    
    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    assert(a.insert(v) == -1  &&  b.insert(v) != -1);
    
    Sequence s1(4);
    s1.insert(0, 20);
    s1.insert(0, 10);
    s1.insert(15);
    assert(s1.size() == 3);
    ItemType x = 999;
    assert(s1.get(0, x) && x == 10);
    assert(s1.get(1, x) && x == 15);
    assert(s1.get(2, x) && x == 20);
    s1.erase(0);
    assert(s1.size() == 2);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 20);
    s1.insert(20);
    assert(s1.size() == 3);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 20);
    assert(s1.get(2, x) && x == 20);
    assert(s1.remove(20) == 2);
    assert(s1.size() == 1);
    assert(s1.get(0, x) && x == 15);
    s1.insert(1, 25);
    s1.insert(2, 40);
    s1.insert(3, 22);
    assert(s1.size() == 4);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 25);
    assert(s1.get(2, x) && x == 40);
    assert(s1.get(3, x) && x == 22);
    s1.set(3, 25);
    assert(s1.size() == 4);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 25);
    assert(s1.get(2, x) && x == 40);
    assert(s1.get(3, x) && x == 25);
    assert(s1.find(25) == 1);
    assert(s1.remove(25) == 2);
    assert(s1.size() == 2);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 40);
    assert(s1.find(40) == 1);
    
    Sequence s2(3);
    s2.insert(5);
    s2.insert(10);
    s2.insert(8);
    assert(s2.get(0, x) && x == 5);
    assert(s2.get(1, x) && x == 8);
    assert(s2.get(2, x) && x == 10);
    
    s1.swap(s2);
    
    assert(s1.size() == 3);
    assert(s1.get(0, x) && x == 5);
    assert(s1.get(1, x) && x == 8);
    assert(s1.get(2, x) && x == 10);
    
    assert(s2.size() == 2);
    assert(s2.get(0, x) && x == 15);
    assert(s2.get(1, x) && x == 40);
    
    Sequence s3(s2);
    
    assert(s3.size() == 2);
    assert(s3.get(0, x) && x == 15);
    assert(s3.get(1, x) && x == 40);
    
    s3 = s1;
    
    assert(s3.size() == 3);
    assert(s3.get(0, x) && x == 5);
    assert(s3.get(1, x) && x == 8);
    assert(s3.get(2, x) && x == 10);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
