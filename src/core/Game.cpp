#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include <iostream>

Game::Game() {
    board_ = std::make_shared<Board>(8, 8);  // 8x8 的国际象棋棋盘
    players_.push_back(std::make_shared<Player>("Rui", "Red"));   // 红色玩家
    players_.push_back(std::make_shared<Player>("Beta", "Blue")); // 蓝色玩家
    currentPlayer_ = nullptr;
    gameOver_ = false;
    selectedPiece_ = nullptr; // 初始化选择的棋子
    isPieceSelected_ = false; // 初始化选择状态
}

void Game::initializeChessPieces() {
    // 清空棋盘上的所有棋子
    gameOver_ = false;
    for
    (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board_->setPiece(row, col, nullptr); // 清空每个位置
        }
    }
    redKing_=std::make_shared<Piece>("Red", "King",0, 0, 4, true, 1);
    redRook1_=std::make_shared<Piece>("Red", "Rook", 1,0, 0, true, 1);
    redRook2_=std::make_shared<Piece>("Red", "Rook", 2,0, 7, true, 1);
    redKnight1_=std::make_shared<Piece>("Red", "Knight",3, 0, 1, true, 1);
    redKnight2_=std::make_shared<Piece>("Red", "Knight",4, 0, 6, true, 1);
    redBishop1_=std::make_shared<Piece>("Red", "Bishop",5, 0, 2, true, 1);
    redBishop2_=std::make_shared<Piece>("Red", "Bishop",6, 0, 5, true, 1);
    redQueen_=std::make_shared<Piece>("Red", "Queen",7, 0, 3, true, 1);
    redPawn1_=std::make_shared<Piece>("Red", "Pawn",8, 1, 0, true, 1);
    redPawn2_=std::make_shared<Piece>("Red", "Pawn",9, 1, 1, true, 1);
    redPawn3_=std::make_shared<Piece>("Red", "Pawn",10, 1, 2, true, 1);
    redPawn4_=std::make_shared<Piece>("Red", "Pawn",11, 1, 3, true, 1);
    redPawn5_=std::make_shared<Piece>("Red", "Pawn",12, 1, 4, true, 1);
    redPawn6_=std::make_shared<Piece>("Red", "Pawn",13, 1, 5, true, 1);
    redPawn7_=std::make_shared<Piece>("Red", "Pawn",14, 1, 6, true, 1);
    redPawn8_=std::make_shared<Piece>("Red", "Pawn",15, 1, 7, true, 1);
    blueKing_=std::make_shared<Piece>("Blue", "King",32, 7, 4, true, 1);
    blueRook1_=std::make_shared<Piece>("Blue", "Rook",33, 7, 0, true, 1);
    blueRook2_=std::make_shared<Piece>("Blue", "Rook",34, 7, 7, true, 1);
    blueKnight1_=std::make_shared<Piece>("Blue", "Knight",35, 7, 1, true, 1);
    blueKnight2_=std::make_shared<Piece>("Blue", "Knight",36, 7, 6, true, 1);
    blueBishop1_=std::make_shared<Piece>("Blue", "Bishop",37, 7, 2, true, 1);
    blueBishop2_=std::make_shared<Piece>("Blue", "Bishop",38, 7, 5, true, 1);
    blueQueen_=std::make_shared<Piece>("Blue", "Queen",39, 7, 3, true, 1);
    bluePawn1_=std::make_shared<Piece>("Blue", "Pawn",40, 6, 0, true, 1);
    bluePawn2_=std::make_shared<Piece>("Blue", "Pawn",41, 6, 1, true, 1);
    bluePawn3_=std::make_shared<Piece>("Blue", "Pawn",42, 6, 2, true, 1);
    bluePawn4_=std::make_shared<Piece>("Blue", "Pawn",43, 6, 3, true, 1);
    bluePawn5_=std::make_shared<Piece>("Blue", "Pawn",44, 6, 4, true, 1);
    bluePawn6_=std::make_shared<Piece>("Blue", "Pawn",45, 6, 5, true, 1);
    bluePawn7_=std::make_shared<Piece>("Blue", "Pawn",46, 6, 6, true, 1);
    bluePawn8_=std::make_shared<Piece>("Blue", "Pawn",47, 6, 7, true, 1);
    board_->setPiece(0, 0, redRook1_);
    board_->setPiece(0, 7, redRook2_);
    board_->setPiece(0, 1, redKnight1_);
    board_->setPiece(0, 6, redKnight2_);
    board_->setPiece(0, 2, redBishop1_);
    board_->setPiece(0, 5, redBishop2_);
    board_->setPiece(0, 3, redQueen_);
    board_->setPiece(0, 4, redKing_);
    board_->setPiece(1, 0, redPawn1_);
    board_->setPiece(1, 1, redPawn2_);
    board_->setPiece(1, 2, redPawn3_);
    board_->setPiece(1, 3, redPawn4_);
    board_->setPiece(1, 4, redPawn5_);
    board_->setPiece(1, 5, redPawn6_);
    board_->setPiece(1, 6, redPawn7_);
    board_->setPiece(1, 7, redPawn8_);
    board_->setPiece(6, 0, bluePawn1_);
    board_->setPiece(6, 1, bluePawn2_);
    board_->setPiece(6, 2, bluePawn3_);
    board_->setPiece(6, 3, bluePawn4_);
    board_->setPiece(6, 4, bluePawn5_);
    board_->setPiece(6, 5, bluePawn6_);
    board_->setPiece(6, 6, bluePawn7_);
    board_->setPiece(6, 7, bluePawn8_);
    board_->setPiece(7, 0, blueRook1_);
    board_->setPiece(7, 7, blueRook2_);
    board_->setPiece(7, 1, blueKnight1_);
    board_->setPiece(7, 6, blueKnight2_);
    board_->setPiece(7, 2, blueBishop1_);
    board_->setPiece(7, 5, blueBishop2_);
    board_->setPiece(7, 3, blueQueen_);
    board_->setPiece(7, 4, blueKing_);
}

void Game::start() {
    initializeChessPieces();
    currentPlayer_ = players_[0];
}

void Game::end() {
    gameOver_ = true;
}

// 检查移动是否合法的辅助函数
bool Game::isValidMove(std::shared_ptr<Piece> selectedPiece, int row, int col, std::shared_ptr<Piece> targetPiece) {
    int currentRow = selectedPiece->getRow();
    int currentCol = selectedPiece->getCol();
    std::string color = selectedPiece->getColor();

    // 兵的移动逻辑
    if (selectedPiece->getType() == "Pawn") {
        // 计算移动方向
        int direction = (color == "Red") ? 1 : -1; // 红色向下移动，蓝色向上移动

        // 检查是否是向前移动一格
        if (col == currentCol && row == currentRow + direction) {
            return targetPiece == nullptr; // 目标位置必须为空
        }

        // 检查是否是起始位置向前移动两格
        if (col == currentCol && row == currentRow + 2 * direction && currentRow == (color == "Red" ? 1 : 6)) {
            return targetPiece == nullptr && board_->getPiece(currentRow + direction, currentCol) == nullptr; // 目标位置和中间位置必须为空
        }
        // 检查是否是对角线吃子
        if (abs(col - currentCol) == 1 && row == currentRow + direction) {
            return targetPiece != nullptr && targetPiece->getColor() != color; // 目标位置必须有对方棋子
        }
    }

    // 其他棋子的规则...
    // 检查是否不能移动到自己的棋子上
    if (targetPiece != nullptr && targetPiece->getColor() == color) {
        return false; // 不能移动到自己的棋子上
    }

    // 车的移动逻辑
    if (selectedPiece->getType() == "Rook") {
        if (currentRow == row || currentCol == col) {
            return isPathClear(currentRow, currentCol, row, col);
        }
    } else if (selectedPiece->getType() == "Knight") {
        if ((abs(currentRow - row) == 2 && abs(currentCol - col) == 1) || 
            (abs(currentRow - row) == 1 && abs(currentCol - col) == 2)) {
            return targetPiece == nullptr || targetPiece->getColor() != color; // 目标位置可以为空或有对方棋子
        }
    } else if (selectedPiece->getType() == "Bishop") {
        if (abs(currentRow - row) == abs(currentCol - col)) {
            return isPathClear(currentRow, currentCol, row, col);
        }
    } else if (selectedPiece->getType() == "Queen") {
        if (currentRow == row || currentCol == col || 
            abs(currentRow - row) == abs(currentCol - col)) {
            return isPathClear(currentRow, currentCol, row, col);
        }
    } else if (selectedPiece->getType() == "King") {
        if (abs(currentRow - row) <= 1 && abs(currentCol - col) <= 1) {
            return targetPiece == nullptr || targetPiece->getColor() != color; // 目标位置可以为空或有对方棋子
        }
    }

    // 默认返回无效
    return false; 
}

bool Game::undoMove() {
    // 实现悔棋功能并切换玩家
    if (gameOver_) {
        return false;
    }
    if (board_->undoLastMove()) {
        switchPlayer();
        return true;
    }
    return false;
}

void Game::switchPlayer() {
    currentPlayer_ = (currentPlayer_ == players_[0]) ? players_[1] : players_[0];
}

const std::shared_ptr<Board> Game::getBoard() const {
    return board_;
}

const std::vector<std::shared_ptr<Player>> Game::getPlayers() const {
    return players_;
}

const std::shared_ptr<Player> Game::getCurrentPlayer() const {
    return currentPlayer_;
}

bool Game::isGameOver() const {
    return gameOver_;
}

bool Game::redoMove() {
    // 实现重做功能
}

std::tuple<bool, int, int> Game::selectPiece(int row, int col) {
    auto piece = board_->getPiece(row, col);
    
    if (piece && piece->getColor() == currentPlayer_->getColor()) {
        selectedPiece_ = piece; // 选择该棋子
        return std::make_tuple(true, row, col); // 返回选择成功和坐标
    }

    return std::make_tuple(false, -1, -1); // 返回失败和无效坐标
}

bool Game::press(int row, int col) {
    // 检查游戏是否已经结束

    // 获取当前点击位置的棋子
    auto targetPiece = board_->getPiece(row, col);
    
    // 如果当前已选择棋子
    if (isPieceSelected_) {
        // 检查移动是否合法
        if (isValidMove(selectedPiece_, row, col, targetPiece)) {
            
            // 擦掉原棋子
            board_->setPiece(selectedPiece_->getRow(), selectedPiece_->getCol(), nullptr);

            // 如果点击的是对方的棋子，处理对方棋子
            if (targetPiece ) {
                if(targetPiece->getColor() != currentPlayer_->getColor()){
                targetPiece->setAlive(false);
                } 
            }

            // 移动棋子到新位置
            selectedPiece_->setRow(row); // 更新行
            selectedPiece_->setCol(col); // 更新列
            board_->setPiece(row, col, selectedPiece_);
            updateLifespans();
            if (!isKingAlive("Red") || !isKingAlive("Blue")) {
                gameOver_ = true;
                return false;
            }

            if ((selectedPiece_->isAlive() || selectedPiece_->getLifespan() > 0) && 
                (selectedPiece_->getType() == "Pawn" && 
                ((selectedPiece_->getColor() == "Red" && row == 7) || 
                (selectedPiece_->getColor() == "Blue" && row == 0)))) {
                board_->setPiece(row, col, std::make_shared<Piece>(selectedPiece_->getColor(), "Queen",16+numQueenAdditional_+32 *(int) (selectedPiece_->getColor()=="Red"), row, col, true, 1)); // 升变为后
                numQueenAdditional_++;


            }

            selectedPiece_ = nullptr; // 移动后重置选择
            isPieceSelected_ = false; // 更新选择状态

            switchPlayer(); // 切换玩家
            return true; // 移动成功
        }
        else if(targetPiece && targetPiece->getColor() == currentPlayer_->getColor()){
            selectedPiece_ = targetPiece;

            targetPiece = nullptr;
            isPieceSelected_ = true;
            return true;
        }
        return true; // 如果移动不合法，仍然返回 true
    } else {
        // 如果当前没有选择棋子，尝试选择棋子
        if (targetPiece && targetPiece->getColor() == currentPlayer_->getColor()) {
            selectedPiece_ = targetPiece; // 选择该棋子
           isPieceSelected_ = true; // 更新选择状态
            return true; // 选择成功
        }
    }
    return true; // 如果没有选择成功，仍然返回 true
}

void Game::updateLifespans() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = board_->getPiece(row, col);
            if (piece && !piece->isAlive()) { // 检查棋子是否已死亡
                if(piece->getLifespan() > 1){
                    piece->setLifespan(piece->getLifespan() - 1); // 假设 Piece 类有 getLifespan 和 setLifespan 方法
                }
                else if(piece->getLifespan() == 1){
                    piece->setAlive(false);
                    board_->setPiece(row, col, nullptr);
                }
            }
        }
    }
}

// 检查国王是否存活的辅助函数
bool Game::isKingAlive(const std::string& color) {
    if (color == "Red") {
        return redKing_->isAlive(); // 直接返回红方国王的存活状态
    }
    else {
        return blueKing_->isAlive(); // 直接返回蓝方国王的存活状态
    }
    return false; // 如果颜色不匹配，返回死亡
}

// 检查路径是否被阻挡的辅助函数
bool Game::isPathClear(int startRow, int startCol, int endRow, int endCol) {
    int rowDirection = (endRow > startRow) ? 1 : (endRow < startRow) ? -1 : 0;
    int colDirection = (endCol > startCol) ? 1 : (endCol < startCol) ? -1 : 0;

    int currentRow = startRow + rowDirection;
    int currentCol = startCol + colDirection;

    while (currentRow != endRow || currentCol != endCol) {
        if (board_->getPiece(currentRow, currentCol) != nullptr) {
            return false; // 路径被阻挡
        }
        currentRow += rowDirection;
        currentCol += colDirection;
    }
    return true; // 路径清晰
}

