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

    drawTopStrip(painter);

    for (int row = 0; row < game_->getBoard()->getRows(); ++row) {
        for (int col = 0; col < game_->getBoard()->getCols(); ++col) {
            drawBoardCell(painter, row, col);
            auto piece = game_->getBoard()->getPiece(row, col);
            if (piece) {
                drawPiece(painter, piece, row, col, 255);
            }
        }
    }
    for(auto piece:game_->getDyingPieceList()){
        drawPiece(painter, piece, piece->getRow(), piece->getCol(), 192);
    }

    drawBottomStrip(painter);
}

void BoardWidget::drawTopStrip(QPainter& painter) {
    QColor stripColor = game_->getStep() % 2 == 1 ? QColor("#620303") : Qt::white;
    painter.fillRect(0, 0, width(), stripSize_, stripColor);
}

void BoardWidget::drawBoardCell(QPainter& painter, int row, int col) {
    QColor cellColor = (row + col) % 2 == 1 ? Qt::gray : Qt::white;
    if (game_->isPieceSelected_){
        if(game_->getBoard()->getPiece(row, col) == game_->selectedPiece_) {
            cellColor = Qt::yellow;
        }
        if(game_->isValidMove(game_->selectedPiece_,row,col,game_->getBoard()->getPiece(row,col))){
            cellColor = Qt::green;
        }
    }
    painter.fillRect(col * cellSize_, row * cellSize_ + stripSize_, 
                    cellSize_, cellSize_, cellColor);
}

void BoardWidget::drawPiece(QPainter& painter, std::shared_ptr<Piece> piece, int row, int col, int opacity) {
    // 构建图片路径
    std::string color = piece->getColor();
    std::string type = piece->getType();
    if(!piece->isAlive()&&piece->getDeathTime()<game_->getStep()){
        return;
    }
    std::string pieceImagePath = "../pics/" + color + "/" + type + ".png";

    QPixmap piecePixmap(QString::fromStdString(pieceImagePath));
    if (!piecePixmap.isNull()) {
        QPixmap scaledPiecePixmap = piecePixmap.scaled(cellSize_, cellSize_, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap tempPixmap(scaledPiecePixmap.size());
        tempPixmap.fill(Qt::transparent);

        QPainter tempPainter(&tempPixmap);
        tempPainter.setOpacity(opacity / 255.0);
        tempPainter.drawPixmap(0, 0, scaledPiecePixmap);
        
        QPoint position = boardToPixel(row, col);
        painter.drawPixmap(position.x() - tempPixmap.width() / 2, position.y() - tempPixmap.height() / 2, tempPixmap);


        // 如果是时间棋子，绘制金色指示器
        if (piece->isTimePiece()) {
            drawTimeIndicator(painter, row, col);
        }

        // 如果棋子已死亡且有死亡时间，先画遮罩，再画计时器
        if (!piece->isAlive()) {
            // 绘制整个格子的半透明遮罩
            painter.fillRect(col * cellSize_, row * cellSize_ + stripSize_, cellSize_, cellSize_, QColor(128, 128, 128, 160));  // 稍微调整透明度为160
            drawDeathTimer(painter, piece, row, col);
        }
    } else {
        qDebug() << "Failed to load piece image!";
    }
}

void BoardWidget::drawTimeIndicator(QPainter& painter, int row, int col) {
    painter.setPen(QPen(QColor(255, 215, 0), 2)); // 金色，线宽2
    
    int x = col * cellSize_;
    int y = row * cellSize_ + stripSize_;
    
    QPoint top(x + cellSize_/2, y);
    QPoint right(x + cellSize_, y + cellSize_/2);
    QPoint bottom(x + cellSize_/2, y + cellSize_);
    QPoint left(x, y + cellSize_/2);
    
    painter.drawLine(top, right);
    painter.drawLine(right, bottom);
    painter.drawLine(bottom, left);
    painter.drawLine(left, top);
}

void BoardWidget::drawDeathTimer(QPainter& painter, std::shared_ptr<Piece> piece, int row, int col) {
    QFont font = painter.font();
    font.setPointSize(20);
    font.setBold(true);
    painter.setFont(font);

    QColor textColor = piece->getColor() == "Red" ? QColor("#620303") : QColor("#2b1752");
    painter.setPen(textColor); 

    int x = col * cellSize_ + cellSize_ - 20;
    int y = row * cellSize_ + 50;
    painter.drawText(x, y, QString::number(piece->getDeathTime() - game_->getStep() + 1));
}

void BoardWidget::drawBottomStrip(QPainter& painter) {
    int bottomRowY = game_->getBoard()->getRows() * cellSize_ + stripSize_;
    QColor stripColor = (game_->getStep() % 2 == 0 && game_->getStep() != 0) ? QColor("#2b1752") : Qt::white;
    painter.fillRect(0, bottomRowY, cellSize_ * game_->getBoard()->getCols(), stripSize_, stripColor);
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