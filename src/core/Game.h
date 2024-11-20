#pragma once

#include <memory>
#include <vector>
#include <tuple> // 添加此行以使用 std::tuple
#include "Board.h"
#include "Player.h"
#include "Piece.h" // 确保包含 Piece.h

class Game {
public:
    Game();
    ~Game() = default;

    void start();
    void end();

    bool undoMove();
    void switchPlayer();
    void initializeChessPieces();
    std::tuple<bool, int, int> selectPiece(int row, int col); // 修改方法声明

    const std::shared_ptr<Board> getBoard() const;
    const std::vector<std::shared_ptr<Player>> getPlayers() const;
    const std::shared_ptr<Player> getCurrentPlayer() const;
    bool isGameOver() const;
    bool redoMove();
    bool isValidMove(std::shared_ptr<Piece> piece, int row, int col, std::shared_ptr<Piece> targetPiece);
    bool press(int row, int col);
    void updateLifespans();
    bool isKingAlive(const std::string& color);

    // 添加以下两行以定义国王指针
    std::shared_ptr<Piece> redKing_;
    std::shared_ptr<Piece> blueKing_;
    std::shared_ptr<Piece> redRook1_;
    std::shared_ptr<Piece> redRook2_;
    std::shared_ptr<Piece> blueRook1_;
    std::shared_ptr<Piece> blueRook2_;
    std::shared_ptr<Piece> redKnight1_;
    std::shared_ptr<Piece> redKnight2_;
    std::shared_ptr<Piece> blueKnight1_;
    std::shared_ptr<Piece> blueKnight2_;
    std::shared_ptr<Piece> redBishop1_;
    std::shared_ptr<Piece> redBishop2_;
    std::shared_ptr<Piece> blueBishop1_;
    std::shared_ptr<Piece> blueBishop2_;
    std::shared_ptr<Piece> redQueen_;
    std::shared_ptr<Piece> blueQueen_;
    std::shared_ptr<Piece> redPawn1_;
    std::shared_ptr<Piece> redPawn2_;
    std::shared_ptr<Piece> redPawn3_;   
    std::shared_ptr<Piece> redPawn4_;
    std::shared_ptr<Piece> redPawn5_;
    std::shared_ptr<Piece> redPawn6_;
    std::shared_ptr<Piece> redPawn7_;
    std::shared_ptr<Piece> redPawn8_;
    std::shared_ptr<Piece> bluePawn1_;
    std::shared_ptr<Piece> bluePawn2_;
    std::shared_ptr<Piece> bluePawn3_;
    std::shared_ptr<Piece> bluePawn4_;
    std::shared_ptr<Piece> bluePawn5_;
    std::shared_ptr<Piece> bluePawn6_;
    std::shared_ptr<Piece> bluePawn7_;
    std::shared_ptr<Piece> bluePawn8_;
    std::shared_ptr<Piece> selectedPiece_;
    bool isPieceSelected_; // 指示是否已选择棋子



protected:
    std::shared_ptr<Board> board_;
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<Player> currentPlayer_;
    bool gameOver_;
private:
    bool isPathClear(int startRow, int startCol, int endRow, int endCol); // 检查路径是否被阻挡的辅助函数
    int numQueenAdditional_=0;
    
};
