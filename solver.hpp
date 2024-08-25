#pragma once
#include "board.hpp"
#include <algorithm>

class Solver {

    public: 
    struct Point {
        int y; 
        int x;
    };
    struct Solution {
        std::vector<Point> clear;
        std::vector<Point> mines;
    };

    Solver(Board b);

    void clearTrivial();
    void guaranteedClick();
    Solution testAllCases();
    Solution testCasesForTile(int y_coord, int x_coord);
    void makeMove(Solution moves);

    private:
    Board board;
    bool isBorderTile(int y_coord, int x_coord);
    std::vector<Point> getAllBorderingUncleared();
    std::vector<Point> getSurroundingUncleared(int y_coord, int x_coord);
    std::vector<std::vector<bool>> generateCombinations(int numMines, int numUncleared);
    std::vector<std::vector<bool>> generateAllCombinations(int totalMines, int numUncleared);

}; 