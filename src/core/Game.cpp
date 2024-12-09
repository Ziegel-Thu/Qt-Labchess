#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <QMessageBox>
Game::Game()
{
    board_ = std::make_shared<Board>(8, 8);
    dyingBoard_ = std::make_shared<Board>(8, 8);
    players_.push_back(std::make_shared<Player>("Rui", "Red"));   
    players_.push_back(std::make_shared<Player>("Beta", "Blue")); 
    currentPlayer_ = nullptr;
    gameOver_ = false;
    selectedPiece_ = nullptr;
    isPieceSelected_ = false;
    machineNumber1_ = 1;
    machineNumber0_ = 1;
    selectable_ = true;
}

void Game::initializeChessPieces()
{
    gameOver_ = false;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            board_->setPiece(row, col, nullptr);
            dyingBoard_->setPiece(row, col, nullptr);
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
    updateTimePiecePosition();

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

void Game::end() {gameOver_ = true;}

void Game::switchPlayer() {currentPlayer_ = (currentPlayer_ == players_[0]) ? players_[1] : players_[0];}

const std::shared_ptr<Board> Game::getBoard() const {return board_;}

const std::vector<std::shared_ptr<Player>> Game::getPlayers() const {return players_;}

const std::shared_ptr<Player> Game::getCurrentPlayer() const {return currentPlayer_;}

bool Game::isGameOver() const {return gameOver_;}

int Game::getStep() {return moveHistory_.size();}

void Game::setSelectable(bool selectable) {selectable_ = selectable;}

std::vector<std::shared_ptr<Piece>> Game::getDyingPieceList() const {return dyingPieceList_;}

bool Game::isValidMove(std::shared_ptr<Piece> selectedPiece, int row, int col, std::shared_ptr<Piece> targetPiece)
{
    int currentRow = selectedPiece->getRow();
    int currentCol = selectedPiece->getCol();
    std::string color = selectedPiece->getColor();

    if (selectedPiece->getType() == "Pawn")
    {
        int direction = (color == "Red") ? 1 : -1; 

        if (col == currentCol && row == currentRow + direction)
        {
            return targetPiece == nullptr; 
        }
        
        if (col == currentCol && row == currentRow + 2 * direction && currentRow == (color == "Red" ? 1 : 6))
        {
            return targetPiece == nullptr && board_->getPiece(currentRow + direction, currentCol) == nullptr; 
        }
        // 检查是否是对角线吃子
        if (abs(col - currentCol) == 1 && row == currentRow + direction)
        {
            return targetPiece != nullptr && targetPiece->getColor() != color; 
        }
    }

    if (targetPiece != nullptr && targetPiece->getColor() == color)
    {
        return false; 
    }

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
            return targetPiece == nullptr || targetPiece->getColor() != color; 
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
            return targetPiece == nullptr || targetPiece->getColor() != color; 
        }
    }
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


bool Game::press(int row, int col)
{
    if (!selectable_)
    {
        return false;
    }
    auto targetPiece = board_->getPiece(row, col);
    if (isPieceSelected_)
    {
        if (isValidMove(selectedPiece_, row, col, targetPiece))
        {
            board_->setPiece(selectedPiece_->getRow(), selectedPiece_->getCol(), nullptr);
            if (targetPiece)
            {
                if (targetPiece->getColor() != currentPlayer_->getColor())
                {
                    targetPiece->setAlive(false, getStep());
                }
            }
            selectedPiece_->setRow(row);
            selectedPiece_->setCol(col);
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
                board_->setPiece(row, col, std::make_shared<Piece>(selectedPiece_->getColor(), "Queen", selectedPiece_->isTimePiece(), row, col, true, -1)); // 升变为后
                pieceList_.push_back(std::make_shared<Piece>(selectedPiece_->getColor(), "Queen", selectedPiece_->isTimePiece(), row, col, true, -1));
                numQueenAdditional_++;
            }
            updateMoveHistory();
            updateTimePiecePosition();
            selectedPiece_ = nullptr; 
            isPieceSelected_ = false; 

            switchPlayer();
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
        return true;
    }
    else
    {
        if (targetPiece && targetPiece->getColor() == currentPlayer_->getColor())
        {
            selectedPiece_ = targetPiece; 
            isPieceSelected_ = true;      
            return true;                  
        }
    }
    return true;
}

void Game::updateDeathTime()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            auto piece = board_->getPiece(row, col);
            if (piece && piece->getDeathTime() > 0)
            {
                if (piece->getDeathTime() == getStep())
                {
                    piece->setAlive(false, getStep());
                    board_->setPiece(row, col, nullptr);
                }
            }
        }
    }
}
bool Game::isKingAlive(const std::string &color)
{
    if (color == "Red")
    {
        return redKing_->isAlive(); 
    }
    else
    {
        return blueKing_->isAlive();
    }
    return false;
}

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
            return false;
        }
        currentRow += rowDirection;
        currentCol += colDirection;
    }
    return true;
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

void Game::updateTimePiecePosition()
{
    timePiecePosition_.clear();
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board_->getPiece(i,j)&&board_->getPiece(i,j)->isTimePiece()){
                timePiecePosition_.push_back(std::make_tuple(board_->getPiece(i,j),i,j));
            }
        }
    }
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
    for(auto piece:dyingPieceList_){
        piece->setAlive(false, getStep() - 1);
    }
    if (getCurrentPlayer()->getName() == players_[1]->getName())
    {
        machineNumber1_--;
    }
    else
    {
        machineNumber0_--;
    }
    QMessageBox::information(nullptr, "提示", "时光逆流成功，请继续");
}
void Game::undoBoard(){
    std::vector<std::tuple<std::shared_ptr<Piece>, int, int, int>> temp = moveHistory_.back();
    dyingPieceList_.clear();
    for(auto timePiecePosition:timePiecePosition_){
        if(std::get<0>(timePiecePosition)->isTimePiece()){
            board_->setPiece(std::get<1>(timePiecePosition), std::get<2>(timePiecePosition), std::get<0>(timePiecePosition));
        }
    }
    for (const auto& [piece, row, col,deathTime] : temp)
    {
        if(board_->getPiece(row,col)&&board_->getPiece(row,col)->isTimePiece()){
            if(piece){
                if(piece->isTimePiece()){
                    continue;
                }
                dyingPieceList_.push_back(piece);
                piece->setCol(col);
                piece->setRow(row);
                dyingBoard_->setPiece(row, col, piece);
            }
        }
        else{
            if(piece){
                if(piece->isTimePiece()){
                    continue;
                }
                piece->setCol(col);
                piece->setRow(row);
            }
            board_->setPiece(row, col, piece);
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
    for(auto timePiecePosition:timePiecePosition_){
        if(std::get<0>(timePiecePosition)->isTimePiece()){
            board_->setPiece(std::get<1>(timePiecePosition), std::get<2>(timePiecePosition), std::get<0>(timePiecePosition));
        }
    }
    dyingPieceList_.clear();
    for (const auto& [piece, row, col,deathTime] : temp)
    {
        if(board_->getPiece(row,col)&&board_->getPiece(row,col)->isTimePiece()){
            if(piece){
                if(piece->isTimePiece()){
                    continue;
                }
                dyingPieceList_.push_back(piece);
                piece->setCol(col);
                piece->setRow(row);
                dyingBoard_->setPiece(row, col, piece);
            }
        }
        else{
            if(piece){
                if(piece->isTimePiece()){
                    continue;
                }
                piece->setCol(col);
                piece->setRow(row);
            }
            board_->setPiece(row, col, piece);
        }
    }
}