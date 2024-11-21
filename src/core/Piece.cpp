#include "Piece.h"

Piece::Piece(const std::string& color, const std::string& type, int index, int row, int col, bool isAlive, int deathTime) 
    : color_(color), type_(type), index_(index), row_(row), col_(col), isAlive_(isAlive), deathTime_(deathTime) {}

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

void Piece::setAlive(bool isAlive, int deathTime) {
    isAlive_ = isAlive;
    deathTime_ = deathTime;
}

bool Piece::isAlive() const {
    return isAlive_;
}

void Piece::setDeathTime(int deathTime) {
    deathTime_ = deathTime;
}

int Piece::getDeathTime() const {
    return deathTime_;
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