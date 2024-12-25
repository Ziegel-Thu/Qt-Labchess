#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <QMessageBox>
#include <QInputDialog>
Game::Game()
{
    board_ = std::make_shared<Board>(8, 8);
    initializePlayers();
    currentPlayer_ = nullptr;
    gameOver_ = false;
    selectedPiece_ = nullptr;
    isPieceSelected_ = false;
    machineNumber1_ = 1;
    machineNumber0_ = 1;
    selectable_ = false;
}

void Game::reset(){
    gameOver_ = false;
    initializeChessPieces();
    selectedPiece_ = nullptr;
    isPieceSelected_ = false;
    machineNumber1_ = 1;
    machineNumber0_ = 1;
    selectable_ = true;

}

void Game::end(){
    gameOver_ = true;
}

void Game::initializePlayers() {
    players_.clear();
    QString player1Name = "玩家1";
    QString player2Name = "玩家2";
    
    players_.push_back(std::make_shared<Player>(player1Name.toStdString(), "Red"));
    players_.push_back(std::make_shared<Player>(player2Name.toStdString(), "Blue"));
    emit playerLanguageSwitchRequested();
}


void Game::inputName(){

    while(true){
        QInputDialog dialog;
        bool ok;
        QString player1Name = dialog.getText(nullptr, isLanguageChinese_?"玩家名称":"Player Name", isLanguageChinese_?"请输入玩家1（红方）的名称":"Please enter the name (red) of player 1", QLineEdit::Normal, "玩家1", &ok);
        if(ok){
            players_[0]->setName(player1Name.toStdString());
        }
        QString player2Name = dialog.getText(nullptr, isLanguageChinese_?"玩家名称":"Player Name", isLanguageChinese_?"请输入玩家2（蓝方）的名称":"Please enter the name (blue) of player 2", QLineEdit::Normal, "玩家2", &ok);
        if(ok){
            players_[1]->setName(player2Name.toStdString());
        }
        if(player1Name.isEmpty()||player2Name.isEmpty()){
            QMessageBox::information(nullptr, isLanguageChinese_?"提示":"Notice", isLanguageChinese_?"玩家名称不能为空":"Player name cannot be empty");
        }
        else{
            if(player1Name==player2Name){
                QMessageBox::information(nullptr, isLanguageChinese_?"提示":"Notice", isLanguageChinese_?"玩家名称不能相同":"Player name cannot be the same");
            }
            else{
                break;
            }
        }
    }
}

void Game::initializeChessPieces()
{
    gameOver_ = false;
    for (int row = 0; row < board_->getRows(); ++row)
    {
        for (int col = 0; col < board_->getCols(); ++col)
        {
            board_->setPiece(row, col, nullptr);
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
    for (int i = 0; i < board_->getCols(); i++)
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
    for (int i = 0; i < board_->getCols(); i++)
    {
        pieceList_.push_back(std::make_shared<Piece>("Blue", "Pawn", i % 2 == 0, 6, i, true, -1));
    }
    for (const auto& piece : pieceList_)
    {
        board_->setPiece(piece->getRow(), piece->getCol(), piece);
    }
    selectable_ = true;
    updateMoveHistory();
    updateTimePiecePosition();

}

void Game::start()
{
    numQueenAdditional_ = 0;
    moveHistory_.clear();
    viewedMoveHistory_.clear();
    machineNumber1_ = 1;
    machineNumber0_ = 1;
    initializeChessPieces();
    currentPlayer_ = players_[0];

}

void Game::languageSwitch(){
    isLanguageChinese_ = !isLanguageChinese_;   
    players_[0]->setName(isLanguageChinese_?"玩家1":"Player 1");
    players_[1]->setName(isLanguageChinese_?"玩家2":"Player 2");
    emit playerLanguageSwitchRequested();
}

void Game::switchPlayer() {currentPlayer_ = (currentPlayer_ == players_[0]) ? players_[1] : players_[0];}

const std::shared_ptr<Board> Game::getBoard() const {return board_;}

const MyVector<std::shared_ptr<Player>> Game::getPlayers() const {return players_;}

const std::shared_ptr<Player> Game::getCurrentPlayer() const {return currentPlayer_;}

void Game::setSelectable(bool selectable) {selectable_ = selectable;}

bool Game::isGameOver() const {return gameOver_;}

int Game::getStep() {return moveHistory_.size();}

MyVector<std::shared_ptr<Piece>> Game::getDyingPieceList() const {return dyingPieceList_;}

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
    if (moveHistory_.size() <=2)
    {
        QMessageBox::information(nullptr, isLanguageChinese_?"提示":"Notice", isLanguageChinese_?"没有更早步骤":"No earlier steps");

        return false;
    }
    undoHistory();
    undoBoard();
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
        QMessageBox::information(nullptr, isLanguageChinese_?"提示":"Notice", isLanguageChinese_?"已经是最新步骤":"Already the latest step");
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
    for (int row = 0; row < board_->getRows(); ++row)
    {
        for (int col = 0; col < board_->getCols(); ++col)
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
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp;
    for(int i=0;i<board_->getRows();i++){
        for(int j=0;j<board_->getCols();j++){
            if(board_->getPiece(i,j)){
                temp.push_back(std::make_tuple(board_->getPiece(i,j),i,j));
            }
            else{
                temp.push_back(std::make_tuple(nullptr,i,j));
            }
        }
    }
    moveHistory_.push_back(temp);
}

void Game::updateTimePiecePosition()
{
    timePiecePosition_.clear();
    for(int i=0;i<board_->getRows();i++){
        for(int j=0;j<board_->getCols();j++){
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
        MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp = viewedMoveHistory_.back();
        viewedMoveHistory_.pop_back();
        moveHistory_.push_back(temp);
    }
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp = viewedMoveHistory_.back();
    for (const auto& [piece, row, col] : temp)
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
    if (getCurrentPlayer()->getColor() == players_[1]->getColor())
    {
        machineNumber1_--;
    }
    else
    {
        machineNumber0_--;
    }
    QMessageBox::information(nullptr, isLanguageChinese_?"提示":"Notice", isLanguageChinese_?"时光逆流成功，请继续":"Time travel successful, please continue");
}
void Game::undoBoard(){
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp = moveHistory_.back();
    handlePieceCrashing(temp);
}
void Game::handlePieceCrashing(MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp){
    dyingPieceList_.clear();
    for (const auto& [piece, row, col] : temp)
    {
        if(board_->getPiece(row,col)&&board_->getPiece(row,col)->isTimePiece()){
            if(piece){
                if(piece->isTimePiece()){
                    continue;
                }
                dyingPieceList_.push_back(piece);
                piece->setCol(col);
                piece->setRow(row);
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
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp = moveHistory_.back();
    moveHistory_.pop_back();
    viewedMoveHistory_.push_back(temp);
}
void Game::redoHistory(){
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp = viewedMoveHistory_.back();
    viewedMoveHistory_.pop_back();
    moveHistory_.push_back(temp);
}
void Game::redoBoard(){
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp = viewedMoveHistory_.back();
    handlePieceCrashing(temp);
}

