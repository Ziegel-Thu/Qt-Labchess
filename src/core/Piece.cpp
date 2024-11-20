#include "Piece.h"

Piece::Piece(const std::string& color, const std::string& type, int index, int row, int col, bool isAlive, int lifespan) 
    : color_(color), type_(type), index_(index), row_(row), col_(col), isAlive_(isAlive), lifespan_(lifespan) {}

void Piece::setColor(const std::string& color) {
    color_ = color;
}

void Piece::setIndex(const int index) {
    index_ = index;
}

std::string Piece::getColor() const {
    return color_;
}

std::string Piece::getType() const {
    return type_;
}

void Piece::setAlive(bool isAlive) {
    isAlive_ = isAlive;
    lifespan_ = 0;
}

bool Piece::isAlive() const {
    return isAlive_;
}

void Piece::setLifespan(int lifespan) {
    lifespan_ = lifespan;
}

int Piece::getLifespan() const {
    return lifespan_;
}

int Piece::getRow() const {
    return row_; // 返回行
}

int Piece::getCol() const {
    return col_; // 返回列
}

void Piece::setRow(int row) {
    row_ = row;
}

void Piece::setCol(int col) {
    col_ = col;
}
void Piece::setType(const std::string& type) {
    type_ = type;
}
int Piece::getIndex() const {
    return index_;
}