#include "OperationWidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <iostream>

OperationWidget::OperationWidget(QWidget *parent, std::shared_ptr<Game> game) : QWidget(parent), game_(game) {
    auto layout = new QVBoxLayout(this);

    startButton_ = std::make_shared<QPushButton>("开始新游戏", this);
    inputNameButton_ = std::make_shared<QPushButton>("自定义昵称", this);
    languageSwitchButton_ = std::make_shared<QPushButton>("中/En", this);
    timemachineButton1_ = std::make_shared<QPushButton>(QString::fromStdString(game_->players_[1]->getName())+"的时光机", this);
    timemachineButton0_ = std::make_shared<QPushButton>(QString::fromStdString(game_->players_[0]->getName())+"的时光机", this);
    undoButton_ = std::make_shared<QPushButton>("回退", this);
    redoButton_ = std::make_shared<QPushButton>("前进", this);
    passButton_ = std::make_shared<QPushButton>("放弃", this);
    confirmButton_ = std::make_shared<QPushButton>("确认", this);

    // 设置回退和前进按钮初始不可见
    undoButton_->setVisible(false);
    redoButton_->setVisible(false);

    layout->addWidget(startButton_.get());
    layout->addWidget(inputNameButton_.get());
    layout->addWidget(languageSwitchButton_.get());
    layout->addWidget(timemachineButton1_.get());
    layout->addWidget(timemachineButton0_.get());
    timemachineButton1_->setVisible(false);
    timemachineButton0_->setVisible(false);
    layout->addWidget(undoButton_.get());
    layout->addWidget(redoButton_.get());
    layout->addWidget(passButton_.get());
    layout->addWidget(confirmButton_.get());
    passButton_->setVisible(false);
    confirmButton_->setVisible(false);
    connect(startButton_.get(), &QPushButton::clicked, this, &OperationWidget::startGameRequested);
    connect(inputNameButton_.get(), &QPushButton::clicked, this, &OperationWidget::inputNameRequested);
    connect(languageSwitchButton_.get(), &QPushButton::clicked, this, &OperationWidget::languageSwitchRequested);
    connect(undoButton_.get(), &QPushButton::clicked, this, &OperationWidget::undoRequested);
    connect(timemachineButton1_.get(), &QPushButton::clicked, this, &OperationWidget::timemachineRequested);
    connect(timemachineButton0_.get(), &QPushButton::clicked, this, &OperationWidget::timemachineRequested);
    connect(redoButton_.get(), &QPushButton::clicked, this, &OperationWidget::redoRequested);
    connect(passButton_.get(), &QPushButton::clicked, this, &OperationWidget::passRequested);
    connect(confirmButton_.get(), &QPushButton::clicked, this, &OperationWidget::confirmRequested);
}

std::shared_ptr<QPushButton> OperationWidget::getTimeMachineButton1() const {
    return timemachineButton1_;
}

std::shared_ptr<QPushButton> OperationWidget::getTimeMachineButton0() const {
    return timemachineButton0_;
}

void OperationWidget::showUndoRedoButtons(bool show) {
    undoButton_->setVisible(show);
    redoButton_->setVisible(show);
}
void OperationWidget::showPassConfirmButtons(bool show){
    passButton_->setVisible(show);
    confirmButton_->setVisible(show);
}

void OperationWidget::languageSwitchRequested(){
    if(game_-> isLanguageChinese_){
        startButton_->setText("Start New Game");
        inputNameButton_->setText("Customize Name");
        undoButton_->setText("Undo");
        redoButton_->setText("Redo");
        passButton_->setText("Pass");
        confirmButton_->setText("Confirm");}
    else{
        startButton_->setText("开始新游戏");
        inputNameButton_->setText("自定义昵称");
        undoButton_->setText("回退");
        redoButton_->setText("前进");
        passButton_->setText("放弃");
        confirmButton_->setText("确认");
    }
    game_->languageSwitch();
    emit updateTimeMachineButton();
}