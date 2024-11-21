#pragma once

#include <memory>
#include <vector>
#include <tuple>
#include <QObject>
#include "Board.h"
#include "Player.h"
#include "Piece.h"

class Game : public QObject
{
    Q_OBJECT

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
    void updateDeathTime();
    bool isKingAlive(const std::string &color);
    int getStep();

    // 添加以下两行以定义国王指针
    std::shared_ptr<Piece> redKing_;
    std::shared_ptr<Piece> blueKing_;

    std::shared_ptr<Piece> selectedPiece_;
    bool isPieceSelected_; // 指示是否已选择棋子
    std::vector<std::shared_ptr<Piece> > pieceList_;
    bool selectable_=true;
    void setSelectable(bool selectable);
    int machineNumberBeta_;
    int machineNumberRui_;
    void pass();
    void confirm();

signals:
    void updateGameTimeMachineButton();

protected:
    std::shared_ptr<Board> board_;
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<Player> currentPlayer_;
    bool gameOver_;

private:
    bool isPathClear(int startRow, int startCol, int endRow, int endCol); // 检查路径是否被阻挡的辅助函数
    int numQueenAdditional_ = 0;
    std::vector<std::vector<std::tuple<std::shared_ptr<Piece>,int,int,int> > > moveHistory_;
    std::vector<std::vector<std::tuple<std::shared_ptr<Piece>,int,int,int> > > viewedMoveHistory_;
    void updateMoveHistory();
    void undoBoard();
    void undoHistory();
    void redoBoard();
    void redoHistory();

    

};
