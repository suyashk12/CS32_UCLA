#ifndef NEWSEQUENCE_INCLUDED
#define NEWSEQUENCE_INCLUDED

const int DEFAULT_MAX_ITEMS = 250;
using ItemType = unsigned long;

class Sequence
{
public:
    Sequence(int len = DEFAULT_MAX_ITEMS);
    Sequence(const Sequence &other);
    Sequence& operator=(const Sequence &other);
    ~Sequence();
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
    ItemType *m_sequence;
    int m_present;
    int m_max;
};

#endif
