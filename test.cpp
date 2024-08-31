#include "solver.hpp"

int main() {

    Board b(15);
    Solver s(b);
    s.guaranteedClick();

    while (!s.board.gameOver) {
        s.clearTrivial();
        Solver::Solution solve = s.testAllCases();
        if (solve.clear.size() != 0 || solve.mines.size() != 0) {
            s.makeMove(solve);
        } else {
            s.guaranteedClick();
        }
        //s.guaranteedClick();
        for (int i=0; i<Board::Y_DIMENSION; i++) {
            for (int j=0; j<Board::X_DIMENSION; j++) {
                std::cout << s.board.visibleBoard[i][j] << "\t";
            }
            std::cout << "\n";
        }
    } 
    std::cout << "END BOARD: " << std::endl;
    for (int i=0; i<Board::Y_DIMENSION; i++) {
        for (int j=0; j<Board::X_DIMENSION; j++) {
            std::cout << s.board.visibleBoard[i][j] << "\t";
        }
        std::cout << "\n";
    }
    return 0;
}