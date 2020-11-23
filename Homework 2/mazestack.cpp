#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = 'v';
    
    while(!coordStack.empty())
    {
        Coord present = coordStack.top();
        coordStack.pop();
        if(present.r() == er && present.c() == ec)
            return(true);
        else
        {
            if(present.r()!=9 && maze[present.r()+1][present.c()]!='X' && maze[present.r()+1][present.c()]!='v')
            {
                coordStack.push(Coord(present.r()+1, present.c()));
                maze[present.r()+1][present.c()] = 'v';
            }
            if(present.c()!=0 && maze[present.r()][present.c()-1]!='X' && maze[present.r()][present.c()-1]!='v')
            {
                coordStack.push(Coord(present.r(), present.c()-1));
                maze[present.r()][present.c()-1] = 'v';
            }
            if(present.r()!=0 && maze[present.r()-1][present.c()]!='X' && maze[present.r()-1][present.c()]!='v')
            {
                coordStack.push(Coord(present.r()-1, present.c()));
                maze[present.r()-1][present.c()] = 'v';
            }
            if(present.c()!=9 && maze[present.r()][present.c()+1]!='X' && maze[present.r()][present.c()+1]!='v')
            {
                coordStack.push(Coord(present.r(), present.c()+1));
                maze[present.r()][present.c()+1] = 'v';
            }
        }
    }
    return(false);
}

int main()
{
    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X' },
        { 'X','.','.','.','.','X','.','.','.','X' },
        { 'X','.','X','X','.','X','X','X','.','X' },
        { 'X','X','X','.','.','.','.','X','.','X' },
        { 'X','.','X','X','X','.','X','X','X','X' },
        { 'X','.','X','.','.','.','X','.','.','X' },
        { 'X','.','.','.','X','.','X','.','.','X' },
        { 'X','X','X','X','X','.','X','.','X','X' },
        { 'X','.','.','.','.','.','.','.','.','X' },
        { 'X','X','X','X','X','X','X','X','X','X' }
    };
    
    if (pathExists(maze, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
