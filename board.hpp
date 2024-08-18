#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

class Board {

    public: 
    static const int UNCLEARED_VALUE;
    static const int CLEARED_VALUE;
    static const int MINE_VALUE ;
    static const int FLAG_VALUE;
    static const int X_DIMENSION;
    static const int Y_DIMENSION;

    std::vector<std::vector<int>> visibleBoard;
    std::vector<std::vector<int>> fullBoard;
    // int visibleBoard[Y_DIMENSION][X_DIMENSION];
    // int fullBoard[Y_DIMENSION][X_DIMENSION];

    int minesLeft;
    int totalMines;
    bool gameOver;

    Board(int mineCount);
    void click(int y_coord, int x_coord);
    void chord(int y_coord, int x_coord);
    void flag(int y_coord, int x_coord);
    int countMines(int y_coord, int x_coord, std::vector<std::vector<int>> board);
    //int countMines(int y_coord, int x_coord, int board[Y_DIMENSION][X_DIMENSION]);

    private: 
    //std::vector<std::vector<int>> fullBoard;
    bool isOnBoard(int y_coord, int x_coord);
    void clearAdjacent(int y_coord, int x_coord);
};