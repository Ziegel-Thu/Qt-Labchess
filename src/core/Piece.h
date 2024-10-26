#pragma once

#include <string>

class Piece {
public:
    Piece(const std::string& color, const std::string& type, bool isAlive, int lifespan );
    ~Piece() = default;

    void setColor(const std::string& color);
    void setType(const std::string& type);
    std::string getColor() const;
    std::string getType() const;
    bool isAlive() const;
    void setAlive(bool isAlive);
    int getLifespan() const;
    void setLifespan(int lifespan);
    
    

protected:
    std::string color_;
    std::string type_;
    bool isAlive_;
    int lifespan_;
};
