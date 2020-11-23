#include "ScoreList.h"

ScoreList::ScoreList()
{

}

bool ScoreList::add(unsigned long score)
{
    if(score<=100)
    {
        int check = m_list.insert(m_list.size(), score);
        if(check == -1)
        {
            return(false);
        }
        return(true);
    }
    return(false);
}

bool ScoreList::remove(unsigned long score)
{
    for(int i=0; i<m_list.size(); i++)
    {
        unsigned long temp;
        m_list.get(i, temp);
        if(temp == score)
        {
            m_list.erase(i);
            return(true);
        }
    }
    return(false);
}

int ScoreList::size() const
{
    return(m_list.size());
}

unsigned long ScoreList::minimum() const
{
    if(m_list.empty())
    {
        return(NO_SCORE);
    }
    
    unsigned long min;
    m_list.get(0, min);
    unsigned long present;

    for(int i=1; i<m_list.size(); i++)
    {
        m_list.get(i, present);
        if(present<min)
        {
            min = present;
        }
    }

    return(min);
}

unsigned long ScoreList::maximum() const
{
    if(m_list.empty())
    {
        return(NO_SCORE);
    }
    
    unsigned long max;
    m_list.get(0, max);
    unsigned long present;


    for(int i=1; i<m_list.size(); i++)
    {
        m_list.get(i, present);
        if(present>max)
        {
            max = present;
        }
    }

    return(max);

}
