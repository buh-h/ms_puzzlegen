#include "solver.hpp"

Solver::Point::Point() : y(0), x(0) {};

Solver::Point::Point(int y, int x) : y(y), x(x) {};

bool Solver::Point::operator==(const Point &other) const {
    // Hash for 8x8 board
    return x == other.x && y == other.y;
}

std::size_t Solver::PointHash::operator()(const Solver::Point &p) const {
    return p.x * 8 + p.y;
}
Solver::Solver(Board b) : board(std::move(b)) {};

void Solver::clearTrivial() {
    bool changeMade = true;
    while (changeMade) {
        changeMade = false;
        std::vector<Point> borderNumbers = getAllBorderNumbers();
        // Only iterates through tiles of interest
        for (Point point : borderNumbers) {
            if (board.visibleBoard[point.y][point.x] <= 0 || board.countUncleared(point.y, point.x) == 0) 
                continue;
            if (board.visibleBoard[point.y][point.x] == board.countMines(point.y, point.x) + board.countUncleared(point.y, point.x)) {
                board.flagSurrounding(point.y, point.x);
                changeMade = true;
            } else if (board.visibleBoard[point.y][point.x] == board.countMines(point.y, point.y)) {
                board.chord(point.y, point.x);
                changeMade = true;
            }
        }
        // Iterates through all tiles
        // for (int i=0; i<board.Y_DIMENSION; i++) {
        //     for (int j=0; j<board.X_DIMENSION; j++) {
        //         if (board.visibleBoard[i][j] <= 0 || board.countUncleared(i, j) == 0) 
        //             continue;
        //         if (board.visibleBoard[i][j] == board.countMines(i, j) + board.countUncleared(i, j)) {
        //             board.flagSurrounding(i, j);
        //             changeMade = true;
        //         }
        //         if (board.visibleBoard[i][j] == board.countMines(i, j)) {
        //             board.chord(i, j);
        //             changeMade = true;
        //         }
        //     }    
        // }
    }
}

void Solver::guaranteedClick() {
    srand(time(NULL));
    std::vector<Point> borderingUncleared = getAllBorderingUncleared();
    int randX, randY;
    if (borderingUncleared.size() == 0) {
        // Clicks randomly
        do {
            randX = rand() % Board::X_DIMENSION;
            randY = rand() % Board::Y_DIMENSION;
        } while (board.fullBoard[randY][randX] == Board::MINE_VALUE 
                    || board.visibleBoard[randY][randX] != Board::UNCLEARED_VALUE);

    } else {
        // Clicks a tile that borders a number tile
        do {
            int randIndex = rand() % borderingUncleared.size();
            randY = borderingUncleared[randIndex].y;
            randX = borderingUncleared[randIndex].x;
        } while (board.fullBoard[randY][randX] == Board::MINE_VALUE 
                    || board.visibleBoard[randY][randX] != Board::UNCLEARED_VALUE);
    }
    board.click(randY, randX);
}

Solver::Solution Solver::testAllCases() {
    std::unordered_map<Point, int, PointHash> clear;
    std::unordered_map<Point, int, PointHash> mines;

    std::vector<Point> borderingUncleared = getAllBorderingUncleared();
    std::vector<Point> borderNumbers = getAllBorderNumbers();
    std::vector<std::vector<int>> allCombinations = generateAllCombinations(board.minesLeft, borderingUncleared.size());
    int numValidCombinations = allCombinations.size();
    std::vector<std::vector<int>> testBoard = board.visibleBoard;

    for (std::vector<int> combination : allCombinations) {
        bool isCaseValid = true;
        // Putting combination info into the testing board
        for (int i=0; i<combination.size(); i++) {
            int currentY = borderingUncleared[i].y;
            int currentX = borderingUncleared[i].x;
            testBoard[currentY][currentX] = combination[i] == 1 ? Board::MINE_VALUE : Board::CLEARED_VALUE;
        }
        // Testing whether the combination is valid
        for (Point p : borderNumbers) {
            if (!isTileValid(p.y, p.x, testBoard)) {
                isCaseValid = false;
                numValidCombinations--;
                break;
            }
        }
        // if the case is valid adds the solution into clear and mines maps and reverts changes to testing board
        for (int i=0; i<combination.size(); i++) {
            int currentY = borderingUncleared[i].y;
            int currentX = borderingUncleared[i].x;
            testBoard[currentY][currentX] = Board::UNCLEARED_VALUE;
            Point currentPoint = {currentY, currentX};
            if (isCaseValid && combination[i] == 0) {
                clear[currentPoint]++;
            } else if (isCaseValid && combination[i] == 1) {
                mines[currentPoint]++;
            }
        }
    }
    
    Solution solution;
    for (const auto & [point, count] : clear) {
        if (count == numValidCombinations) {
            solution.clear.push_back(point);
        }
    }
    for (const auto & [point, count] : mines) {
        if (count == numValidCombinations) {
            solution.mines.push_back(point);
        }
    }
    return solution;
}

void Solver::makeMove(Solution moves) {
    for (Point point : moves.clear) {
        board.click(point.y, point.x);
    }
    for (Point point : moves.mines) {
        board.flag(point.y, point.x);
    }
}

bool Solver::isBorderUncleared(int y_coord, int x_coord) {
    if (board.visibleBoard[y_coord][x_coord] != Board::UNCLEARED_VALUE) 
        return false;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Board::Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= Board::X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && (board.visibleBoard[i][j] > 0))
                return true;
        }
    }
    return false;
}

bool Solver::isBorderNumber(int y_coord, int x_coord) {
    if (board.visibleBoard[y_coord][x_coord] <= 0) 
        return false;

    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Board::Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= Board::X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && board.visibleBoard[i][j] == Board::UNCLEARED_VALUE)
                return true;
        }
    }
    return false;  
}

std::vector<Solver::Point> Solver::getAllBorderingUncleared() {
    std::vector<Point> borderList;
    for (int i=0; i<Board::Y_DIMENSION; i++) {
        for (int j=0; j<Board::X_DIMENSION; j++) {
            if (isBorderUncleared(i, j)) borderList.push_back(Point(i, j));
        }
    }
    return borderList;
}

std::vector<Solver::Point> Solver::getAllBorderNumbers() {
    std::vector<Point> borderNumbers;
        for (int i=0; i<Board::Y_DIMENSION; i++) {
            for (int j=0; j<Board::X_DIMENSION; j++) {
                if (isBorderNumber(i, j)) borderNumbers.push_back(Point(i, j));
            }
        }
    return borderNumbers;
}

std::vector<std::vector<int>> Solver::generateCombinations(int numMines, int numUncleared) {
    std::vector<std::vector<int>> allCombinations;
    std::vector<int> combination(numUncleared, 0);

    if (numMines >= numUncleared) {
        std::fill(combination.begin(), combination.end(), 1);
    } else {
        std::fill(combination.begin(), combination.begin() + numMines, 1);
    }

    do {
        allCombinations.push_back(combination);
    } while (std::next_permutation(combination.begin(), combination.end()));
    
    return allCombinations;
}

std::vector<std::vector<int>> Solver::generateAllCombinations(int totalMines, int numUncleared) {
    std::vector<std::vector<int>> allCombinations;
    int numMines = totalMines > numUncleared ? numUncleared : totalMines;

    while (numMines-- > 0) {
        std::vector<std::vector<int>> combinations = generateCombinations(numMines, numUncleared);
        allCombinations.insert(allCombinations.end(), combinations.begin(), combinations.end());
    }

    return allCombinations;
}

bool Solver::isTileValid(int y_coord, int x_coord, std::vector<std::vector<int>> testCase) {
    int numMines = 0;
    for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Board::Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= Board::X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && (testCase[i][j] == Board::MINE_VALUE))
                numMines++;
        }
    }
    return board.visibleBoard[y_coord][x_coord] == numMines;
}