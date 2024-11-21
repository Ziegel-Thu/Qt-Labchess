#pragma once

#include <memory>
#include <QWidget>
#include <QPushButton>

class OperationWidget : public QWidget {
    Q_OBJECT

public:
    OperationWidget(QWidget *parent = nullptr);
    std::shared_ptr<QPushButton> getTimeMachineButtonBeta() const;
    std::shared_ptr<QPushButton> getTimeMachineButtonRui() const;

    void showUndoRedoButtons(bool show);
    void showPassConfirmButtons(bool show);
signals:
    void startGameRequested();
    void undoRequested();
    void timemachineRequested();
    void redoRequested();
    void passRequested();
    void confirmRequested();
private:
    std::shared_ptr<QPushButton> startButton_;
    std::shared_ptr<QPushButton> undoButton_;
    std::shared_ptr<QPushButton> timemachineButtonBeta_;
    std::shared_ptr<QPushButton> timemachineButtonRui_;
    std::shared_ptr<QPushButton> redoButton_;
    std::shared_ptr<QPushButton> passButton_;
    std::shared_ptr<QPushButton> confirmButton_;
};