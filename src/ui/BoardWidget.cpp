#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QDir>
#include <iostream>

BoardWidget::BoardWidget(std::shared_ptr<Game> game, QWidget *parent) : QWidget(parent), game_(game) {
    cellSize_ = 100;
    stripSize_ = 30;
    setFixedSize(cellSize_ * game_->getBoard()->getCols(), cellSize_ * game_->getBoard()->getRows()+2*stripSize_);
}

void BoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 添加顶部行
    painter.fillRect(0, 0, width(), stripSize_, game_->getStep()%2==1 ? QColor("#620303") : Qt::white);

    // 绘制棋盘
    for (int row = 0; row < game_->getBoard()->getRows(); ++row) {
        for (int col = 0; col < game_->getBoard()->getCols(); ++col) {
            // 计算方格的颜色
            if ((row + col) % 2 == 1) {
                painter.fillRect(col * cellSize_, row * cellSize_ + stripSize_, cellSize_, cellSize_, Qt::gray); // 黑色方格
            } else {
                painter.fillRect(col * cellSize_, row * cellSize_ + stripSize_, cellSize_, cellSize_, Qt::white); // 白色方格
            }
            if(game_->isPieceSelected_){
                if(game_->getBoard()->getPiece(row,col)==game_->selectedPiece_){
                    painter.fillRect(col * cellSize_, row * cellSize_ + stripSize_, cellSize_, cellSize_, Qt::yellow); // 黄色方格
                }
            }

            // 获取棋子
            auto piece = game_->getBoard()->getPiece(row, col);
            if (piece) {
                // 根据棋子的颜色和类型构建图像路径
                std::string color = piece->getColor();
                std::string type = piece->getType();
                std::string pieceImagePath = "../pics/" + color + "/" + type + ".png"; // 构建路径

                QPixmap piecePixmap(QString::fromStdString(pieceImagePath)); // 使用棋子图像路径
                if (!piecePixmap.isNull()) {
                    // 缩放图片
                    QPixmap scaledPiecePixmap = piecePixmap.scaled(cellSize_, cellSize_, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    // 计算棋子的绘制位置
                    QPoint position = boardToPixel(row, col);
                    // 绘制棋子
                    painter.drawPixmap(position.x() - scaledPiecePixmap.width() / 2, position.y() - scaledPiecePixmap.height() / 2, scaledPiecePixmap);
                } else {
                    qDebug() << "Failed to load piece image!";
                }

                if (!piece->isAlive()&&piece->getDeathTime()>0) {
                    // 设置字体为粗体
                    QFont font = painter.font();
                    font.setPointSize(20); // 设置字体大小为12
                    font.setBold(true); // 设置为粗体
                    painter.setFont(font);

                    // 根据棋子的颜色设置文本颜色
                    QColor textColor;
                    if (piece->getColor() == "Red") {
                        textColor = QColor("#620303"); // 红色
                    } else if (piece->getColor() == "Blue") {
                        textColor = QColor("#2b1752"); // 蓝色
                    }

                    painter.setPen(textColor); // 使用指定的颜色

                    // 在棋子右上角标注寿命，稍微留出边距
                    int x = col * cellSize_ + cellSize_ - 20; // 右边距
                    int y = row * cellSize_ + 50; // 上边距
                    painter.drawText(x, y, QString::number(piece->getDeathTime()-game_->getStep()+1));
                }
            }
        }
    }

    // 添加底部行
    int bottomRowY = game_->getBoard()->getRows() * cellSize_+stripSize_; // 底部行的Y坐标
    if (game_->getStep()%2==0&&game_->getStep()!=0) {
        painter.fillRect(0, bottomRowY, cellSize_ * game_->getBoard()->getCols(), stripSize_, QColor("#2b1752")); 
    } else {
        painter.fillRect(0, bottomRowY, cellSize_ * game_->getBoard()->getCols(), stripSize_, Qt::white); 
    }


}

void BoardWidget::mousePressEvent(QMouseEvent *event) {
    QPoint boardPos = pixelToBoard(event->pos());
    emit moveMade(boardPos.y(), boardPos.x()); // 这里保持不变
}

QPoint BoardWidget::boardToPixel(int row, int col) const {
    return QPoint(col * cellSize_ + cellSize_ / 2, row * cellSize_ + cellSize_ / 2 + stripSize_);
}

QPoint BoardWidget::pixelToBoard(const QPoint& pixel) const {
    return QPoint(pixel.x() / cellSize_, (pixel.y() - 30) / cellSize_);
}