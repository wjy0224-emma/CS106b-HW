// 迷宫处理：生成合法移动、验证路径、搜索路线并读入迷宫数据。
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 返回当前位置在迷宫中可直接到达的所有相邻通道格。 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    GridLocation next;
    next={cur.row-1,cur.col};;
    if(maze.inBounds(next.row,next.col) && maze[next.row][next.col]){
        neighbors.add(next);
    }
    next={cur.row,cur.col-1};
    if(maze.inBounds(next.row,next.col) && maze[next.row][next.col]){
        neighbors.add(next);
    }
    next={cur.row+1,cur.col};
    if(maze.inBounds(next.row,next.col) && maze[next.row][next.col]){
        neighbors.add(next);
    }
    next={cur.row,cur.col+1};
    if(maze.inBounds(next.row,next.col) && maze[next.row][next.col]){
        neighbors.add(next);
    }

    return neighbors;
}


/*
 * 验证路径是否从入口连续走到出口，且不越界、不穿墙、不重复位置；
 * 路径按值传入，因此检查过程可以安全地弹出其中的元素。
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */
    Set<GridLocation> visited;
    while(!path.isEmpty()){
        GridLocation cur=path.pop();
        if(cur.row<0 || cur.row>=maze.numRows() || cur.col<0 || cur.col>=maze.numCols()){
            error("Path goes out of maze bounds");
        }
        if(!maze[cur.row][cur.col]) error("Path goes through a wall");
        if(visited.contains(cur)) error("Path contains a repeated location");
        visited.add(cur);

        if(!path.isEmpty()){
            GridLocation pre=path.peek()
            Set<GridLocation> neighbors=generateValidMoves(maze,pre);
            if(!neighbors.contains(cur)) {
                error("Path contains an invalid move");
            }
        }
        else {
            if(cur!={0,0}) {
                error("Path does not start at maze entrance");
            }
        }
    }
    /* If you find a problem with the solution, call error() to report it.
     * If the path is valid, then this function should run to completion
     * without throwing any errors.
     */
}

/* 使用广度优先搜索寻找从左上角入口到右下角出口的一条有效路径。 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);

    Queue<Stack<GridLocation>> paths;

    GridLocation start={0,0};
    GridLocation end={maze.numRows()-1,maze.numCols()-1};

    Set<GridLocation> visited;

    Stack<GridLocation> startPath;
    startPath.push(start);

    paths.enqueue(startPath);
    visited.add(start);

    while(!paths.isEmpty()){
        Stack<GridLocation> path=paths.dequeue();
        GridLocation cur=path.peek();

        if(cur==end){
            return path;
        }

        Set<GridLocation> neighbors=generateValidMoves(maze,cur);
    
        for(GridLocation neighbor : neighbors){
            if (!visited.contains(neighbor)){
                visited.add(neighbor);

                Stack<GridLocation> newPath=path;
                newPath.push(neighbor);
                paths.enqueue(newPath);
            }
        }
    }
    error("Maze has no solution");
}

/* 读取迷宫文件，并将墙壁和通道转换为布尔网格。 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following lines read in the data from the file into a Vector
     * of strings representing the lines of the file. We haven't talked
     * in class about what ifstreams are, so don't worry if you don't fully
     * understand what is going on here.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            }
        }
    }
}

/* 读取 .soln 文件，将其中的位置序列解析并保存到路径栈中。 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() != 1){
        error("File contained too many or too few lines.");
    }

    istringstream istr(lines[0]); // Stack read does its own error-checking
    if (!(istr >> soln)) {// if not successfully read
        error("Solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateNeighbors on location in the center of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateNeighbors on location on the side of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateNeighbors on corner of 2x2 grid with walls"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}


PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/2x2.maze", maze));
}

PROVIDED_TEST("Test readMazeFile on valid file 5x7.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/5x7.maze", maze));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

PROVIDED_TEST("readMazeFile on malformed file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
}
