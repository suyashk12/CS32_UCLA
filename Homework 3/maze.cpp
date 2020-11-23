bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    maze[sr][sc] = 'v';
    
    if(sr == er && sc == ec)
        return(true);
    else
    {
        if(sr!=9 && maze[sr+1][sc]!='X' && maze[sr+1][sc]!='v')
        {
            if(pathExists(maze, sr+1, sc, er, ec))
                return true;
        }
        if(sc!=0 && maze[sr][sc-1]!='X' && maze[sr][sc-1]!='v')
        {
            if(pathExists(maze, sr, sc-1, er, ec))
                return true;
        }
        if(sr!=0 && maze[sr-1][sc]!='X' && maze[sr-1][sc]!='v')
        {
            if(pathExists(maze, sr-1, sc, er, ec))
                return true;
        }
        if(sc!=9 && maze[sr][sc+1]!='X' && maze[sr][sc+1]!='v')
        {
            if(pathExists(maze, sr, sc+1, er, ec))
                return true;
        }
    }
    return(false);
}
