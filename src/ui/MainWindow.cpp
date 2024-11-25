#include "MainWindow.h"
#include "BoardWidget.h"
#include "PlayerInfoWidget.h"
#include "OperationWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <iostream>
#include <thread>
#include <QApplication>
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
    operationWidget_ = std::make_shared<OperationWidget>(this, game_);

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
    connect(operationWidget_.get(), &OperationWidget::passRequested, this, &MainWindow::onPassRequested);
    connect(operationWidget_.get(), &OperationWidget::confirmRequested, this, &MainWindow::onConfirmRequested);
    connect(boardWidget_.get(), &BoardWidget::moveMade, this, &MainWindow::onPress); // 修改这里
    connect(game_.get(), &Game::updateGameTimeMachineButton, this, &MainWindow::updateTimeMachineButton);}


void MainWindow::onGameStart() {
    game_->start();
    boardWidget_->update();
    playerInfoWidget_->update();
    operationWidget_->showUndoRedoButtons(false);
    operationWidget_->showPassConfirmButtons(false);
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
    operationWidget_->showPassConfirmButtons(true);
    game_->setSelectable(false);

}

void MainWindow::updateTimeMachineButton() {
    // 获取当前玩家的名字
    QString currentPlayerName = QString::fromStdString(game_->getCurrentPlayer()->getName()); // 获取当前玩家名字并转换为 QString
    operationWidget_->getTimeMachineButton1()->setVisible(false);
    operationWidget_->getTimeMachineButton0()->setVisible(false);
    if((currentPlayerName==QString::fromStdString(game_->players_[1]->getName())&&game_->machineNumber1_!=0)){
        operationWidget_->getTimeMachineButton1()->setVisible(true);

    }
    if((currentPlayerName==QString::fromStdString(game_->players_[0]->getName())&&game_->machineNumber0_!=0)){
        operationWidget_->getTimeMachineButton0()->setVisible(true);
    }


}

void MainWindow::onUndoRequested() {
    if (game_->undoMove()) {
        playerInfoWidget_->update();
        boardWidget_->update();
    }
    if(game_->undoMove()){
        playerInfoWidget_->update();
        boardWidget_->update();
    }
    updateTimeMachineButton();
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

void MainWindow::onPassRequested(){
    operationWidget_->showUndoRedoButtons(false);
    operationWidget_->showPassConfirmButtons(false);
    game_->pass();
    boardWidget_->update();
    playerInfoWidget_->update();
    updateTimeMachineButton(); 
}

void MainWindow::onConfirmRequested(){
    operationWidget_->showUndoRedoButtons(false);
    operationWidget_->showPassConfirmButtons(false);
    game_->confirm();
    boardWidget_->update();
    playerInfoWidget_->update();
    updateTimeMachineButton();
}