#include "Piece.h"

Piece::Piece(const std::string& color, const std::string& type, bool isAlive, int lifespan) 
    : color_(color), type_(type), isAlive_(isAlive), lifespan_(lifespan) {}

void Piece::setColor(const std::string& color) {
    color_ = color;
}

void Piece::setType(const std::string& type) {
    type_ = type;
}

std::string Piece::getColor() const {
    return color_;
}

std::string Piece::getType() const {
    return type_;
}

void Piece::setAlive(bool isAlive) {
    isAlive_ = isAlive;
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
