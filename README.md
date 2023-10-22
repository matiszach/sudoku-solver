# sudoku-solver
A program for finding Sudoku solutions

# Algorithm

My program uses a 2-phase recursive search in order to find one of possibly many valid solutions to the given Sudoku state.

## Phase 1

The first phase of the algorithm tries to deterministically fill as many Sudoku tiles as it can.
For this purpose, it calculates a fitness table that holds for every tile and every digit whether a digit can be placed in the tile.

#### The fitness table is calculated based on checking if the digit was already placed in the row, column, or 3x3 square where the tile is located.

If for a certain tile, there is only one available digit in the fitness table, then it has to be placed there.

Phase 1 is repeated until there are no tiles where only one digit fits in.
When phase 1 is unable to fill any new tiles then the algorithm moves to phase 2.

## Phase 2

In the second phase, the algorithm takes the fitness table and goes through the empty tiles.

For every one of those tiles, it creates a copy of the current Sudoku state and fills the tile with one of the possible digits from the fitness table.

Then, the algorithm tries to recursively solve the new Sudoku state and moves to phase 1.

The recursion stops when the entire Sudoku is solved.

# Analysis

The second phase of the algorithm is backtracking all the possible choices of digits for every tile so it may seem that the program is pretty slow as the complexity is clearly exponential. 

However, the first phase speeds the algorithm substantially and the program works extremely fast for starting states with as few as 16 tiles filled (which is considered to be hard to solve by humans). 

## But why?

For most Sudoku states the number of tiles that can be filled deterministically is correlated with the number of already filled tiles, because the fitness table is based on the existing digits, and more of them means fewer digits fitting in every tile.

When there are very few filled tiles, phase 1 is unable to help by much, but when the number of filled tiles is increasing the first phase starts to speed up the process of filling more and more.

Therefore, the recursion has only to go a few steps deep in order for the first phase to speed up the filling process.

## Limitations

Because the initial recursion is very slow the starting state needs at least about 16 filled tiles to prevent recursion from going too deep into search.

# Usage

To use the program you need to compile it first using C++ 17 or higher.
```
g++ -std=c++17 -o sudokusolver sudokusolver.cpp
```
Then run it

```
./sudokusolver
```
The program takes as an input a 9 x 9 grid of space-separated digits 0 - 9, where 0 indicates an empty tile (There is a folder called examples with sample inputs)

Program outputs solved the Sudoku grid separated by ASCII characters. (They can be disabled by calling the print_board function with mode = false)

## Example Input

```
0 0 0 0 0 4 0 0 0
0 0 0 0 0 0 2 0 9
2 0 9 0 7 0 0 4 0
0 0 0 0 0 5 0 8 0
0 0 0 3 0 7 0 0 0
0 5 0 0 0 0 0 1 0
0 8 0 0 3 0 4 0 1
7 0 0 0 0 0 0 0 0
0 0 6 4 0 0 0 0 0
```
## Example Output

```
+---+---+---+---+---+---+---+---+---+
| 1 | 3 | 5 | 2 | 9 | 4 | 6 | 7 | 8 | 
+---+---+---+---+---+---+---+---+---+
| 4 | 7 | 8 | 1 | 6 | 3 | 2 | 5 | 9 | 
+---+---+---+---+---+---+---+---+---+
| 2 | 6 | 9 | 5 | 7 | 8 | 1 | 4 | 3 | 
+---+---+---+---+---+---+---+---+---+
| 6 | 1 | 3 | 9 | 2 | 5 | 7 | 8 | 4 | 
+---+---+---+---+---+---+---+---+---+
| 8 | 2 | 4 | 3 | 1 | 7 | 5 | 9 | 6 | 
+---+---+---+---+---+---+---+---+---+
| 9 | 5 | 7 | 8 | 4 | 6 | 3 | 1 | 2 | 
+---+---+---+---+---+---+---+---+---+
| 5 | 8 | 2 | 7 | 3 | 9 | 4 | 6 | 1 | 
+---+---+---+---+---+---+---+---+---+
| 7 | 4 | 1 | 6 | 8 | 2 | 9 | 3 | 5 | 
+---+---+---+---+---+---+---+---+---+
| 3 | 9 | 6 | 4 | 5 | 1 | 8 | 2 | 7 | 
+---+---+---+---+---+---+---+---+---+
```
