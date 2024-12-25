#pragma once

#include <memory>
#include <QWidget>
#include <QPushButton>
#include "../core/Game.h"
class OperationWidget : public QWidget {
    Q_OBJECT

public:
    OperationWidget(QWidget *parent = nullptr, std::shared_ptr<Game> game = nullptr);
    std::shared_ptr<QPushButton> getTimeMachineButton1() const;
    std::shared_ptr<QPushButton> getTimeMachineButton0() const;

    void showUndoRedoButtons(bool show);
    void showPassConfirmButtons(bool show);
    void showInputNameButtons(bool show);
    void languageSwitchRequested();
signals:
    void startGameRequested();
    void undoRequested();
    void timemachineRequested();
    void redoRequested();
    void passRequested();
    void confirmRequested();
    void inputNameRequested();
    void updateTimeMachineButton();
private:
    std::shared_ptr<Game> game_;
    std::shared_ptr<QPushButton> startButton_;
    std::shared_ptr<QPushButton> inputNameButton_;
    std::shared_ptr<QPushButton> languageSwitchButton_;
    std::shared_ptr<QPushButton> undoButton_;
    std::shared_ptr<QPushButton> timemachineButton1_;
    std::shared_ptr<QPushButton> timemachineButton0_;
    std::shared_ptr<QPushButton> redoButton_;
    std::shared_ptr<QPushButton> passButton_;
    std::shared_ptr<QPushButton> confirmButton_;
};