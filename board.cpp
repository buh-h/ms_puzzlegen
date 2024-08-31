#include "board.hpp"

const int Board::UNCLEARED_VALUE = -1;
const int Board::CLEARED_VALUE = 0;
const int Board::MINE_VALUE = -2;
const int Board::FLAG_VALUE = -3;
const int Board::X_DIMENSION = 8;
const int Board::Y_DIMENSION = 8;

Board::Board(int mineCount) {
    gameOver = false;
    minesLeft = mineCount;
    totalMines = mineCount;
    numCleared = 0;
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
            fullBoard[i][j] = fullBoard[i][j] == MINE_VALUE ? MINE_VALUE : countMinesOnFull(i, j);
        }
    }
}

void Board::click(int y_coord, int x_coord) {
    bool isValidClick = visibleBoard[y_coord][x_coord] == UNCLEARED_VALUE;
    if (gameOver || !isOnBoard(y_coord, x_coord) || !isValidClick) 
        return;

    if (fullBoard[y_coord][x_coord] == MINE_VALUE) {
        gameOver = true;
        std::cout << "Clicked on mine at X: " << x_coord << " Y: " << y_coord << std::endl;
        return;
    }
    clearAdjacent(y_coord, x_coord);

    // Checks if game was won by comparing the visible and full board
    if (numCleared == X_DIMENSION * Y_DIMENSION - totalMines) {
        gameOver = true;
        std::cout << "You won" << std::endl;
    }
}

void Board::chord(int y_coord, int x_coord) {
    bool isValidChord = visibleBoard[y_coord][x_coord] > 0 
                            && countMines(y_coord, x_coord) == visibleBoard[y_coord][x_coord];
    if (gameOver || !isOnBoard(y_coord, x_coord) || !isValidChord)
        return;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && (visibleBoard[i][j] != FLAG_VALUE)) click(i, j);
        }
    }
}

void Board::flag(int y_coord, int x_coord) {
    bool isValidFlag = visibleBoard[y_coord][x_coord] == UNCLEARED_VALUE;
    if (gameOver || !isOnBoard(y_coord, x_coord) || !isValidFlag)
        return;

    visibleBoard[y_coord][x_coord] = FLAG_VALUE;
    minesLeft--;
}

void Board::flagSurrounding(int y_coord, int x_coord) {
    bool isValidMove = visibleBoard[y_coord][x_coord] > 0;
    if (gameOver || !isOnBoard(y_coord, x_coord) || !isValidMove)
        return;
    
    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && (visibleBoard[i][j] == UNCLEARED_VALUE)) flag(i, j);
        }
    }
}

int Board::countMines(int y_coord, int x_coord) {
    int mineCount = 0;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && visibleBoard[i][j] == FLAG_VALUE) 
                mineCount++;
        }
    }
    return mineCount;
}

int Board::countUncleared(int y_coord, int x_coord) {
    int unclearedCount = 0;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && visibleBoard[i][j] == UNCLEARED_VALUE) 
                unclearedCount++;
        }
    }
    return unclearedCount;
}

bool Board::isOnBoard(int y_coord, int x_coord) {
    return y_coord >= 0 && x_coord >= 0 && y_coord < Y_DIMENSION && x_coord < X_DIMENSION;
}

int Board::countMinesOnFull(int y_coord, int x_coord) {
    int mineCount = 0;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && fullBoard[i][j] == MINE_VALUE) 
                mineCount++;
        }
    }
    return mineCount;
}

void Board::clearAdjacent(int y_coord, int x_coord) {
    if (!isOnBoard(y_coord, x_coord))
        return;
    if ((fullBoard[y_coord][x_coord] == MINE_VALUE) || (visibleBoard[y_coord][x_coord] != UNCLEARED_VALUE))
        return;

    visibleBoard[y_coord][x_coord] = fullBoard[y_coord][x_coord];
    numCleared++;
    
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

