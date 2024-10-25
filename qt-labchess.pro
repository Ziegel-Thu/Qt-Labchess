QT       += core gui widgets

TARGET = qt-labchess
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/ui/MainWindow.cpp \
    src/ui/BoardWidget.cpp \
    src/ui/PlayerInfoWidget.cpp \
    src/ui/OperationWidget.cpp \
    src/core/Game.cpp \
    src/core/Board.cpp \
    src/core/Piece.cpp \
    src/core/Player.cpp 

HEADERS += \
    src/ui/MainWindow.h \
    src/ui/BoardWidget.h \
    src/ui/PlayerInfoWidget.h \
    src/ui/OperationWidget.h \
    src/core/Game.h \
    src/core/Board.h \
    src/core/Piece.h \
    src/core/Player.h 

INCLUDEPATH += \
    src \
    src/core \
    src/ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target