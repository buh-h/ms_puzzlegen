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
                Solver::Solution solve = s.testCasesByTile();
                if (solve.clear.size() == 0 && solve.mines.size() == 0) {
                    s.guaranteedClick();
                } else {

                    std::string boardString = "";
                    std::string clearPointsString = "";
                    std::string minePointsString = "";
                    for (std::vector<int> row : s.board.visibleBoard) {
                        for (int i : row) {
                            boardString += std::to_string(i) + " ";
                        }
                    }
                    for (Solver::Point clearPoint : solve.clear) {
                        clearPointsString += "(" + std::to_string(clearPoint.y) 
                                                    + "," + std::to_string(clearPoint.x) + ") ";
                    }
                    for (Solver::Point minePoint : solve.mines) {
                        minePointsString += "(" + std::to_string(minePoint.y) 
                                                    + "," + std::to_string(minePoint.x) + ") ";
                    }
                    out << boardString << "\n" << clearPointsString << "\n" << minePointsString << "\n";
                    s.makeMove(solve);
                    num++;
                }
            }
        }
    }
    std::cout << num;
    out.close();
}