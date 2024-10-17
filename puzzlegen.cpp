#include "solver.hpp"
#include <fstream>
#include <string>

int main() {
    srand(time(NULL));
    std::ofstream out;
    out.open("puzzles.txt");

    if (!out) {
        return 1;
    }
    int num = 0;
    for (int n=10; n<25; n++){
        int x = 25;

        while (x--) {
            Board b(n);
            Solver s(b);
    
            while (!s.board.gameOver) {
                s.clearTrivial();
                Solver::Solution solve = s.testNxNTiles(3);
                if (solve.clear.size() == 0 && solve.mines.size() == 0) {
                    s.guaranteedClick();
                } else {

                    std::string boardString = "";
                    std::string clearPointsString = "";
                    std::string minePointsString = "";
                    for (int i=0; i<Board::Y_DIMENSION; i++) {
                        for (int j=0; j<Board::X_DIMENSION; j++) {
                            if (std::find(solve.clear.begin(), solve.clear.end(), Solver::Point(i,j)) != solve.clear.end()) {
                                boardString += "O ";
                            } 
                            else if (std::find(solve.mines.begin(), solve.mines.end(), Solver::Point(i,j)) != solve.mines.end()) {
                                boardString += "X ";
                            } else {
                                boardString += std::to_string(s.board.visibleBoard[i][j]) + " ";
                            }
                        }
                    }
                    // for (Solver::Point clearPoint : solve.clear) {
                    //     clearPointsString += "(" + std::to_string(clearPoint.y) 
                    //                                 + "," + std::to_string(clearPoint.x) + ") ";
                    // }
                    // for (Solver::Point minePoint : solve.mines) {
                    //     minePointsString += "(" + std::to_string(minePoint.y) 
                    //                                 + "," + std::to_string(minePoint.x) + ") ";
                    // }
                    out << boardString << "\n";
                    // out << boardString << "\n" << clearPointsString << "\n" << minePointsString << "\n";
                    s.makeMove(solve);
                    num++;
                }
            }
        }
    }
    std::cout << num;
    out.close();
}