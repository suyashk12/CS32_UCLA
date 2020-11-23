bool allTrue(const string a[], int n)
{
    if(n<=0)
        return true;
    
    else if(n == 1)
        return(somePredicate(a[0]));
    
    else
        return(allTrue(a, n-1) && somePredicate(a[n-1]));
}

int countFalse(const string a[], int n)
{
    if(n<=0)
        return(0);
    
    else if(n==1)
        return(!somePredicate(a[0]));
    
    else
        return(!somePredicate(a[n-1])+countFalse(a, n-1));
}

int firstFalse(const string a[], int n)
{
    if(n<=0)
        return(-1);
    
    else if(n==1)
    {
        if(!somePredicate(a[0]))
            return(0);
        
        else
            return(-1);
    }
    
    else
    {
        int k = firstFalse(a, n-1);
        
        if(k == -1)
        {
            if(!somePredicate(a[n-1]))
                return(n-1);
            
            else
                return(-1);
        }
        
        else
            return(k);
    }
}

int indexOfLeast(const string a[], int n)
{
    if(n <= 0)
        return(-1);
    
    else if(n==1)
        return(0);
    
    else
    {
        int k = indexOfLeast(a, n-1);
        
        if(a[n-1]<a[k])
            return(n-1);
        
        else
            return(k);
    }
}

bool includes(const string a1[], int n1, const string a2[], int n2)
{
    if(n2 <= 0)
        return true;
    
    else if(n1<n2)
        return false;
    
    else if(n1 == 1 && n2 == 1)
        return(a1[0] == a2[0]);
    
    else
        return(includes(a1, n1-1, a2, n2 - (a1[n1-1] == a2[n2-1])));
}
