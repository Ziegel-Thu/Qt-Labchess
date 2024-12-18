QT += testlib
QT += widgets
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

# 源文件路径
INCLUDEPATH += \
    ../core \
    ../ui

# 测试源文件
SOURCES += \
    test_game.cpp

# 核心模块源文件
SOURCES += \
    ../core/Game.cpp \
    ../core/Player.cpp \
    ../core/Board.cpp \
    ../core/Piece.cpp

# 核心模块头文件
HEADERS += \
    ../core/Game.h \
    ../core/Player.h \
    ../core/Board.h \
    ../core/Piece.h

# 代码覆盖率配置
QMAKE_CXXFLAGS += -fprofile-instr-generate -fcoverage-mapping
QMAKE_LFLAGS += -fprofile-instr-generate -fcoverage-mapping

# 使用明确的文件名
DEFINES += LLVM_PROFILE_FILE=\"default.profraw\"

# 使用 xcrun 确保使用系统正确的工具链
QMAKE_CC = xcrun clang
QMAKE_CXX = xcrun clang++
QMAKE_LINK = xcrun clang++
QMAKE_LINK_C = xcrun clang
