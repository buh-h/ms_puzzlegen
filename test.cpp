#include "solver.hpp"

void testfuncs() {
    Solver s(Board(12));    

    std::vector<std::vector<int>> test = {
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {1,1,1,-1,-1,-1,-1,-1},
        {-0,-0,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1}

    };
    s.board.visibleBoard = test;

    Solver::Solution a = s.test2x2Tiles();
    std::cout << "clear\n";
    for (Solver::Point& p: a.clear) {
        std::cout << "y: " << p.y << " x: " << p.x << "\n";
    }
    std::cout << "mines\n";
    for (Solver::Point& p: a.mines) {
        std::cout << "y: " << p.y << " x: " << p.x << "\n";
    }
    // std::vector<Solver::Point> nums;
    // s.get2x2BorderNumbers(0, 1, nums);
    // std::vector<Solver::Point> tiles;
    // s.getBorderingUnclearedForList(nums, tiles);
    // std::vector<std::vector<Solver::Point>> cases;
    // s.findCasesDFS(nums, 0, std::vector<Solver::Point>(), cases, s.board);

    // for (Solver::Point& p: tiles) {
    //     std::cout << "y: " << p.y << " x: " << p.x << "\n";
    // }
    // std::cout << "Cases\n"; 
    // int n = 1;
    // for (std::vector<Solver::Point>& v: cases) {
    //     std::cout << "case " << n << "\n";
    //     for (Solver::Point& p: v) {
    //         std::cout << "y: " << p.y << " x: " << p.x << "\n";
    //     }
    //     n++;
    // }

    //s.printBoard();

}


int main() {

    //testfuncs();
    int x = 100;
    int num = 0;
    int one = 0;
    int two = 0;
    srand(time(NULL));
    while (x--) {
        Board b(12);
        Solver s(b);
  
        while (!s.board.gameOver) {
            s.clearTrivial();
            Solver::Solution solve = s.testCasesByTile();
            Solver::Solution solve2 = s.test2x2Tiles();
            if (solve.clear.size() == 0 && solve.mines.size() == 0) {
                std::cout << "guessing" << std::endl;
                s.guaranteedClick();
            } else {
                std::cout << "old Clear tiles: " << std::endl;
                for (Solver::Point point : solve.clear) {
                    std::cout << point.y << " " << point.x << std::endl;
                }
                std::cout << "old Mine tiles: " << std::endl;
                for (Solver::Point point : solve.mines) {
                    std::cout << point.y << " " << point.x << std::endl;
                }

                std::cout << "new Clear tiles: " << std::endl;
                if (solve2.clear.size() == 0) std::cout << "none\n";
                for (Solver::Point point : solve2.clear) {
                    std::cout << point.y << " " << point.x << std::endl;
                }
                std::cout << "new Mine tiles: " << std::endl;
                if (solve2.mines.size() == 0) std::cout << "none\n";
                for (Solver::Point point : solve2.mines) {
                
                    std::cout << point.y << " " << point.x << std::endl;
                }
                if (solve2.clear.size() > solve.clear.size() || solve2.mines.size() > solve.mines.size()) {
                    one++;
                } else {
                    two++;
                }
                s.printBoard(); std::cout << std::endl;
                s.makeMove(solve);
                num++;
                //std::cout << s.board.minesLeft;
            }
        }
    }
    //std::cout << one << " " << two << "\n";

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