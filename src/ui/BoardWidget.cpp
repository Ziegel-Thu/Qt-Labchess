#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QDir> // 添加这一行以包含 QDir
#include <iostream>
BoardWidget::BoardWidget(std::shared_ptr<Game> game, QWidget *parent) : QWidget(parent), game_(game) {
    cellSize_ = 100;
    setFixedSize(cellSize_ * game_->getBoard()->getCols(), cellSize_ * game_->getBoard()->getRows());
}

void BoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    for (int row = 0; row < game_->getBoard()->getRows(); ++row) {
        for (int col = 0; col < game_->getBoard()->getCols(); ++col) {
            // 计算方格的颜色
            if ((row + col) % 2 == 1) {
                painter.fillRect(col * cellSize_, row * cellSize_, cellSize_, cellSize_, Qt::gray); // 黑色方格
            } else {
                painter.fillRect(col * cellSize_, row * cellSize_, cellSize_, cellSize_, Qt::white); // 白色方格
            }
        }
    }

    // 打印当前工作目录


    // 加载车的图片
    QPixmap rookPixmap("../pics/image.png"); // 使用相对路径
    if (rookPixmap.isNull()) {
        qDebug() << "Failed to load rook image!";
        return; // 如果加载失败，返回
    }

    // 缩放图片
    int scaledWidth = cellSize_;  // 设置缩放后的宽度
    int scaledHeight = cellSize_; // 设置缩放后的高度
    QPixmap scaledRookPixmap = rookPixmap.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 绘制棋子 
    for (int i = 0; i < game_->getBoard()->getRows(); ++i) {
        for (int j = 0; j < game_->getBoard()->getCols(); ++j) {
            // 假设您有一个方法来获取棋子类型
            auto piece = game_->getBoard()->getPiece(i, j);
            if (piece) { // 检查是否是车
                // 计算车的绘制位置
                QPoint position = boardToPixel(i, j);
                
                // 设置透明度
                painter.setOpacity(1.0); // 设置透明度为50%
                
                // 绘制车的图片
                painter.drawPixmap(position.x() - scaledRookPixmap.width() / 2, position.y() - scaledRookPixmap.height() / 2, scaledRookPixmap);
                
                // 恢复透明度
                painter.setOpacity(1.0);
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
