#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <QMessageBox>
Game::Game()
{
    board_ = std::make_shared<Board>(8, 8);                       // 8x8 的国际象棋棋盘
    players_.push_back(std::make_shared<Player>("Rui", "Red"));   // 红色玩家
    players_.push_back(std::make_shared<Player>("Beta", "Blue")); // 蓝色玩家
    currentPlayer_ = nullptr;
    gameOver_ = false;
    selectedPiece_ = nullptr; // 初始化选择的棋子
    isPieceSelected_ = false; // 初始化选择状态
    machineNumber1_ = 1;
    machineNumber0_ = 1;
    selectable_ = true;
}

void Game::initializeChessPieces()
{
    // 清空棋盘上的所有棋子
    gameOver_ = false;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            board_->setPiece(row, col, nullptr); // 清空每个位置
        }
    }
    pieceList_.clear();
    redKing_ = std::make_shared<Piece>("Red", "King", false, 0, 4, true, -1);
    pieceList_.push_back(redKing_);
    pieceList_.push_back(std::make_shared<Piece>("Red", "Rook", false, 0, 0, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Red", "Rook", true, 0, 7, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Red", "Knight", true, 0, 1, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Red", "Knight", false, 0, 6, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Red", "Bishop", false, 0, 2, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Red", "Bishop", true, 0, 5, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Red", "Queen", true, 0, 3, true, -1));
    for (int i = 0; i < 8; i++)
    {
        pieceList_.push_back(std::make_shared<Piece>("Red", "Pawn", i % 2 == 0, 1, i, true, -1));
    }
    blueKing_ = std::make_shared<Piece>("Blue", "King", false, 7, 4, true, -1);
    pieceList_.push_back(blueKing_);
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Rook", false, 7, 0, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Rook", true, 7, 7, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Knight", true, 7, 1, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Knight", false, 7, 6, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Bishop", false, 7, 2, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Bishop", true, 7, 5, true, -1));
    pieceList_.push_back(std::make_shared<Piece>("Blue", "Queen", true, 7, 3, true, -1));
    for (int i = 0; i < 8; i++)
    {
        pieceList_.push_back(std::make_shared<Piece>("Blue", "Pawn", i % 2 == 0, 6, i, true, -1));
    }
    for (const auto& piece : pieceList_)
    {
        board_->setPiece(piece->getRow(), piece->getCol(), piece);
    }
    updateMoveHistory();

}

void Game::start()
{
    pieceList_.clear();
    numQueenAdditional_ = 0;
    moveHistory_.clear();
    viewedMoveHistory_.clear();
    machineNumber1_ = 1;
    machineNumber0_ = 1;
    initializeChessPieces();
    currentPlayer_ = players_[0];

}

void Game::end()
{
    gameOver_ = true;
}

// 检查移动是否合法的辅助函数
bool Game::isValidMove(std::shared_ptr<Piece> selectedPiece, int row, int col, std::shared_ptr<Piece> targetPiece)
{
    int currentRow = selectedPiece->getRow();
    int currentCol = selectedPiece->getCol();
    std::string color = selectedPiece->getColor();

    // 兵的移动逻辑
    if (selectedPiece->getType() == "Pawn")
    {
        // 计算移动方向
        int direction = (color == "Red") ? 1 : -1; // 红色向下移动，蓝色向上移动

        // 检查是否是向前移动一格
        if (col == currentCol && row == currentRow + direction)
        {
            return targetPiece == nullptr; // 目标位置必须为空
        }

        // 检查是否是起始位置向前移动两格
        if (col == currentCol && row == currentRow + 2 * direction && currentRow == (color == "Red" ? 1 : 6))
        {
            return targetPiece == nullptr && board_->getPiece(currentRow + direction, currentCol) == nullptr; // 目标位置和中间位置必须为空
        }
        // 检查是否是对角线吃子
        if (abs(col - currentCol) == 1 && row == currentRow + direction)
        {
            return targetPiece != nullptr && targetPiece->getColor() != color; // 目标位置必须有对方棋子
        }
    }

    // 其他棋子的规则...
    // 检查是否不能移动到自己的棋子上
    if (targetPiece != nullptr && targetPiece->getColor() == color)
    {
        return false; // 不能移动到自己的棋子上
    }

    // 车的移动逻辑
    if (selectedPiece->getType() == "Rook")
    {
        if (currentRow == row || currentCol == col)
        {
            return isPathClear(currentRow, currentCol, row, col);
        }
    }
    else if (selectedPiece->getType() == "Knight")
    {
        if ((abs(currentRow - row) == 2 && abs(currentCol - col) == 1) ||
            (abs(currentRow - row) == 1 && abs(currentCol - col) == 2))
        {
            return targetPiece == nullptr || targetPiece->getColor() != color; // 目标位置可以为空或有对方棋子
        }
    }
    else if (selectedPiece->getType() == "Bishop")
    {
        if (abs(currentRow - row) == abs(currentCol - col))
        {
            return isPathClear(currentRow, currentCol, row, col);
        }
    }
    else if (selectedPiece->getType() == "Queen")
    {
        if (currentRow == row || currentCol == col ||
            abs(currentRow - row) == abs(currentCol - col))
        {
            return isPathClear(currentRow, currentCol, row, col);
        }
    }
    else if (selectedPiece->getType() == "King")
    {
        if (abs(currentRow - row) <= 1 && abs(currentCol - col) <= 1)
        {
            return targetPiece == nullptr || targetPiece->getColor() != color; // 目标位置可以为空或有对方棋子
        }
    }

    // 默认返回无效
    return false;
}

bool Game::undoMove()
{

    // 实现悔棋功能并切换玩家
    if (gameOver_)
    {
        return false;
    }
    if (moveHistory_.size() ==1)
    {
        if(getCurrentPlayer()->getName()==players_[1]->getName()){
            redoBoard();
            redoHistory();
        }
        QMessageBox::information(nullptr, "提示", "没有更早步骤");

        return false;
    }
    undoHistory();
    undoBoard();
    return true;
}

void Game::switchPlayer()
{
    currentPlayer_ = (currentPlayer_ == players_[0]) ? players_[1] : players_[0];
}

const std::shared_ptr<Board> Game::getBoard() const
{
    return board_;
}

const std::vector<std::shared_ptr<Player>> Game::getPlayers() const
{
    return players_;
}

const std::shared_ptr<Player> Game::getCurrentPlayer() const
{
    return currentPlayer_;
}

bool Game::isGameOver() const
{
    return gameOver_;
}

bool Game::redoMove()
{
    if (gameOver_)
    {
        return false;
    }
    if (viewedMoveHistory_.size() <= 1)
    {
        QMessageBox::information(nullptr, "提示", "已经是最新步骤");
        return false;
    }
    redoBoard();
    redoHistory();
    redoBoard();
    redoHistory();
    return true;
}

std::tuple<bool, int, int> Game::selectPiece(int row, int col)
{
    auto piece = board_->getPiece(row, col);

    if (piece && piece->getColor() == currentPlayer_->getColor())
    {
        selectedPiece_ = piece;                 // 选择该棋子
        return std::make_tuple(true, row, col); // 返回选择成功和坐标
    }

    return std::make_tuple(false, -1, -1); // 返回失败和无效坐标
}

bool Game::press(int row, int col)
{
    // 检查游戏是否已经结束

    // 获取当前点击位置的棋子
    if (!selectable_)
    {
        return false;
    }
    auto targetPiece = board_->getPiece(row, col);

    // 如果当前已选择棋子
    if (isPieceSelected_)
    {
        // 检查移动是否合法
        if (isValidMove(selectedPiece_, row, col, targetPiece))
        {

            // 擦掉原棋子
            board_->setPiece(selectedPiece_->getRow(), selectedPiece_->getCol(), nullptr);

            // 如果点击的是对方的棋子，处理对方棋子
            if (targetPiece)
            {
                if (targetPiece->getColor() != currentPlayer_->getColor())
                {
                    targetPiece->setAlive(false, getStep());
                }
            }

            // 移动棋子到新位置
            selectedPiece_->setRow(row); // 更新行
            selectedPiece_->setCol(col); // 更新列
            board_->setPiece(row, col, selectedPiece_);
            updateDeathTime();
            if (!isKingAlive("Red") || !isKingAlive("Blue"))
            {
                gameOver_ = true;
                return false;
            }

            if ((selectedPiece_->isAlive()) &&
                (selectedPiece_->getType() == "Pawn" &&
                ((selectedPiece_->getColor() == "Red" && row == 7) ||
                (selectedPiece_->getColor() == "Blue" && row == 0))))
            {
                board_->setPiece(row, col, std::make_shared<Piece>(selectedPiece_->getColor(), "Queen", 32 + numQueenAdditional_, row, col, true, -1)); // 升变为后
                pieceList_.push_back(std::make_shared<Piece>(selectedPiece_->getColor(), "Queen", 32 + numQueenAdditional_, row, col, true, -1));
                numQueenAdditional_++;
            }
            updateMoveHistory();

            selectedPiece_ = nullptr; // 移动后重置选择
            isPieceSelected_ = false; // 更新选择状态

            switchPlayer(); // 切换玩家
            emit updateGameTimeMachineButton();
            return true;
        }
        else if (targetPiece && targetPiece->getColor() == currentPlayer_->getColor())
        {
            selectedPiece_ = targetPiece;

            targetPiece = nullptr;
            isPieceSelected_ = true;
            return true;
        }
        return true; // 如果移动不合法，仍然返回 true
    }
    else
    {
        // 如果当前没有选择棋子，尝试选择棋子
        if (targetPiece && targetPiece->getColor() == currentPlayer_->getColor())
        {
            selectedPiece_ = targetPiece; // 选择该棋子
            isPieceSelected_ = true;      // 更新选择状态
            return true;                  // 选择成功
        }
    }
    return true; // 如果没有选择成功，仍然返回 true
}

void Game::updateDeathTime()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            auto piece = board_->getPiece(row, col);
            if (piece && piece->getDeathTime() > 0)
            { // 检查棋子是否已死亡
                if (piece->getDeathTime() == getStep())
                {
                    piece->setAlive(false, getStep());
                    board_->setPiece(row, col, nullptr);
                }
            }
        }
    }
}

// 检查国王是否存活的辅助函数
bool Game::isKingAlive(const std::string &color)
{
    if (color == "Red")
    {
        return redKing_->isAlive(); // 直接返回红方国王的存活状态
    }
    else
    {
        return blueKing_->isAlive(); // 直接返回蓝方国王的存活状态
    }
    return false; // 如果颜色不匹配，返回死亡
}

// 检查路径是否被阻挡的辅助函数
bool Game::isPathClear(int startRow, int startCol, int endRow, int endCol)
{
    int rowDirection = (endRow > startRow) ? 1 : (endRow < startRow) ? -1 : 0;
    int colDirection = (endCol > startCol) ? 1 : (endCol < startCol) ? -1 : 0;

    int currentRow = startRow + rowDirection;
    int currentCol = startCol + colDirection;

    while (currentRow != endRow || currentCol != endCol)
    {
        if (board_->getPiece(currentRow, currentCol) != nullptr)
        {
            return false; // 路径被阻挡
        }
        currentRow += rowDirection;
        currentCol += colDirection;
    }
    return true; // 路径清晰
}

void Game::updateMoveHistory()
{
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board_->getPiece(i,j)){
                temp.push_back(std::make_tuple(board_->getPiece(i,j),i,j,board_->getPiece(i,j)->getDeathTime()));
            }
            else{
                temp.push_back(std::make_tuple(nullptr,i,j,-1));
            }
        }
    }
    moveHistory_.push_back(temp);
}

int Game::getStep()
{
    return moveHistory_.size();
}

void Game::setSelectable(bool selectable)
{
    selectable_ = selectable;
}

void Game::pass()
{
    selectable_ = true;
    if (viewedMoveHistory_.size() == 0)
    {
        return;
    }
    while (viewedMoveHistory_.size() != 1)
    {
        std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = viewedMoveHistory_.back();
        viewedMoveHistory_.pop_back();
        moveHistory_.push_back(temp);
    }
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = viewedMoveHistory_.back();
    for (const auto& [piece, row, col, deathTime] : temp)
    {
        if (piece) {
            piece->setCol(col);
            piece->setRow(row);
            board_->setPiece(row, col, piece);
        }
        else {
            board_->setPiece(row, col, nullptr);
        }
    }
    viewedMoveHistory_.pop_back();
    moveHistory_.push_back(temp);
}
void Game::confirm()
{
    selectable_ = true;
    viewedMoveHistory_.clear();
    if (getCurrentPlayer()->getName() == players_[1]->getName())
    {
        machineNumber1_--;
    }
    else
    {
        machineNumber0_--;
    }
}
void Game::undoBoard(){
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = moveHistory_.back();
    for (const auto& [piece, row, col,deathTime] : temp)
    {
        if (piece) {
            piece->setCol(col);
            piece->setRow(row);
            board_->setPiece(row, col, piece);
        }
        else {
            board_->setPiece(row, col, nullptr);
        }
    }
}
void Game::undoHistory(){
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = moveHistory_.back();
    moveHistory_.pop_back();
    viewedMoveHistory_.push_back(temp);
}
void Game::redoHistory(){
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = viewedMoveHistory_.back();
    viewedMoveHistory_.pop_back();
    moveHistory_.push_back(temp);
}
void Game::redoBoard(){
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = viewedMoveHistory_.back();
    for (const auto& [piece, row, col,deathTime] : temp)
    {
        if (piece) {
            piece->setCol(col);
            piece->setRow(row);
            board_->setPiece(row, col, piece);
        }
        else {
            board_->setPiece(row, col, nullptr);
        }
    }
}