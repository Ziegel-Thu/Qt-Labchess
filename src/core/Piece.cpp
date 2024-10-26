#include "Piece.h"

Piece::Piece(const std::string& color, const std::string& type) : color_(color), type_(type) {}

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