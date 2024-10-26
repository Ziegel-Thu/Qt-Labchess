#include "Game.h"
#include "Board.h"
#include "Piece.h"



Game::Game() {
    board_ = std::make_shared<Board>(8, 8);  // 8x8 的国际象棋棋盘
    players_.push_back(std::make_shared<Player>("Rui", "Red"));   // 红色玩家
    players_.push_back(std::make_shared<Player>("Beta", "Blue")); // 蓝色玩家
    currentPlayer_ = nullptr;
    gameOver_ = true;

 // 初始化棋子
}

void Game::initializeChessPieces() {
    // 摆放红方棋子
    board_->setPiece(0, 0, std::make_shared<Piece>("Red", "Rook"));
    board_->setPiece(0, 1, std::make_shared<Piece>("Red", "Knight"));
    board_->setPiece(0, 2, std::make_shared<Piece>("Red", "Bishop"));
    board_->setPiece(0, 3, std::make_shared<Piece>("Red", "Queen"));
    board_->setPiece(0, 4, std::make_shared<Piece>("Red", "King"));
    board_->setPiece(0, 5, std::make_shared<Piece>("Red", "Bishop"));
    board_->setPiece(0, 6, std::make_shared<Piece>("Red", "Knight"));
    board_->setPiece(0, 7, std::make_shared<Piece>("Red", "Rook"));
    
    for (int col = 0; col < 8; ++col) {
        board_->setPiece(1, col, std::make_shared<Piece>("Red", "Pawn"));
    }

    // 摆放蓝方棋子
    board_->setPiece(7, 0, std::make_shared<Piece>("Blue", "Rook"));
    board_->setPiece(7, 1, std::make_shared<Piece>("Blue", "Knight"));
    board_->setPiece(7, 2, std::make_shared<Piece>("Blue", "Bishop"));
    board_->setPiece(7, 3, std::make_shared<Piece>("Blue", "Queen"));
    board_->setPiece(7, 4, std::make_shared<Piece>("Blue", "King"));
    board_->setPiece(7, 5, std::make_shared<Piece>("Blue", "Bishop"));
    board_->setPiece(7, 6, std::make_shared<Piece>("Blue", "Knight"));
    board_->setPiece(7, 7, std::make_shared<Piece>("Blue", "Rook"));
    
    for (int col = 0; col < 8; ++col) {
        board_->setPiece(6, col, std::make_shared<Piece>("Blue", "Pawn"));
    }
}

void Game::start() {
    initializeChessPieces();
    currentPlayer_ = players_[0];
    gameOver_ = false;
}

void Game::end() {
    gameOver_ = true;
}

bool Game::makeMove(int row, int col) {
    if (gameOver_ || !board_->isValidPosition(row, col) || board_->getPiece(row, col)) {
        return false;
    }

    // 使用当前玩家的颜色和类型创建棋子
    auto piece = std::make_shared<Piece>(currentPlayer_->getColor(), "Pawn"); // 这里假设是兵，您可以根据实际情况调整
    board_->setPiece(row, col, piece);
    board_->recordMoveHistory(row, col);
    if (board_->isGameOver(row, col)) {
        end();
        return true;
    }

    switchPlayer();
    return true;
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
