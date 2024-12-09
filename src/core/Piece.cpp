#include "Piece.h"

Piece::Piece(const std::string& color, const std::string& type, bool isTimePiece, int row, int col, bool isAlive, int deathTime) 
    : color_(color), type_(type), isTimePiece_(isTimePiece), row_(row), col_(col), isAlive_(isAlive), deathTime_(deathTime) {}

void Piece::setColor(const std::string& color) {color_ = color;}

void Piece::setTimePiece(bool isTimePiece) {isTimePiece_ = isTimePiece;}

std::string Piece::getColor() const {return color_;}

std::string Piece::getType() const {return type_;}

void Piece::setRow(int row) {row_ = row;}

void Piece::setCol(int col) {col_ = col;}

int Piece::getRow() const {return row_;}

int Piece::getCol() const {return col_;}

void Piece::setAlive(bool isAlive, int deathTime) {
    isAlive_ = isAlive;
    deathTime_ = deathTime;
}

bool Piece::isAlive() const {return isAlive_;}

void Piece::setDeathTime(int deathTime) {deathTime_ = deathTime;}

int Piece::getDeathTime() const {return deathTime_;}


void Piece::setType(const std::string& type) {type_ = type;}

bool Piece::isTimePiece() const {return isTimePiece_;}
