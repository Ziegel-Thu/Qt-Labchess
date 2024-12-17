#include "Board.h"
#include "Piece.h"
Board::Board(int rows, int cols) : rows_(rows), cols_(cols) {
    grid_.resize(rows_, MyVector<std::shared_ptr<Piece>>(cols_, nullptr));
}

void Board::initialize() {
    for (auto& row : grid_) {
        std::fill(row.begin(), row.end(), nullptr);
    }
}

bool Board::setPiece(int row, int col, std::shared_ptr<Piece> piece) {
    if (isValidPosition(row, col) ) {
        grid_[row][col] = piece; // 将棋子放置到指定位置
        return true; // 成功放置棋子
    }
    return false; // 放置失败
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}


bool Board::recordMoveHistory(int row ,int col ){
    moveHistory.push(std::make_pair(row,col));
    return true;
}
bool Board::undoLastMove() {
    if(moveHistory.empty()){
        return false;
    }
    auto lastMove = moveHistory.top();
    int lastMoveRow = lastMove.first;
    int lastMoveCol = lastMove.second;
    grid_[lastMoveRow][lastMoveCol] = nullptr;
    moveHistory.pop();
    return true;

}

std::shared_ptr<Piece> Board::getPiece(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return nullptr;
    }
    return grid_[row][col];
}


