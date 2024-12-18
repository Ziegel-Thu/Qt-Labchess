#pragma once
#include "../MySTL/my_vector.h"
#include <memory>
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
    void inputName();
    void end();

    bool undoMove();
    void switchPlayer();
    void initializeChessPieces();
    const std::shared_ptr<Board> getBoard() const;
    const std::shared_ptr<Board> getDyingBoard() const;
    const MyVector<std::shared_ptr<Player>> getPlayers() const;
    const std::shared_ptr<Player> getCurrentPlayer() const;
    bool isGameOver() const;
    bool redoMove();
    bool isValidMove(std::shared_ptr<Piece> piece, int row, int col, std::shared_ptr<Piece> targetPiece);
    bool press(int row, int col);
    void updateDeathTime();
    bool isKingAlive(const std::string &color);
    int getStep();
    MyVector<std::shared_ptr<Piece>> getDyingPieceList() const;

    std::shared_ptr<Piece> redKing_;
    std::shared_ptr<Piece> blueKing_;

    std::shared_ptr<Piece> selectedPiece_;
    bool isPieceSelected_; 
    MyVector<std::shared_ptr<Piece>> pieceList_;
    bool selectable_=true;
    void setSelectable(bool selectable);
    int machineNumber1_;
    int machineNumber0_;
    void pass();
    void confirm();
    MyVector<std::shared_ptr<Player>> players_;

signals:
    void updateGameTimeMachineButton();

protected:
    std::shared_ptr<Board> board_;
    std::shared_ptr<Player> currentPlayer_;
    bool gameOver_;

private:
    bool isPathClear(int startRow, int startCol, int endRow, int endCol); // 检查路径是否被阻挡的辅助函数
    int numQueenAdditional_ = 0;
    MyVector<MyVector<std::tuple<std::shared_ptr<Piece>,int,int>>> moveHistory_;
    MyVector<MyVector<std::tuple<std::shared_ptr<Piece>,int,int>>> viewedMoveHistory_;
    MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> timePiecePosition_;
    MyVector<std::shared_ptr<Piece>> dyingPieceList_;
    void initializePlayers();
    void updateMoveHistory();
    void undoBoard();
    void undoHistory();
    void redoBoard();
    void redoHistory();
    void updateTimePiecePosition();
    void handlePieceCrashing(MyVector<std::tuple<std::shared_ptr<Piece>, int, int>> temp);
};
