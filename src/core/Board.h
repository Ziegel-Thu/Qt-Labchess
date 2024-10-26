#pragma once

#include <memory>
#include <stack>
#include <vector>
#include "Piece.h"

class Board {
public:
    Board(int rows, int cols);
    ~Board() = default;

    void initialize();
    bool setPiece(int row, int col, std::shared_ptr<Piece> piece);
    bool isValidPosition(int row, int col) const;
    bool isGameOver(int row, int col) const;
    bool undoLastMove();
    bool recordMoveHistory(int row,int col);

    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
    std::shared_ptr<Piece> getPiece(int row, int col) const;


protected:
    int rows_;
    int cols_;
    std::vector<std::vector<std::shared_ptr<Piece>>> grid_;
    std::stack<std::pair<int,int>>moveHistory;
};
