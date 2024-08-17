#include "board.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

const int Board::UNCLEARED_VALUE = -1;
const int Board::CLEARED_VALUE = 0;
const int Board::MINE_VALUE = -2;
const int Board::FLAG_VALUE = -3;
const int Board::X_DIMENSION = 8;
const int Board::Y_DIMENSION = 8;

Board::Board(int mineCount) {
    gameOver = false;
    minesLeft = mineCount;
    fullBoard = std::vector<std::vector<int>>(Y_DIMENSION, std::vector<int>(X_DIMENSION, UNCLEARED_VALUE));
    visibleBoard = std::vector<std::vector<int>>(Y_DIMENSION, std::vector<int>(X_DIMENSION, UNCLEARED_VALUE));

    srand(time(NULL));
    while (mineCount > 0) {
        int mineXCoord = rand() % X_DIMENSION;
        int mineYCoord = rand() % Y_DIMENSION;

        if (fullBoard[mineYCoord][mineXCoord] != MINE_VALUE) {
            fullBoard[mineYCoord][mineXCoord] = MINE_VALUE;
            mineCount--;
        }
    }

    for (int i=0; i<Y_DIMENSION; i++) {
        for (int j=0; j<X_DIMENSION; j++) {
            fullBoard[i][j] = fullBoard[i][j] == MINE_VALUE ? MINE_VALUE : countMines(i, j, fullBoard);
        }
    }
}

void Board::click(int y_coord, int x_coord) {
    if (gameOver || !isOnBoard(y_coord, x_coord)) 
        return;
    if (visibleBoard[y_coord][x_coord] != UNCLEARED_VALUE) 
        return; 

    if (fullBoard[y_coord][x_coord] == MINE_VALUE) {
        gameOver = true;
        return;
    }
    clearAdjacent(y_coord, x_coord);
}

void Board::chord(int y_coord, int x_coord) {
    if (gameOver || !isOnBoard(y_coord, x_coord))
        return;
    if ((!(visibleBoard[y_coord][x_coord] > 0) && countMines(y_coord, x_coord, visibleBoard) != visibleBoard[y_coord][x_coord]))
        return;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord)) click(i, j);
        }
    }
}

void Board::flag(int y_coord, int x_coord) {
    if (gameOver || !isOnBoard(y_coord, x_coord))
        return;
    if (visibleBoard[y_coord][x_coord] != UNCLEARED_VALUE) 
        return;

    visibleBoard[y_coord][x_coord] = MINE_VALUE;
}

int Board::countMines(int y_coord, int x_coord, std::vector<std::vector<int>> board) {
    int numMines = 0;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if ( !(i == y_coord && j == x_coord) && board[i][j] == MINE_VALUE) 
                numMines++;
        }
    }
    return numMines;
}

bool Board::isOnBoard(int y_coord, int x_coord) {
    return y_coord >= 0 && x_coord >= 0 && y_coord < Y_DIMENSION && x_coord < X_DIMENSION;
}

void Board::clearAdjacent(int y_coord, int x_coord) {
    if (!isOnBoard(y_coord, x_coord))
        return;
    if ((fullBoard[y_coord][x_coord] == MINE_VALUE) || (visibleBoard[y_coord][x_coord] != UNCLEARED_VALUE))
        return;

    visibleBoard[y_coord][x_coord] = fullBoard[y_coord][x_coord];
    
    if (fullBoard[y_coord][x_coord] != CLEARED_VALUE) 
        return;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord)) clearAdjacent(i, j);
        }
    }

}

