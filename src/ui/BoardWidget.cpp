#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

BoardWidget::BoardWidget(std::shared_ptr<Game> game, QWidget *parent) : QWidget(parent), game_(game) {
    cellSize_ = 100;
    setFixedSize(cellSize_ * game_->getBoard()->getCols(), cellSize_ * game_->getBoard()->getRows());
}

void BoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int squareSize = 100; // 每个方格的大小
    for (int row = 0; row < game_->getBoard()->getRows(); ++row) {
        for (int col = 0; col < game_->getBoard()->getCols(); ++col) {
            // 计算方格的颜色
            if ((row + col) % 2 == 1) {
                painter.fillRect(col * squareSize, row * squareSize, squareSize, squareSize, Qt::black); // 黑色方格
            } else {
                painter.fillRect(col * squareSize, row * squareSize, squareSize, squareSize, Qt::white); // 白色方格
            }
        }
    }

    // 绘制棋子 
    for (int i = 0; i < game_->getBoard()->getRows(); ++i) {
        for (int j = 0; j < game_->getBoard()->getCols(); ++j) {
            auto piece = game_->getBoard()->getPiece(i, j);

            }
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event) {
    QPoint boardPos = pixelToBoard(event->pos());
    emit moveMade(boardPos.y(), boardPos.x());
}

QPoint BoardWidget::boardToPixel(int row, int col) const {
    return QPoint(col * cellSize_ + cellSize_ / 2, row * cellSize_ + cellSize_ / 2);
}

QPoint BoardWidget::pixelToBoard(const QPoint& pixel) const {
    return QPoint(pixel.x() / cellSize_, pixel.y() / cellSize_);
}
