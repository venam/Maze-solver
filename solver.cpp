#include <iostream>
#include <string>
#include "maze_solver.hpp"

int main()
{
    maze_solver my_maze('S','E','X','o');
    my_maze.solve("test_maze");
    my_maze.print();
    return 0;
}
