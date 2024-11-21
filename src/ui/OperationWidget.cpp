#include "OperationWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

OperationWidget::OperationWidget(QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);

    startButton_ = std::make_shared<QPushButton>("开始游戏", this);
    timemachineButtonBeta_ = std::make_shared<QPushButton>("Beta的时光机", this);
    timemachineButtonRui_ = std::make_shared<QPushButton>("Rui的时光机", this);
    undoButton_ = std::make_shared<QPushButton>("回退", this);
    redoButton_ = std::make_shared<QPushButton>("前进", this);
    passButton_ = std::make_shared<QPushButton>("放弃", this);
    confirmButton_ = std::make_shared<QPushButton>("确认", this);

    // 设置回退和前进按钮初始不可见
    undoButton_->setVisible(false);
    redoButton_->setVisible(false);

    layout->addWidget(startButton_.get());
    layout->addWidget(timemachineButtonBeta_.get());
    layout->addWidget(timemachineButtonRui_.get());
    timemachineButtonBeta_->setVisible(false);
    timemachineButtonRui_->setVisible(false);
    layout->addWidget(undoButton_.get());
    layout->addWidget(redoButton_.get());
    layout->addWidget(passButton_.get());
    layout->addWidget(confirmButton_.get());
    passButton_->setVisible(false);
    confirmButton_->setVisible(false);
    connect(startButton_.get(), &QPushButton::clicked, this, &OperationWidget::startGameRequested);
    connect(undoButton_.get(), &QPushButton::clicked, this, &OperationWidget::undoRequested);
    connect(timemachineButtonBeta_.get(), &QPushButton::clicked, this, &OperationWidget::timemachineRequested);
    connect(timemachineButtonRui_.get(), &QPushButton::clicked, this, &OperationWidget::timemachineRequested);
    connect(redoButton_.get(), &QPushButton::clicked, this, &OperationWidget::redoRequested);
    connect(passButton_.get(), &QPushButton::clicked, this, &OperationWidget::passRequested);
    connect(confirmButton_.get(), &QPushButton::clicked, this, &OperationWidget::confirmRequested);
}

std::shared_ptr<QPushButton> OperationWidget::getTimeMachineButtonBeta() const {
    return timemachineButtonBeta_;
}

std::shared_ptr<QPushButton> OperationWidget::getTimeMachineButtonRui() const {
    return timemachineButtonRui_;
}

void OperationWidget::showUndoRedoButtons(bool show) {
    undoButton_->setVisible(show);
    redoButton_->setVisible(show);
}
void OperationWidget::showPassConfirmButtons(bool show){
    passButton_->setVisible(show);
    confirmButton_->setVisible(show);
}