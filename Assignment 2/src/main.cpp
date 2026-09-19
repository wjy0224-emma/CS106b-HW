#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "maze.h"
#include "search.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

/* 运行选定测试，然后启动迷宫求解与文本搜索程序。 */
int main()
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    solveMaze(maze);


    searchEngine("res/website.txt");

    cout << endl << "All done!" << endl;
    return 0;
}
