#include <iostream>
#include "board.hpp"
#include <string>
int main() {

    Board b(15);
    b.click(0,0);
    for (int i=0; i<b.fullBoard.size(); i++) {
        for (int j=0; j<b.fullBoard.size(); j++) {
            std::cout << b.fullBoard[i][j] << "\t";
        }
        std::cout << "\n";
    }

        std::cout << "\n";

    for (int i=0; i<b.visibleBoard.size(); i++) {
        for (int j=0; j<b.visibleBoard.size(); j++) {
            std::cout << b.visibleBoard[i][j] << "\t";
        }
        std::cout << "\n";
    }
    return 0;
}