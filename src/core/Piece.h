#pragma once

#include <string>

class Piece {
public:
    Piece(const std::string& color, const std::string& type, int index, int row, int col, bool isAlive = true, int lifespan = 0);
    ~Piece() = default;

    void setColor(const std::string& color);
    void setType(const std::string& type);
    void setIndex(const int index);
    std::string getColor() const;
    std::string getType() const;
    int getIndex() const;
    bool isAlive() const;
    void setAlive(bool isAlive);
    int getLifespan() const;
    void setLifespan(int lifespan);
    void setRow(int row);
    void setCol(int col);
    int getRow() const;
    int getCol() const;

protected:
    std::string color_;
    std::string type_;
    int index_;
    int row_; // 行
    int col_; // 列
    bool isAlive_;
    int lifespan_;
};
