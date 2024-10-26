#include "OperationWidget.h"
#include <QVBoxLayout>
#include <QPushButton>

OperationWidget::OperationWidget(QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);

    startButton_ = std::make_shared<QPushButton>("开始游戏", this);
    timemachineButton_ = std::make_shared<QPushButton>("时光机",this);
    undoButton_ = std::make_shared<QPushButton>("悔棋", this);

    layout->addWidget(startButton_.get());
    layout->addWidget(timemachineButton_.get());
    layout->addWidget(undoButton_.get());

    connect(startButton_.get(), &QPushButton::clicked, this, &OperationWidget::startGameRequested);
    connect(undoButton_.get(), &QPushButton::clicked, this, &OperationWidget::undoRequested);
    connect(timemachineButton_.get(), &QPushButton::clicked, this, &OperationWidget::timemachineRequested);
}