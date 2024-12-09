#pragma once

#include <memory>
#include <QWidget>
#include "../core/Game.h"

class BoardWidget : public QWidget {
    Q_OBJECT

public:
    BoardWidget(std::shared_ptr<Game> game, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void drawPiece(QPainter& painter, std::shared_ptr<Piece> piece, int row, int col);
    void drawTopStrip(QPainter& painter);
    void drawBottomStrip(QPainter& painter);
    void drawBoardCell(QPainter& painter, int row, int col);
    void drawTimeIndicator(QPainter& painter, int row, int col);
    void drawDeathTimer(QPainter& painter, std::shared_ptr<Piece> piece, int row, int col);

signals:
    void moveMade(int row, int col);

private:
    std::shared_ptr<Game> game_;
    int cellSize_;
    int stripSize_;

    QPoint boardToPixel(int row, int col) const;
    QPoint pixelToBoard(const QPoint& pixel) const;
};