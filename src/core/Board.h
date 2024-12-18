#pragma once
#include "../MySTL/my_vector.h"
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
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
    std::shared_ptr<Piece> getPiece(int row, int col) const;


protected:
    int rows_;
    int cols_;
    MyVector<MyVector<std::shared_ptr<Piece>>> grid_;
};
