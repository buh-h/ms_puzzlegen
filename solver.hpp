#pragma once
#include "board.hpp"
#include <unordered_map>
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

    void clearTrivial();
    void guaranteedClick();
    Solution testAllCases();
    void makeMove(Solution moves);

    private:
    Board board;
    bool isBorderUncleared(int y_coord, int x_coord);
    bool isBorderNumber(int y_coord, int x_coord);
    std::vector<Point> getAllBorderingUncleared();
    std::vector<Point> getAllBorderNumbers();
    std::vector<std::vector<int>> generateCombinations(int numMines, int numUncleared);
    std::vector<std::vector<int>> generateAllCombinations(int totalMines, int numUncleared);
    bool isTileValid(int y_coord, int x_coord, std::vector<std::vector<int>> testBoard);

}; 