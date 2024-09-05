#include "solver.hpp"

int main() {
    int x = 10;
    int num = 0;
    srand(time(NULL));
    while (x--) {
        Board b(12);
        Solver s(b);
  
        while (!s.board.gameOver) {
            s.clearTrivial();
            Solver::Solution solve = s.testCasesByTile();
            if (solve.clear.size() == 0 && solve.mines.size() == 0) {
                s.guaranteedClick();
            } else {
                s.printBoard(); std::cout << std::endl;
                s.makeMove(solve);
                num++;
            }
        }
    }
    std::cout << num;

    // Board b(12);
    // Solver s(b);
    // s.guaranteedClick();
    // s.guaranteedClick();
    // s.guaranteedClick();
    // s.guaranteedClick();
    // s.guaranteedClick();
    // Solver::Solution solve = s.testCasesByTile();

    // s.printBoard();
    // s.makeMove(solve);
    // std::cout << std::endl;
    // s.printBoard();
    // std::cout << "full" << std::endl;
    // for (int i=0; i<8; i++) {
    //     for (int j=0; j<8; j++) {
    //         if (s.board.fullBoard[i][j] >= 0 )
    //             std::cout << " " << s.board.fullBoard[i][j] << "\t";
    //         else 
    //             std::cout << s.board.fullBoard[i][j] << "\t";
    //     }
    //     std::cout << std::endl;
    // }

    return 0;

}