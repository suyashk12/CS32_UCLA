#include <iostream>
#include <cstdlib>
#include "newSequence.h"

Sequence::Sequence(int len): m_present(0), m_max(len)
{
    if(len<0)
    {
        std::cout << "Sequence having negative number of elements cannot be created." << std::endl;
        exit(1);
    }
    m_sequence = new ItemType[m_max];
}

Sequence::Sequence(const Sequence &other): m_present(other.m_present), m_max(other.m_max)
{
    m_sequence = new ItemType[m_max];
    
    for(int i=0; i<m_present; i++)
    {
        m_sequence[i] = other.m_sequence[i];
    }
}

Sequence& Sequence::operator=(const Sequence &other)
{
    if(this!=&other)
    {
        Sequence temp(other);
        swap(temp);
    }
    return(*this);
}

Sequence::~Sequence()
{
    delete [] m_sequence;
}

bool Sequence::empty() const
{
    if(m_present == 0)
    {
        return(true);
    }
    
    return(false);
}

int Sequence::size() const
{
    return(m_present);
}

int Sequence::insert(int pos, const ItemType& value)
{
    if(pos>=0 && pos<=m_present && m_present<m_max)
    {
        for(int i = m_present; i>pos; i--)
        {
            m_sequence[i] = m_sequence[i-1];
        }
        m_sequence[pos] = value;
        m_present++;
        return(pos);
    }
    
    return(-1);
    
}

int Sequence::insert(const ItemType& value)
{
    int pos = m_present;
    for(int i=0; i<m_present; i++)
    {
        if(value <= m_sequence[i])
        {
            pos = i;
            break;
        }
    }
    
    return(insert(pos, value));
}

bool Sequence::erase(int pos)
{
    if(pos>=0 && pos<m_present && m_present<=m_max)
    {
        for(int i=pos; i<(m_present-1); i++)
        {
            m_sequence[i] = m_sequence[i+1];
        }
        m_present--;
        return(true);
    }
    return(false);
}

int Sequence::remove(const ItemType& value)
{
    int counter = 0;
    for(int i=0; i<m_present; i++)
    {
        if(m_sequence[i] == value)
        {
            erase(i);
            i--;
            counter++;
        }
    }
    return(counter);
}

bool Sequence::get(int pos, ItemType& value) const
{
    if(pos>=0 && pos<m_present)
    {
        value = m_sequence[pos];
        return(true);
    }
    return(false);
}

bool Sequence::set(int pos, const ItemType& value)
{
    if(pos>=0 && pos<m_present)
    {
        m_sequence[pos] = value;
        return(true);
    }
    return(false);
}

int Sequence::find(const ItemType& value) const
{
    for(int i=0; i<m_present; i++)
    {
        if(m_sequence[i]==value)
        {
            return(i);
        }
    }
    return(-1);
}

void Sequence::swap(Sequence& other)
{
    int temp_present = other.m_present;
    other.m_present = m_present;
    m_present = temp_present;
    
    int temp_max = other.m_max;
    other.m_max = m_max;
    m_max = temp_max;
    
    ItemType* temp_sequence = other.m_sequence;
    other.m_sequence = m_sequence;
    m_sequence = temp_sequence;
}
