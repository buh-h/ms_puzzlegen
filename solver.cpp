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

void Solver::guaranteedClick() {
    srand(time(NULL));
    int randX, randY;
    do {
        randX = rand() % board.X_DIMENSION;
        randY = rand() % board.Y_DIMENSION;
    } while (board.fullBoard[randY][randX] == board.MINE_VALUE 
                || board.visibleBoard[randY][randX] != board.UNCLEARED_VALUE);
    board.click(randY, randX);
}

Solver::Solution Solver::testAllCases() {
    
}

Solver::Solution Solver::testCasesForTile(int y_coord, int x_coord) {

}

bool Solver::isBorderTile(int y_coord, int x_coord) {
    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= board.Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= board.X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && (board.visibleBoard[i][j] > 0))
                return true;
        }
    }
    return false;
}

std::vector<Solver::Point> Solver::getAllBorderingUncleared() {
    std::vector<Solver::Point> borderList;
    for (int i=0; i<board.Y_DIMENSION; i++) {
        for (int j=0; j<board.X_DIMENSION; j++) {
            if (isBorderTile(i, j)) borderList.push_back({i, j});
        }
    }
    return borderList;
}

std::vector<Solver::Point> Solver::getSurroundingUncleared(int y_coord, int x_coord) {
    std::vector<Solver::Point> unclearedList;
    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= board.Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= board.X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && (board.visibleBoard[i][j] == board.UNCLEARED_VALUE))
                unclearedList.push_back({i, j});
        }
    }
    return unclearedList;
}

std::vector<std::vector<bool>> Solver::generateCombinations(int numMines, int numUncleared) {
    std::vector<std::vector<bool>> allCombinations;
    std::vector<bool> combination(numUncleared, false);

    if (numMines >= numUncleared) {
        std::fill(combination.begin(), combination.end(), true);
    } else {
        std::fill(combination.begin(), combination.begin() + numMines, true);
    }

    do {
        allCombinations.push_back(combination);
    } while (std::next_permutation(combination.begin(), combination.end()));
    
    return allCombinations;
}

std::vector<std::vector<bool>> Solver::generateAllCombinations(int totalMines, int numUncleared) {
    std::vector<std::vector<bool>> allCombinations;
    int numMines = totalMines > numUncleared ? numUncleared : totalMines;

    while (numMines-- > 0) {
        std::vector<std::vector<bool>> combinations = generateCombinations(numMines, numUncleared);
        allCombinations.insert(allCombinations.end(), combinations.begin(), combinations.end());
    }

    return allCombinations;
}