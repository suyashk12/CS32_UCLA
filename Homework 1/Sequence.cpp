#include <iostream>
#include "Sequence.h"

Sequence::Sequence(): m_size(0)
{
    
}

bool Sequence::empty() const
{
    if(m_size == 0)
    {
        return(true);
    }
    
    return(false);
}

int Sequence::size() const
{
    return(m_size);
}

int Sequence::insert(int pos, const ItemType& value)
{
    if(pos>=0 && pos<=m_size && m_size<DEFAULT_MAX_ITEMS)
    {
        for(int i = m_size; i>pos; i--)
        {
            m_sequence[i] = m_sequence[i-1];
        }
        m_sequence[pos] = value;
        m_size++;
        return(pos);
    }

    return(-1);
    
}

int Sequence::insert(const ItemType& value)
{
    int pos = m_size;
    for(int i=0; i<m_size; i++)
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
    if(pos>=0 && pos<m_size && m_size<=DEFAULT_MAX_ITEMS)
    {
        for(int i=pos; i<(m_size-1); i++)
        {
            m_sequence[i] = m_sequence[i+1];
        }
        m_size--;
        return(true);
    }
    return(false);
}

int Sequence::remove(const ItemType& value)
{
    int counter = 0;
    for(int i=0; i<m_size; i++)
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
    if(pos>=0 && pos<m_size)
    {
        value = m_sequence[pos];
        return(true);
    }
    return(false);
}

bool Sequence::set(int pos, const ItemType& value)
{
    if(pos>=0 && pos<m_size)
    {
        m_sequence[pos] = value;
        return(true);
    }
    return(false);
}

int Sequence::find(const ItemType& value) const
{
    for(int i=0; i<m_size; i++)
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
    int m_size_this = m_size;
    int m_size_other = other.m_size;
    
    int max = m_size_this;
    
    if(m_size_other > m_size_this)
    {
        max = m_size_other;
    }
    
    for(int i=0; i<max; i++)
    {
        ItemType temp;
        temp = other.m_sequence[i];
        other.m_sequence[i] = m_sequence[i];
        m_sequence[i] = temp;
    }
    
    m_size = m_size_other;
    other.m_size = m_size_this;
}
