#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

#include <string>

using ItemType = std::string;

class Sequence
{
public:
    Sequence();
    Sequence(const Sequence &other);
    ~Sequence();
    Sequence& operator=(const Sequence&other);
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
private:
    struct Node // declaring a private struct for creating nodes of the linked list
    {
        ItemType m_data; // each Node object has an instance of ItemType
        
        // the project utilizes a circular doubly linked list, and for the same, each Node object needs to have a pointer to its successor Node object, and one to its predecessor Node object. This also explains why both these pointers need to be of type Node.
        Node* m_next;
        Node* m_prev;
    };
    
    Node* head; // A pointer of type Node to the dummy node will be needed to implement various functions within this class.
    int m_size; // This integer value is to keep track of how many items the doubly linked list contains presently. Note that it wasn't strictly required to have this (one could traverse through the linked list), but having it at our disposal saves us processing time.
};

// non-member functions required to be implemented as per the spec.

int subsequence(const Sequence& seq1, const Sequence& seq2);

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif
