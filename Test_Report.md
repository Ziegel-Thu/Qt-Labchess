# 测试报告————Qt-LabChess

## 1. 测试配置

本次测试利用Qtest执行，利用Xcrun执行生成llvm-cov覆盖率报告。

包含简要版覆盖率报告（txt版本）和详细版覆盖率报告（html版本）。

## 2. 测试细节

### 2.1 测试用例生成

测试本身采用AI生成Qtest主体，我对AI生成的逻辑错误和没覆盖的分支进行了人工修正。

### 2.2 数据构造

测试本身的数据主要是对每一个功能进行了遍历，从每一种棋子的每一种走法，到时光回溯和

胜负判定。国际象棋本身的走法相对有限，构造是相对平凡的。

### 2.3 测试结果

测试结果如下：

********* Start testing of TestGame *********
Config: Using QtTest library 6.7.3, Qt 6.7.3 (arm64-little_endian-lp64 shared (dynamic) release build; by Apple LLVM 16.0.0 (clang-1600.0.26.4)), macos 14.3
QDEBUG : TestGame::initTestCase() Starting test cases for Game class
PASS   : TestGame::initTestCase()
PASS   : TestGame::testInitialBoardState()
PASS   : TestGame::testSetInvalidPiece()
PASS   : TestGame::testGetInvalidPiece()
PASS   : TestGame::testPawnMovement()
PASS   : TestGame::testUndoRedoMove()
PASS   : TestGame::testKingMovement()
PASS   : TestGame::testGameOver()
PASS   : TestGame::testRookMovement()
PASS   : TestGame::testKnightMovement()
PASS   : TestGame::testBishopMovement()
PASS   : TestGame::testQueenMovement()
PASS   : TestGame::testTimeMachineSequence()
PASS   : TestGame::testTimePieceCollision()
PASS   : TestGame::testPassFunction()
PASS   : TestGame::testPass2()
PASS   : TestGame::testQueenPromotion()
PASS   : TestGame::testInvalidTimeMachineOperations()
QDEBUG : TestGame::cleanupTestCase() Finished all test cases
PASS   : TestGame::cleanupTestCase()
Totals: 19 passed, 0 failed, 0 skipped, 0 blacklisted, 22657ms
********* Finished testing of TestGame *********

获得了全部的通过。

### 2.4 覆盖率报告

覆盖率报告可以详见core_coverage.txt和core_coverage_detailed.html。

其中主体函数Game.cpp的覆盖率达到了89%，剩余的部分里大多是一些在UI中调用的函数，不在本次测试的范围内。Board.cpp达到了100%。




