#pragma once

#include <memory>
#include <vector>
#include <tuple> // 添加此行以使用 std::tuple
#include "Board.h"
#include "Player.h"

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

protected:
    std::shared_ptr<Board> board_;
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<Player> currentPlayer_;
    bool gameOver_;
private:
    std::shared_ptr<Piece> selectedPiece_; // 当前选择的棋子
    bool isPieceSelected_; // 指示是否已选择棋子
};
