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

    int minesLeft;
    int numCleared;
    int totalMines;
    bool gameOver;

    Board(int mineCount);
    void click(int y_coord, int x_coord);
    void chord(int y_coord, int x_coord);
    void flag(int y_coord, int x_coord);
    void flagSurrounding(int y_coord, int x_coord);
    int countMines(int y_coord, int x_coord);
    int countUncleared(int y_coord, int x_coord);

    private: 
    bool isOnBoard(int y_coord, int x_coord);
    int countMinesOnFull(int y_coord, int x_coord);
    void clearAdjacent(int y_coord, int x_coord);

};