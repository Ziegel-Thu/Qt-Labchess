#include "PlayerInfoWidget.h"
#include <QVBoxLayout>

PlayerInfoWidget::PlayerInfoWidget(std::shared_ptr<Game> game, QWidget *parent) : QWidget(parent), game_(game) {
    auto layout = new QVBoxLayout(this);

    player1Label_ = std::make_shared<QLabel>(this);
    player2Label_ = std::make_shared<QLabel>(this);
    currentPlayerLabel_ = std::make_shared<QLabel>(this);

    layout->addWidget(player1Label_.get());
    layout->addWidget(player2Label_.get());
    layout->addWidget(currentPlayerLabel_.get());

    connect(game_.get(), &Game::playerLanguageSwitchRequested, this, &PlayerInfoWidget::update);

    update();
}

void PlayerInfoWidget::update() {
    if(game_->isLanguageChinese_){
        player1Label_->setText(QString("%1 （红方）").arg(game_->getPlayers()[0]->getName().c_str()));
        player2Label_->setText(QString("%1 （蓝方）").arg(game_->getPlayers()[1]->getName().c_str()));
    }
    else{
        player1Label_->setText(QString("%1 (Red)").arg(game_->getPlayers()[0]->getName().c_str()));
        player2Label_->setText(QString("%1 (Blue)").arg(game_->getPlayers()[1]->getName().c_str()));
    }
    
    auto currentPlayer = game_->getCurrentPlayer();
    if (currentPlayer) {
        if (game_->isGameOver()) {
            if(game_->isLanguageChinese_){
                currentPlayerLabel_->setText(QString("游戏结束，%1 获胜").arg(currentPlayer->getName().c_str()));
            }
            else{
                currentPlayerLabel_->setText(QString("%1 wins").arg(currentPlayer->getName().c_str()));
            }
        } else {
            if(game_->isLanguageChinese_){
                currentPlayerLabel_->setText(QString("当前玩家: %1").arg(currentPlayer->getName().c_str()));
            }
            else{
                currentPlayerLabel_->setText(QString("Current Player: %1").arg(currentPlayer->getName().c_str()));
            }
        }
    } else {
        if(game_->isLanguageChinese_){
            currentPlayerLabel_->setText("游戏未开始");
        }
        else{
            currentPlayerLabel_->setText("Game not started");
        }
    }
}