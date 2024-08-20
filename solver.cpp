#include "solver.hpp"

Solver::Solver(Board b) : board(std::move(b)) {};

void Solver::clearTrivial() {
    bool changeMade = true;
    while (changeMade) {
        changeMade = false;

        for (int i=0; i<board.Y_DIMENSION; i++) {
            for (int j=0; j<board.X_DIMENSION; j++) {
                if (board.visibleBoard[i][j] <= 0 || board.countUncleared(i, j) == 0) 
                    continue;
                if (board.visibleBoard[i][j] == board.countMines(i, j) + board.countUncleared(i, j)) {
                    board.flagSurrounding(i, j);
                    changeMade = true;
                }
                if (board.visibleBoard[i][j] == board.countMines(i, j)) {
                    board.chord(i, j);
                    changeMade = true;
                }
            }    
        }
    }
}