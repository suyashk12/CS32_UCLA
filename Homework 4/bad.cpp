void removeBad(list<Movie*>& li)
{
    list<Movie*>::iterator p = li.begin();
    
    while(p!=li.end())
    {
        if((*p)->rating()<55)
        {
            delete *p;
            p = li.erase(p);
            continue;
        }
        p++;
    }
}
