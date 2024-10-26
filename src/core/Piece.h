#pragma once

#include <string>

class Piece {
public:
    Piece(const std::string& color, const std::string& type);
    ~Piece() = default;

    void setColor(const std::string& color);
    void setType(const std::string& type);
    std::string getColor() const;
    std::string getType() const;

protected:
    std::string color_;
    std::string type_;
};