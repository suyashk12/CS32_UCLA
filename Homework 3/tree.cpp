int countIncludes(const string a1[], int n1, const string a2[], int n2)
{
    if(n2<=0)
        return 1;
    
    else if(n1<n2)
        return 0;
    
    else if(n1==1 && n2==1)
        return(a1[0] == a2[0]);
    
    else
    {
        if(a1[n1-1] == a2[n2-1])
            return(countIncludes(a1, n1-1, a2, n2) + countIncludes(a1, n1-1, a2, n2-1));
        
        else
            return(countIncludes(a1, n1-1, a2, n2));
    }
}

void exchange(string& x, string& y)
{
    string t = x;
    x = y;
    y = t;
}

void separate(string a[], int n, string separator,
              int& firstNotLess, int& firstGreater)
{
    if (n < 0)
        n = 0;
    
    firstNotLess = 0;
    firstGreater = n;
    int firstUnknown = 0;
    while (firstUnknown < firstGreater)
    {
        if (a[firstUnknown] > separator)
        {
            firstGreater--;
            exchange(a[firstUnknown], a[firstGreater]);
        }
        else
        {
            if (a[firstUnknown] < separator)
            {
                exchange(a[firstNotLess], a[firstUnknown]);
                firstNotLess++;
            }
            firstUnknown++;
        }
    }
}


void order(string a[], int n)
{
    if(n<=1)
        return;
    else
    {
        int firstleq = 0;
        int firstg = 0;
        separate(a, n, a[0], firstleq, firstg);
        order(a, firstleq);
        order(a+firstg, n-firstg);
    }
}
