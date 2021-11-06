#pragma once

#include <vector>
#include <random>


#define PATH_SCROLL_CHANCE 20
#define SPEED_SCROLL_CHANCE 40
#define DASH_SCROLL_CHANCE 70

#include "../logger.hpp"
#include "maze.h"

/**
 * @file checkedMaze.h
 *
 * @brief Declaration of the CheckedMaze class
 */

/**
 * @brief The CheckedMaze is responsible for checking if the generated maze is correct, formatting the maze, and enriching it with runes.
 */

class CheckedMaze
{
public:
    CheckedMaze(int width, int height);
    
    Logger logger;

    ~CheckedMaze();

    // Solvable checked maze returned by the program
    char* checkedMaze;

    // Checked maze but with an optimal path in it
    char* mazeWithPath;

    char* reversedMaze;

public:
 /**
 * @brief Recursively checks if the maze is solvable, and if it is not, generates a new one. 
 *
 * The function 
 *
 * @param accList list& the list of account
 *                objects created by getAccounts()
 * @return void
 */
    void checkMaze();

    // Used for debugging purposes
    void printCheckedMazeWithPath();
    // Used for debugging purposes
    void printCheckedMaze();
    // Used for debugging purposes
    void printReversedMaze();

    // Getter for width
    int getWidth() const;
    // Getter for height
    int getHeight() const;


private:
    Maze* maze = NULL; //!< @brief Maze class-variable, used for providing a generated maze.

    int m_mWidth; //!< @brief Width of the Maze
    int m_mHeight; //!< @brief Height of the Maze

    int m_possibleWays; //!< @brief Every cell that is not a wall.

    // Current coordinates
    int m_x; 
    int m_y; 

    // Number of already visited cells
    int m_nVisitedCells;

    // Every cell that has already been visited
    std::vector<std::pair<int, int>> m_visitedCells;

    // Stack used for backtracking in the process of solving the maze
    std::vector<std::pair<int, int>> m_stack;

    std::vector<bool> pathScrollChance;
    std::vector<bool> speedScrollChance;
    std::vector<bool> dashScrollChance;

private:

    // Checks if a coordinate is in the maze or outside of it
    bool isInBound(const int& x, const int& y) const;

    // Returns every not-checked-already neighbour of a cell
    std::vector<std::pair<int, int>> getNeighbours(const int& x, const int& y) const;

    // Checks if a cell has been visited
    bool searchForAlreadyVisitedCells(const int& x, const int& y) const;

    // Converts 2D coordinates into 1D
    int toIndex(const int& x, const int& y) const;

    // Removes the markings in the maze and replaces them with '0'
    void fixMaze();

    // Copies the initial maze with its path into a new public variable
    void initMazeWithPath();

    // Copies the checked maze and reverses it
    void initReversedMaze();

    void fillChanceGenerator(std::vector<bool>& pathVector,
                             std::vector<bool>& speedVector,
                             std::vector<bool>& dashVector);

    void shuffleGenerators(std::vector<bool>& pathVector,
                           std::vector<bool>& speedVector,
                           std::vector<bool>& dashVector);

    void enrichMaze();
};