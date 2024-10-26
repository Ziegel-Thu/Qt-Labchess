#include "MainWindow.h"
#include "BoardWidget.h"
#include "PlayerInfoWidget.h"
#include "OperationWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    game_ = std::make_shared<Game>();
    setupUi();
    connectSignals();
}

std::shared_ptr<Game> MainWindow::getGame() const {
    return game_;
}

std::shared_ptr<OperationWidget> MainWindow::getOperationWidget() const {
    return operationWidget_;
}

void MainWindow::setupUi() {
    auto centralWidget = new QWidget(this);
    auto mainLayout = new QHBoxLayout(centralWidget);

    boardWidget_ = std::make_shared<BoardWidget>(game_, this);
    playerInfoWidget_ = std::make_shared<PlayerInfoWidget>(game_, this);
    operationWidget_ = std::make_shared<OperationWidget>(this);

    mainLayout->addWidget(boardWidget_.get());

    auto sideLayout = new QVBoxLayout();
    sideLayout->addWidget(playerInfoWidget_.get());
    sideLayout->addWidget(operationWidget_.get());

    mainLayout->addLayout(sideLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::connectSignals() {
    connect(operationWidget_.get(), &OperationWidget::startGameRequested, this, &MainWindow::onGameStart);
    connect(operationWidget_.get(), &OperationWidget::undoRequested, this, &MainWindow::onUndoRequested);
    connect(operationWidget_.get(), &OperationWidget::timemachineRequested, this, &MainWindow::onTimemachineRequested);
    connect(operationWidget_.get(), &OperationWidget::redoRequested, this, &MainWindow::onRedoRequested);
    connect(boardWidget_.get(), &BoardWidget::moveMade, this, &MainWindow::onPress); // 修改这里
}

void MainWindow::onGameStart() {
    game_->start();
    boardWidget_->update();
    playerInfoWidget_->update();
    updateTimeMachineButton(); // 在游戏开始时更新时光机按钮
}

void MainWindow::onGameEnd() {
    std::string str = game_->getCurrentPlayer()->getName() + " 获胜！";
    QMessageBox::information(this, "Game Over", str.c_str());
}

void MainWindow::onPress(int row, int col) {
    // 调用游戏的 press 方法
    if (game_->press(row, col)) {
        boardWidget_->update(); // 更新棋盘
        playerInfoWidget_->update(); // 更新玩家信息
    }
    if (game_->isGameOver()) {
        onGameEnd();
    }
}

void MainWindow::onTimemachineRequested() {
    // 实现时光机功能
    operationWidget_->showUndoRedoButtons(true); // 显示回退和前进按钮
}

void MainWindow::updateTimeMachineButton() {
    // 获取当前玩家的名字
    QString currentPlayerName = QString::fromStdString(game_->getCurrentPlayer()->getName()); // 获取当前玩家名字并转换为 QString

    // 更新时光机按钮的文本
    operationWidget_->getTimeMachineButton()->setText(currentPlayerName + "的时光机"); // 假设 getTimeMachineButton() 返回时光机按钮的指针
}

void MainWindow::onUndoRequested() {
    // 实现悔棋功能并更新界面显示
    if (game_->undoMove()) {
        boardWidget_->update();
        playerInfoWidget_->update();
        updateTimeMachineButton(); // 更新时光机按钮
    }
}

void MainWindow::onRedoRequested() {
    if (game_->redoMove()) {
        boardWidget_->update();
        playerInfoWidget_->update();
        updateTimeMachineButton(); // 更新时光机按钮
    }
}

void MainWindow::showUndoRedoButtons(bool show) {
    operationWidget_->showUndoRedoButtons(show); // 调用 OperationWidget 中的方法
}
