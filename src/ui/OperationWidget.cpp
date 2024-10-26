#include "OperationWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

OperationWidget::OperationWidget(QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);

    startButton_ = std::make_shared<QPushButton>("开始游戏", this);
    timemachineButton_ = std::make_shared<QPushButton>("时光机", this);
    undoButton_ = std::make_shared<QPushButton>("回退", this);
    redoButton_ = std::make_shared<QPushButton>("前进", this);

    // 设置回退和前进按钮初始不可见
    undoButton_->setVisible(false);
    redoButton_->setVisible(false);

    layout->addWidget(startButton_.get());
    layout->addWidget(timemachineButton_.get());
    layout->addWidget(undoButton_.get());
    layout->addWidget(redoButton_.get());
    connect(startButton_.get(), &QPushButton::clicked, this, &OperationWidget::startGameRequested);
    connect(undoButton_.get(), &QPushButton::clicked, this, &OperationWidget::undoRequested);
    connect(timemachineButton_.get(), &QPushButton::clicked, this, &OperationWidget::timemachineRequested);
    connect(redoButton_.get(), &QPushButton::clicked, this, &OperationWidget::redoRequested);
}

std::shared_ptr<QPushButton> OperationWidget::getTimeMachineButton() const {
    return timemachineButton_;
}

void OperationWidget::showUndoRedoButtons(bool show) {
    undoButton_->setVisible(show);
    redoButton_->setVisible(show);
}
