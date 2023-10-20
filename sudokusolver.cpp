#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
#define pb push_back

// predefining 3x3 squares indexes for faster computation
int sqr[9][9];
vector<pii> tiles[9];

// function to print the board state
void print_board(vector<vector<int>> &board, bool mode)
{
    if (!mode)
    {
	// plain output
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
                cout << board[i][j] << " ";
            cout << "\n";
        }
        cout << "\n";
    }
    else
    {
	// "ascii decorated" output
        for (int i = 0; i < 9; ++i)
        {
            if (i % 3 == 0)
            {
                for (int j = 0; j < 9; ++j)
                    cout << "+---";
            }
            else
            {
                for (int j = 0; j < 9; ++j)
                    cout << "+---";
            }
            cout << "+\n";
            cout << "| ";
            for (int j = 0; j < 9; ++j)
            {
                cout << board[i][j];
                if (j % 3 == 2)
                    cout << " | ";
                else
                    cout << " | ";
            }
            cout << "\n";
        }
        for (int j = 0; j < 9; ++j)
            cout << "+---";
        cout << "+\n";
    }
}

// function to check if the  sudoku is solved
bool check(vector<vector<int>> &arr)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (!arr[i][j])
                return 0;
    return 1;
}

// calculating for every number all positions it can potentially fit in for the given board
array<vector<vector<int>>, 10> calculate_fitness(vector<vector<int>> &board)
{
    array<vector<vector<int>>, 10> fit;
    for (int i = 0; i < 10; ++i)
        fit[i].resize(9, vector<int>(9));
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (!board[row][col])
                continue;
            int x = board[row][col];
            // rows
            for (int i = 0; i < 9; ++i)
                fit[x][row][i] = 1;
            // columns
            for (int i = 0; i < 9; ++i)
                fit[x][i][col] = 1;
            // 3x3 squares
            for (pii &p : tiles[sqr[row][col]])
            {
                fit[x][p.first][p.second] = 1;
            }
        }
    }
    return fit;
}

// function to recursively try to solve the sudoku for the given board
vector<vector<int>> try_solve(vector<vector<int>> board, int depth, int max_depth)
{
    // phase 1:
    // filling every tile that has only one candidate for the number until there are no such tiles left
    vector<vector<int>> last = board;
    while (!check(board))
    {
        array<vector<vector<int>>, 10> fit = calculate_fitness(board);
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                if (board[row][col])
                    continue;
                int cnt = 0, pos = 0;
                for (int i = 1; i <= 9; ++i)
                    if (!fit[i][row][col])
                    {
                        ++cnt;
                        pos = i;
                    }
                if (cnt == 1)
                {
                    board[row][col] = pos;
                    goto stop_checking;
                }
            }
        }
        // checking rows
        for (int row = 0; row < 9; ++row)
        {
            for (int num = 1; num <= 9; ++num)
            {
                for (int i = 0; i < 9; ++i)
                    if (board[row][i] == num)
                        continue;
                int pos = -1, cnt = 0;
                for (int i = 0; i < 9; ++i)
                {
                    if (board[row][i])
                        continue;
                    if (!fit[num][row][i])
                    {
                        pos = i;
                        ++cnt;
                    }
                }
                if (cnt == 1)
                {
                    board[row][pos] = num;
                    goto stop_checking;
                }
            }
        }
        // checking columns
        for (int col = 0; col < 9; ++col)
        {
            for (int num = 1; num <= 9; ++num)
            {
                for (int i = 0; i < 9; ++i)
                    if (board[i][col] == num)
                        continue;
                int pos = -1, cnt = 0;
                for (int i = 0; i < 9; ++i)
                {
                    if (board[i][col])
                        continue;
                    if (!fit[num][i][col])
                    {
                        pos = i;
                        ++cnt;
                    }
                }
                if (cnt == 1)
                {
                    board[pos][col] = num;
                    goto stop_checking;
                }
            }
        }
        // checking 3x3 sqaures
        for (int idx = 0; idx < 9; ++idx)
        {
            for (int num = 1; num <= 9; ++num)
            {
                for (pii &p : tiles[idx])
                    if (board[p.first][p.second] == num)
                        continue;
                pii pos = {-1, -1};
                int cnt = 0;
                for (pii &p : tiles[idx])
                {
                    if (board[p.first][p.second])
                        continue;
                    if (!fit[num][p.first][p.second])
                    {
                        pos = p;
                        ++cnt;
                    }
                }
                if (cnt == 1)
                {
                    board[pos.first][pos.second] = num;
                    goto stop_checking;
                }
            }
        }
        // phase 2:
        // recursively trying to solve the board by substituting numbers in tiles 
        // where there is no certainity about occupancy 
        // if the entire board was filled in phase 1 this part is skipped
        
        // rows
        if (depth == max_depth)
            return vector<vector<int>>(9, vector<int>(9));
        for (int row = 0; row < 9; ++row)
        {
            for (int num = 1; num <= 9; ++num)
            {
                for (int i = 0; i < 9; ++i)
                    if (board[row][i] == num)
                        continue;
                for (int i = 0; i < 9; ++i)
                {
                    if (board[row][i])
                        continue;
                    if (!fit[num][row][i])
                    {
                        vector<vector<int>> _board = board;
                        _board[row][i] = num;
                        vector<vector<int>> res = try_solve(_board, depth + 1, max_depth);
                        if (check(res))
                            return res;
                    }
                }
            }
        }
        // columns
        for (int col = 0; col < 9; ++col)
        {
            for (int num = 1; num <= 9; ++num)
            {
                for (int i = 0; i < 9; ++i)
                    if (board[i][col] == num)
                        continue;
                for (int i = 0; i < 9; ++i)
                {
                    if (board[i][col])
                        continue;
                    if (!fit[num][i][col])
                    {
                        vector<vector<int>> _board = board;
                        _board[i][col] = num;
                        vector<vector<int>> res = try_solve(_board, depth + 1, max_depth);
                        if (check(res))
                            return res;
                    }
                }
            }
        }
        // 3x3 squares
        for (int idx = 0; idx < 9; ++idx)
        {
            for (int num = 1; num <= 9; ++num)
            {
                for (pii &p : tiles[idx])
                    if (board[p.first][p.second] == num)
                        continue;
                for (pii &p : tiles[idx])
                {
                    if (board[p.first][p.second])
                        continue;
                    if (!fit[num][p.first][p.second])
                    {
                        vector<vector<int>> _board = board;
                        _board[p.first][p.second] = num;
                        vector<vector<int>> res = try_solve(_board, depth + 1, max_depth);
                        if (check(res))
                            return res;
                    }
                }
            }
        }
        // for debugging purposes
        //~ print_board(board);
        return board;
    stop_checking:
        if (board == last)
            break;
        last = board;
    }
    return board;
}

// initialize 3x3 squares positions for faster computations
void init()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            sqr[i][j] = (i / 3) * 3 + j / 3;
            tiles[(i / 3) * 3 + j / 3].pb({i, j});
        }
    }
}

// solve the board for given input (cin)
void solve()
{
    vector<vector<int>> board(9, vector<int>(9));
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            cin >> board[i][j];
    //max depth of the recursion
    int max_depth = 50;
    vector<vector<int>> res = try_solve(board, 0, max_depth);
    print_board(res, 1);
}

int main()
{
    ios_base::sync_with_stdio(0);
    init();
    solve();
}
