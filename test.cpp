#include "solver.hpp"

int main() {
    int x = 1000;
    while (x--) {
        Board b(12);
        Solver s(b);
        while (!s.board.gameOver) {
            s.clearTrivial();
            Solver::Solution solve = s.testAllCases();
            if (solve.clear.size() == 0 && solve.mines.size() == 0) {
                s.guaranteedClick();
            } else {
                s.makeMove(solve);
            }
        }
    }
    return 0;
}