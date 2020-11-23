#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1  &&  s.find(42) == 0);

    Sequence s1;
    s1.insert(0, 20);
    s1.insert(0, 10);
    s1.insert(15);
    s1.insert(40);
    assert(s1.size() == 4);
    ItemType x = 999;
    assert(s1.get(0, x) && x == 10);
    assert(s1.get(1, x) && x == 15);
    assert(s1.get(2, x) && x == 20);
    assert(s1.get(3, x) && x == 40);
    s1.erase(0);
    assert(s1.size() == 3);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 20);
    assert(s1.get(2, x) && x == 40);
    s1.set(2, 20);
    assert(s1.size() == 3);
    assert(s1.get(0, x) && x == 15);
    assert(s1.get(1, x) && x == 20);
    assert(s1.get(2, x) && x == 20);
    s1.remove(20);
    assert(s1.size() == 1);
    assert(s1.get(0, x) && x == 15);

    Sequence s2;
    s2.insert(10);
    s2.insert(199);
    s2.insert(25);
    assert(s2.find(25) == 1);

    s1.swap(s2);

    assert(s1.size() == 3);
    assert(s1.get(0, x) && x == 10);
    assert(s1.get(1, x) && x == 25);
    assert(s1.get(2, x) && x == 199);

    assert(s2.size() == 1);
    assert(s2.get(0, x) && x == 15);
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
