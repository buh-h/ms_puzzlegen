#pragma once
#include "board.hpp"

class Solver {

    public: 
    struct Point {
        int x; 
        int y;
    };
    struct Solution {
        std::vector<Point> clear;
        std::vector<Point> mines;
    };

    Solver(Board b);

    void clearTrivial();
    void guessRandomly();
    void guaranteedClick();
    Solution testCases();
    Solution testCasesWithCount();

    private:
    Board board;
}; 