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
        if (board.gameOver) return;
        changeMade = false;
        std::vector<Point> borderNumbers = getAllBorderNumbers();
        // Only iterates through tiles of interest
        for (Point point : borderNumbers) {
            if (board.visibleBoard[point.y][point.x] <= 0 || board.countUncleared(point.y, point.x) == 0) 
                continue;
            if (board.visibleBoard[point.y][point.x] == board.countMines(point.y, point.x) + board.countUncleared(point.y, point.x)) {
                board.flagSurrounding(point.y, point.x);
                changeMade = true;
                //std::cout << "Stuck on case 1" << std::endl;
            } else if (board.visibleBoard[point.y][point.x] == board.countMines(point.y, point.x)) {
                board.chord(point.y, point.x);
                changeMade = true;
                //std::cout << "Stuck on case 2 at " << point.y << " " << point.x << std::endl;
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
    if (board.gameOver) return;
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
    //std::cout << "Making guess at Y: " << randY << " X: " << randX << std::endl;
    board.click(randY, randX);
}

Solver::Solution Solver::testAllCases() {
    Solution solution;
    if (board.gameOver) return solution;
    std::unordered_map<Point, int, PointHash> clear;
    std::unordered_map<Point, int, PointHash> mines;

    std::vector<Point> borderingUncleared = getAllBorderingUncleared();
    std::vector<Point> borderNumbers = getAllBorderNumbers();
    std::vector<std::vector<int>> allCombinations; 
    // Reduces possible combinations when nearing the end of the game
    std::cout << "generating combinations\n";
    if (borderingUncleared.size() == Board::Y_DIMENSION * Board::X_DIMENSION - board.numCleared)
        allCombinations = generateCombinations(board.minesLeft, borderingUncleared.size());
    else 
        allCombinations = generateAllCombinations(board.minesLeft, borderingUncleared.size());
    int numValidCombinations = allCombinations.size();
    std::vector<std::vector<int>> testBoard = board.visibleBoard;
    for (std::vector<int> combination : allCombinations)
      {
        bool isCaseValid = true;
        // Putting combination info into the testing board
        std::cout << "putting info\n";
        for (int i=0; i<combination.size(); i++) {
            int currentY = borderingUncleared[i].y;
            int currentX = borderingUncleared[i].x;
            testBoard[currentY][currentX] = combination[i] == 1 ? Board::FLAG_VALUE : Board::CLEARED_VALUE;
        }
        // Testing whether the combination is valid
        std::cout << "valid or not\n";
        for (Point p : borderNumbers) {
            if (!isTileValid(p.y, p.x, testBoard)) {
                isCaseValid = false;
                numValidCombinations--;
                break;
            }
        }
        // if the case is valid adds the solution into clear and mines maps and reverts changes to testing board
        std::cout << "clearing\n";
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
        std::cout << "finished combination\n";
    }
    std::cout << "finished all combinations\n";
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
Solver::Solution Solver::testCasesByTile() {
    Solution solution;
    if (board.gameOver) return solution;

    std::vector<Point> borderNumbers = getAllBorderNumbers();
    Board testBoard = board;
    std::unordered_set<Point, PointHash> testedTiles;

    for (Point currentNumber : borderNumbers) {
        int y = currentNumber.y; 
        int x = currentNumber.x;
        std::vector<Point> surroundingUncleared = getSurroundingUncleared(y, x, board);
        std::unordered_map<Point, int, PointHash> clear, mines;

        // Checks if the surrounding tiles have already been tested
        bool allContained = true;
        for (Point point : surroundingUncleared) {
            if (testedTiles.find(point) == testedTiles.end()) {
                allContained = false;
                break;
            }
        }
        if (allContained) continue;

        int minesLeft = testBoard.visibleBoard[y][x] - board.countMines(y, x);
        std::vector<std::vector<int>> combinations = generateCombinations(minesLeft, surroundingUncleared.size());
        int numValidArrangements = combinations.size();

        // Tests mine combinations for individual number tiles
        for (std::vector<int> arrangement : combinations) {

            // Inputs mine arrangement into testBoard
            for (int i=0; i<arrangement.size(); i++) {
                int currentY = surroundingUncleared[i].y;
                int currentX = surroundingUncleared[i].x;
                testBoard.visibleBoard[currentY][currentX] = arrangement[i] == 1 ? Board::FLAG_VALUE : Board::CLEARED_VALUE;
            }
            // Tests whether the combination is valid, and adds tiles to caseSolution
            if (!testCombination(testBoard, clear, mines)) {
                numValidArrangements--;
            }
            // Resets any changes made to testBoard
            for (Point unclearedPoint : getAllBorderingUncleared()) {
                testBoard.visibleBoard[unclearedPoint.y][unclearedPoint.x] = Board::UNCLEARED_VALUE;
            }
        }

        testedTiles.insert(surroundingUncleared.begin(), surroundingUncleared.end());

        for (const auto & [point, count] : clear) {
            if (count == numValidArrangements) {
                solution.clear.push_back(point);
            }
        }
        for (const auto & [point, count] : mines) {
            if (count == numValidArrangements) {
               solution.mines.push_back(point);
            }
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

void Solver::printBoard() {
    for (int i=0; i<Board::Y_DIMENSION; i++) {
        for (int j=0; j<Board::X_DIMENSION; j++) {
            if (board.visibleBoard[i][j] >= 0 )
                std::cout << " " << board.visibleBoard[i][j] << "\t";
            else 
                std::cout << board.visibleBoard[i][j] << "\t";
        }
        std::cout << std::endl;
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

std::vector<Solver::Point> Solver::getSurroundingUncleared(int y_coord, int x_coord, Board testBoard) {
    std::vector<Point> surroundingUncleared;
        for (int i=y_coord-1; i<=y_coord+1; i++) {
        if (i < 0) i++;
        if (i >= Board::Y_DIMENSION) break;
        for (int j=x_coord-1; j<= x_coord+1; j++) {
            if (j < 0) j++;
            if (j >= Board::X_DIMENSION) break;
            if (!(i == y_coord && j == x_coord) && testBoard.visibleBoard[i][j] == Board::UNCLEARED_VALUE)
                surroundingUncleared.push_back(Point(i, j));
        }
    }
    return surroundingUncleared;
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
    } while (std::prev_permutation(combination.begin(), combination.end()));
    
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
            if (!(i == y_coord && j == x_coord) && (testCase[i][j] == Board::FLAG_VALUE))
                numMines++;
        }
    }
    return board.visibleBoard[y_coord][x_coord] == numMines;
}

bool Solver::testCombination(Board testBoard, std::unordered_map<Point, int, PointHash>& clearMap, 
                                std::unordered_map<Point, int, PointHash>& minesMap) {
    std::vector<Point> clear, mines;
    std::vector<Point> borderNumbers = getAllBorderNumbers();
    bool changeMade;
    do {
        changeMade = false;
        for (Point currentPoint : borderNumbers) {
            int y = currentPoint.y; int x = currentPoint.x;
            int currentNumber = testBoard.visibleBoard[y][x];
            bool isInvalid = currentNumber > testBoard.countUncleared(y, x) + testBoard.countMines(y, x)
                                    || currentNumber < testBoard.countMines(y, x);
            if (isInvalid) return false;

            bool needsFlag = testBoard.countUncleared(y, x) 
                                    && currentNumber == testBoard.countUncleared(y, x) + testBoard.countMines(y, x);
            bool needsClearing = currentNumber == testBoard.countMines(y, x) && testBoard.countUncleared(y, x) > 0;
            if (needsFlag) {
                changeMade = true;
                for (Point unclearedPoint : getSurroundingUncleared(y, x, testBoard)) {
                    testBoard.visibleBoard[unclearedPoint.y][unclearedPoint.x] = Board::FLAG_VALUE;
                    if (std::find(mines.begin(), mines.end(), unclearedPoint) == mines.end()) {
                        mines.push_back(unclearedPoint);
                    }
                }
            } else if (needsClearing) {
                changeMade = true;
                for (Point unclearedPoint : getSurroundingUncleared(y, x, testBoard)) {
                    testBoard.visibleBoard[unclearedPoint.y][unclearedPoint.x] = Board::CLEARED_VALUE;
                    if (std::find(clear.begin(), clear.end(), unclearedPoint) == clear.end()) {
                        clear.push_back(unclearedPoint);
                    }
                }
            }
        }
    } while (changeMade);

    for (Point p: clear) {
        clearMap[p]++;
    }
    for (Point p: mines) {
        minesMap[p]++;
    }

    return true;
}
