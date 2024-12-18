#include <QtTest>
#include <QApplication>
#include "Game.h"

class TestGame : public QObject
{
    Q_OBJECT

private:
    std::shared_ptr<Game> game;

private slots:
    void initTestCase()
    {
        // 在所有测试用例开始前运行一次
        qDebug() << "Starting test cases for Game class";
    }

    void init()
    {
        // 每个测试用例前运行
        game = std::make_shared<Game>();
        game->start();
    }

    void cleanup()
    {
        // 每个测试用例后运行
        game.reset();
    }

    void cleanupTestCase()
    {
        // 在所有测试用例结束后运行一次
        qDebug() << "Finished all test cases";
    }

    // 测试用例
    void testInitialBoardState()
    {
        // 验证初始棋盘状态
        QVERIFY(!game->isGameOver());
        QCOMPARE(game->getStep() - 1, 0);
        
        // 验证初始棋子位置
        auto board = game->getBoard();
        // 验证红方车的位置
        auto rook = board->getPiece(0, 0);
        QVERIFY(rook != nullptr);
        QCOMPARE(rook->getType(), std::string("Rook"));
        QCOMPARE(rook->getColor(), std::string("Red"));
    }
    void testSetInvalidPiece()
    {
        auto board = game->getBoard();
        auto piece = std::make_shared<Piece>("Red", "Pawn", false, 10, 10, true, -1);
        QVERIFY(!board->setPiece(10, 10, piece)
        );
    }
    
    void testGetInvalidPiece()
    {
        auto board = game->getBoard();
        auto piece = board->getPiece(10, 10);
        QVERIFY(piece == nullptr);
    }

    void testPawnMovement()
    {
        auto board = game->getBoard();
        auto pawn = board->getPiece(1, 0); // 红方兵
        
        // 测试初始两格移动
        QVERIFY(game->isValidMove(pawn, 3, 0, nullptr)); // 初始可前进两格
        game->press(1, 6);
        game->press(1, 4);
        game->press(1, 0); // 选择兵
        game->press(3, 0); // 移动两格
        QCOMPARE(board->getPiece(3, 0), pawn); // 确认移动到位
        QVERIFY(board->getPiece(1, 0) == nullptr); // 确认原位置为空
        game->press(6, 0);
        game->press(4, 0);
        // 测试移动后不能再走两格
        auto pawn2 = board->getPiece(1, 1); // 另一个初始位置的兵
        QVERIFY(game->isValidMove(pawn2, 3, 1, nullptr)); // 初始可以走两格
        game->press(1, 1);
        game->press(2, 1); // 先走一格
        QVERIFY(!game->isValidMove(pawn2, 4, 1, nullptr)); // 不能再走两格
        game->press(6, 1);
        game->press(4, 1);
        // 测试基本移动
        QVERIFY(game->isValidMove(pawn2, 3, 1, nullptr)); // 可以走一格
        QVERIFY(!game->isValidMove(pawn2, 3, 2, nullptr)); // 不能斜走（无吃子时）
        
        // 测试吃子移动
        auto enemyPiece = board->getPiece(6, 2); // 获取一个蓝方棋子
        enemyPiece->setRow(3);
        enemyPiece->setCol(2);
        board->setPiece(3, 2, enemyPiece); // 将敌方棋子放到可吃位置
        QVERIFY(game->isValidMove(pawn2, 3, 2, enemyPiece)); // 可以斜走吃子
        
        // 测试不能后退
        QVERIFY(!game->isValidMove(pawn2, 1, 1, nullptr));
        
        // 测试实际吃子
        game->press(2, 1); // 选择兵
        game->press(3, 2); // 吃子移动
        QCOMPARE(board->getPiece(3, 2), pawn2); // 确认兵移动到目标位置
        QVERIFY(board->getPiece(2, 1) == nullptr); // 确认原位置为空
        QVERIFY(!enemyPiece->isAlive()); // 确认敌方棋子被吃掉
        
        // 测试蓝方兵的初始两格移动
        auto bluePawn = board->getPiece(6, 3); // 蓝方兵
        QVERIFY(game->isValidMove(bluePawn, 4, 3, nullptr)); // 初始可前进两格
        game->press(6, 3);
        game->press(4, 3);
        QCOMPARE(board->getPiece(4, 3), bluePawn);
        QVERIFY(board->getPiece(6, 3) == nullptr);
        
        // 测试蓝方兵移动后不能再走两格
        QVERIFY(!game->isValidMove(bluePawn, 2, 3, nullptr)); // 不能再走两格
        QVERIFY(game->isValidMove(bluePawn, 3, 3, nullptr)); // 可以走一格
    }

    void testUndoRedoMove()
    {
        // 记录初始状态
        auto board = game->getBoard();
        auto redPawn = board->getPiece(1, 0);
        QVERIFY(redPawn->isTimePiece());  // 确认红方是时间子
        auto bluePawn = board->getPiece(6, 1);  // 使用非时间子的蓝方棋子
        
        // 红方移动
        game->press(1, 0);
        game->press(2, 0);
        QCOMPARE(board->getPiece(2, 0), redPawn);
        QCOMPARE(game->getStep() - 1, 1);
        
        // 蓝方移动
        game->press(6, 1);
        game->press(5, 1);
        QCOMPARE(game->getStep() - 1, 2);
        
        // 测试悔棋（应该回退一个来回）
        QVERIFY(game->undoMove());
        QCOMPARE(game->getStep() - 1, 0);  // 步数应该回到初始状态
        
        // 验证棋子位置：时间子应该保持在移动后的位置不变
        QCOMPARE(board->getPiece(2, 0), redPawn);  // 时间子保持在移动后位置
        QVERIFY(redPawn->isTimePiece());  // 确认仍然是时间子
        QCOMPARE(board->getPiece(6, 1), bluePawn);  // 蓝方棋子回到原位
        QVERIFY(board->getPiece(5, 1) == nullptr);  // 蓝方移动位置应该为空
        
        // 测试重做
        QVERIFY(game->redoMove());
        QCOMPARE(game->getStep() - 1, 2);  // 步数应该恢复到移动后
        
        // 验证重做后的状态：时间子仍然保持不变
        QCOMPARE(board->getPiece(2, 0), redPawn);  // 时间子保持在原位
        QVERIFY(redPawn->isTimePiece());  // 确认仍然是时间子
        QCOMPARE(board->getPiece(5, 1), bluePawn);  // 蓝方棋子恢复到移动位置
        QVERIFY(board->getPiece(6, 1) == nullptr);  // 蓝方原位置应该为空
    }

    void testKingMovement()
    {
        auto board = game->getBoard();
        auto king = board->getPiece(0, 4); // 红方国王
        
        // 清除国王前方的棋子以便测试
        board->setPiece(1, 4, nullptr);
        
        // 测试有效移动
        QVERIFY(game->isValidMove(king, 1, 4, nullptr)); // 前进一格
        QVERIFY(game->isValidMove(king, 1, 3, nullptr)); // 斜向移动
        
        // 测试无效移动
        QVERIFY(!game->isValidMove(king, 2, 4, nullptr)); // 不能移动两格
        QVERIFY(!game->isValidMove(king, 0, 6, nullptr)); // 不能移动两格
    }

    void testGameOver()
    {
        // 模拟一个王被吃掉的情况
        auto blueKing = game->getBoard()->getPiece(7, 4);
        blueKing->setAlive(false, game->getStep());
        
        // 执行一次移动触发检查
        game->press(1, 0);
        game->press(2, 0);
        
        QVERIFY(game->isGameOver());
    }

    void testRookMovement()
    {
        auto board = game->getBoard();
        auto rook = board->getPiece(0, 0); // 红方车
        
        // 清除前方棋子以便测试
        board->setPiece(1, 0, nullptr);
        
        // 测试直线移动
        QVERIFY(game->isValidMove(rook, 1, 0, nullptr)); // 前进一格
        QVERIFY(game->isValidMove(rook, 2, 0, nullptr)); // 前进多格
        QVERIFY(!game->isValidMove(rook, 1, 1, nullptr)); // 不能斜走
        
        // 测试吃子
        auto enemyPiece = board->getPiece(6, 0);
        board->setPiece(2, 0, enemyPiece);
        QVERIFY(game->isValidMove(rook, 2, 0, enemyPiece)); // 可以吃子
    }
    void testKnightMovement()
    {
        auto board = game->getBoard();
        auto knight = board->getPiece(0, 1); // 红方马
        
        // 清除周围的棋子以便测试
        board->setPiece(1, 1, nullptr);
        board->setPiece(1, 0, nullptr);
        
        // 测试有效的L形移动
        QVERIFY(game->isValidMove(knight, 2, 2, nullptr)); // 右前方L形移动
        QVERIFY(game->isValidMove(knight, 2, 0, nullptr)); // 左前方L形移动
        
        // 测试无效移动
        QVERIFY(!game->isValidMove(knight, 1, 1, nullptr)); // 直线移动
        QVERIFY(!game->isValidMove(knight, 3, 3, nullptr)); // 对角线移动
        QVERIFY(!game->isValidMove(knight, 0, 3, nullptr)); // 水平移动
    }

    void testBishopMovement()
    {
        auto board = game->getBoard();
        auto bishop = board->getPiece(0, 2); // 红方象
        
        // 清除路径上的棋子
        board->setPiece(1, 1, nullptr);
        board->setPiece(1, 3, nullptr);
        
        // 测试有效的对角线移动
        QVERIFY(game->isValidMove(bishop, 1, 1, nullptr)); // 左前方对角线
        QVERIFY(game->isValidMove(bishop, 1, 3, nullptr)); // 右前方对角线
        QVERIFY(game->isValidMove(bishop, 2, 4, nullptr)); // 长距离对角线
        
        // 测试无效移动
        QVERIFY(!game->isValidMove(bishop, 1, 2, nullptr)); // 直线移动
        QVERIFY(!game->isValidMove(bishop, 2, 1, nullptr)); // 非对角线移动
    }

    void testQueenMovement()
    {
        auto board = game->getBoard();
        auto queen = board->getPiece(0, 3); // 红方后
        
        // 清除所有可能路径上的棋子
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 8; j++) {
                if(i == 0 && j == 3) continue; // 保留皇后
                if(i == 0 && j == 4) continue; // 保留国王
                if(board->getPiece(i, j) != nullptr) {
                    board->setPiece(i, j, nullptr);
                }
            }
        }
        
        // 测试直线移动
        QVERIFY(game->isValidMove(queen, 1, 3, nullptr)); // 前进
        QVERIFY(game->isValidMove(queen, 0, 0, nullptr)); // 水平移动        
        // 测试对角线移动
        QVERIFY(game->isValidMove(queen, 1, 2, nullptr)); // 左前方对角线
        QVERIFY(game->isValidMove(queen, 1, 4, nullptr)); // 右前方对角线
        QVERIFY(game->isValidMove(queen, 2, 5, nullptr)); // 长距离对角线
        
        // 测试吃子
        auto enemyPiece = board->getPiece(7, 3);
        enemyPiece->setRow(2);
        enemyPiece->setCol(3);
        board->setPiece(2, 3, enemyPiece);
        QVERIFY(game->isValidMove(queen, 2, 3, enemyPiece)); // 直线吃子
        
        // 测试无效移动
        QVERIFY(!game->isValidMove(queen, 2, 2, nullptr)); // 马步移动
        QVERIFY(!game->isValidMove(queen, 1, 5, nullptr)); // 非直线非对角线移动
    }
    void testTimeMachineSequence()
    {
        auto board = game->getBoard();
        auto redPawn = board->getPiece(1, 0);
        
        // 第一步：移动红方棋子
        game->press(1, 0);
        game->press(2, 0);
        // 蓝方移动
        game->press(6, 1);  // 使用非时间子位置
        game->press(5, 1);
        QCOMPARE(board->getPiece(2, 0), redPawn);
        
        // 记录当前状态
        int currentStep = game->getStep();
        
        // 第二步：悔棋（时间子保持在移动后位置）
        game->undoMove();
        QCOMPARE(board->getPiece(2, 0), redPawn);  // 时间子保持在移动后位置
        QVERIFY(redPawn->isTimePiece());  // 验证是时间子
        QCOMPARE(game->getStep() - 1, currentStep - 3);
        
        // 第三步：确认时光机
        game->confirm();
        QCOMPARE(board->getPiece(2, 0), redPawn);  // 时间子保持在原位
        QVERIFY(redPawn->isTimePiece());  // 仍然是时间子
        
        // 第四步：新的移动
        game->press(1, 0);  // 这个位置应该是空的，因为时间子在(2,0)
        game->press(2, 0);
        QCOMPARE(board->getPiece(2, 0), redPawn);
    }

    void testTimePieceCollision()
    {
        auto board = game->getBoard();
        auto pawn = board->getPiece(1, 0);
        
        // 第一次移动
        game->press(1, 0);
        game->press(2, 0);
        
        // 悔棋但不确认
        game->undoMove();
        
        // 移动到不同位置
        game->press(1, 0);
        game->press(3, 0);
        
        // 检查时间碰撞
        auto timePiece = board->getPiece(2, 0);
        QVERIFY(timePiece != nullptr);
        QVERIFY(timePiece->isTimePiece());
    }

    void testPassFunction()
    {
        auto board = game->getBoard();
        auto pawn = board->getPiece(1, 0);
        
        // 执行移动
        game->press(1, 0);
        game->press(2, 0);
        
        // 悔棋
        game->undoMove();
        
        // 执行pass
        game->pass();
        
        // 验证状态
        QCOMPARE(board->getPiece(2, 0), pawn);
        QVERIFY(game->getStep() > 0);
    }
    void testPass2()
    {
        game->press(1, 1);
        game->press(2, 1);
        game->press(6, 1);
        game->press(5, 1);
        game->press(1, 0);
        game->press(2, 0);
        game->undoMove();
        game->pass();
        QVERIFY(game->getBoard()->getPiece(2, 1) != nullptr);
    }

    void testQueenPromotion()
    {
        auto board = game->getBoard();
        auto pawn = board->getPiece(1, 0);
        
        // 清除路径上的所有棋子
        for(int i = 2; i <= 7; i++) {
            board->setPiece(i, 0, nullptr);
        }
        for (int i = 5; i>= 0; i--){
            board->setPiece(i, 4, nullptr);

        }
        
        // 移动兵到升变位置
        for (int i = 1; i<= 6;i++){
        game->press(i, 0);
        game->press(i+1, 0);
        game->press(7-i,4);
        game->press(6-i,4);
        }
        
        // 验证升变
        auto promotedPiece = board->getPiece(7, 0);
        QVERIFY(promotedPiece != nullptr);
        QCOMPARE(promotedPiece->getType(), std::string("Queen"));
    }


    void testInvalidTimeMachineOperations()
    {
        // 测试在没有历史记录时的悔棋
        QVERIFY(!game->undoMove());
        
        // 测试在没有悔棋记录时的重做
        QVERIFY(!game->redoMove());
        
        // 测试在游戏结束后的操作
        game->end();
        QVERIFY(!game->undoMove());
        QVERIFY(!game->redoMove());
    }
};

QTEST_MAIN(TestGame)
#include "tst_game.moc" 