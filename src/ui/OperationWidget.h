#pragma once

#include <memory>
#include <QWidget>
#include <QPushButton>

class OperationWidget : public QWidget {
    Q_OBJECT

public:
    OperationWidget(QWidget *parent = nullptr);
    std::shared_ptr<QPushButton> getTimeMachineButton() const;
    void showUndoRedoButtons(bool show);
signals:
    void startGameRequested();
    void undoRequested();
    void timemachineRequested();
    void redoRequested();
private:
    std::shared_ptr<QPushButton> startButton_;
    std::shared_ptr<QPushButton> undoButton_;
    std::shared_ptr<QPushButton> timemachineButton_;
    std::shared_ptr<QPushButton> redoButton_;
};
