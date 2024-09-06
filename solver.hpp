#pragma once
#include "board.hpp"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

class Solver {

    public: 
    struct Point {
        int y, x;
        Point();
        Point(int y, int x);
        bool operator==(const Point &other) const;
    };
    struct PointHash {
        std::size_t operator()(const Point &p) const;
    };
    struct Solution {
        std::vector<Point> clear, mines;
    };

    Solver(Board b);
    Board board;

    void clearTrivial();
    void guaranteedClick();
    Solution testMinecountCases();
    Solution testCasesByTile();
    void makeMove(Solution moves);
    void printBoard();
    // std::vector<std::vector<int>> generateCombinations(int numMines, int numUncleared);
    // std::vector<std::vector<int>> generateAllCombinations(int totalMines, int numUncleared);

    private:
    bool isBorderUncleared(int y_coord, int x_coord);
    bool isBorderNumber(int y_coord, int x_coord);
    std::vector<Point> getSurroundingUncleared(int y_coord, int x_coord, Board testBoard);
    std::vector<Point> getAllBorderingUncleared();
    std::vector<Point> getAllBorderNumbers();
    std::vector<std::vector<int>> generateCombinations(int numMines, int numUncleared);
    std::vector<std::vector<int>> generateAllCombinations(int totalMines, int numUncleared);
    bool isTileValid(int y_coord, int x_coord, std::vector<std::vector<int>> testBoard);
    bool testCombination(Board testBoard, std::unordered_map<Point, int, PointHash>& clearMap, 
                                std::unordered_map<Point, int, PointHash>& minesMap);
}; 