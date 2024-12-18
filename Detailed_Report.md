# 详细报告————TimeChess

## 1. 功能介绍

TimeChess 是一个基于 Qt 的五子棋游戏，支持双人对战，拥有Qt打造的图形界面和形象的棋子。

在基础的国际象棋走子规则（不包含升变成后以外的棋子，过路兵，王车易位）之外，扩充了Time_Piece的概念。

配合Time_Piece的有一个Time_Machine，可以回溯到任意一个轮到该玩家操作的时间点，查看棋盘状态，选择继续回溯或者返回，确定或者放弃回溯。

Time_Piece将在时间回溯中保持不动，时间回溯可以复活死亡的棋子，但是死亡的棋子会有生命周期至其原定死亡的地方。

在回溯中位置被Time_Piece占据的棋子将直接死亡，但可以在下一次回溯中被复活。

## 2.结构设计

具体的代码结构可以详见structure.txt和其他的具体程序。这里将简要介绍其中的逻辑

整体代码分为UI部分，core部分和test部分。

### 2.1 UI部分

UI部分主要处理图形界面，包括了主棋盘，各种按钮，游戏相关信息，还有游戏的控制端。

PlayerInfoWidget.h和PlayerInfoWidget.cpp主要负责显示游戏相关信息，包括当前玩家，游戏状态，以及两个玩家的信息。

BoardWidget.h和BoardWidget.cpp主要负责棋盘的绘制，包括棋盘的背景，棋子的绘制。

OperationWidget.h和OperationWidget.cpp主要负责游戏的控制，包括开始游戏，回溯，前进，放弃，确认，输入名字的点击信号接收，传递给MainWindow。

MainWindow.h和MainWindow.cpp主要负责将各个部分收到的信息整合在一起，作为函数处理，在函数内部调用core的内容，主要是Game.cpp。

### 2.2 core部分

core部分主要维护游戏逻辑，包括棋盘和棋子的关系，棋子的选择，移动，生存与死亡，时间性质，以及处理时间回溯，游戏记录和玩家的相关信息。

Game.h和Game.cpp主要负责游戏逻辑，包括棋盘和棋子的关系，棋子的选择，移动，吃，生存与死亡，时间回溯，还有游戏的记录，玩家的相关信息。

Board.h和Board.cpp主要负责棋盘信息的存储和读取

Piece.h和Piece.cpp主要负责棋子信息的存储和读取

Player.h和Player.cpp主要负责玩家信息的存储和读取

### 2.3 test部分

test部分主要负责测试core的功能，包括棋盘的绘制，棋子的移动，吃，生存与死亡，时间回溯，还有游戏的记录，玩家的相关信息。

## 3. 功能实现

### 3.1 棋盘绘制

利用paint event作出各种矩形，并利用QPainter绘制棋子。

同时棋盘上下方各有一个长条，在该玩家的回合会亮起该玩家的标志色。

在一个棋子被选中的时候将会黄色高亮这个格子。

如果一个位置是合法的走子目标，将会绿色高亮这个格子。

### 3.2 棋子绘制

棋子的图片来源于pic文件夹，是我在网上下载并编辑了色调，更改成了透明的形式。

棋子用pixmap更改大小，并利用QPainter绘制棋子。

对于复活的死亡棋子，会在右上角用标志色标出他的生命周期。

对于在回溯过程中被Time_Piece占据的棋子，会半透明地覆盖显示。他的实现技术是创建了额外的list，并用Qpainter一并按照绘制规则绘制。

### 3.3 走棋

走棋的逻辑比较复杂，需要考虑选棋子和走棋子两种可能。不仅包括不合法的选棋判断，还有更换已选棋子。

press函数被嵌套在一个while循环中，循环条件是true，表示在非时间回溯阶段需要不断下棋直到游戏结束。

首先先用selectable_判断是否是走棋阶段，如果不是就false返回。

然后创建isPieceSelected_判断是否已经选棋：

- 如果没选棋子，那就判断是否选中了自己的棋子，然后返回true进行下一步操作

- 如果选棋了那就要判断下一个目标位置是否是自己的棋子，是则更改已选棋子

- 如果目标位置是空或者对方棋子，那就用isValidMove函数观察是否走子

  - 如果走子成功，那就更新相关数据，记录棋盘数据

  - 如果涉及棋子死亡，那就更新死亡棋子数据

  - 如果涉及升变成后，那就增加一个后放在棋盘上

- 如果走棋失败那就要返回true让它继续选棋

### 3.4 时间回溯

每次走棋之后，都会将棋盘数据和棋子数据记录在moveHistory_中。

在需要时间回溯的时候，会从moveHistory_中取出数据，放入viewedMoveHistory_中，并更新棋盘数据和棋子数据。

- 如果涉及时间棋子，则直接跳过更新，节约计算量

- 如果涉及位置被Time_Piece占据，则将该棋子放入濒死棋子列表中，交由BoardWidget处理

在需要时间前进的时候，会从viewedMoveHistory_中取出数据，放入moveHistory_中，并更新棋盘数据和棋子数据。

在确认时间回溯的时候，所有当前棋盘的棋子将会被设置为当前的状况，把濒死棋子列表中的棋子设置为死亡状态，并更新棋盘数据和棋子数据，扣减时光回溯次数。

在跳过时间回溯的时候，将会把viewedMoveHistory_中的数据清空，并将其底部的数据用于更新棋盘数据和棋子数据。

### 3.5 游戏结束

会在必要的地方检测王的存活情况，如果检测到王死亡，则游戏结束，并让PlayerInfoWidget显示游戏结束，公布获胜玩家。

## 4.类图

classDiagram
    class Game {
        -board_: Board
        -players_: MyVector<Player>
        -currentPlayer_: Player
        -gameOver_: bool
        -selectedPiece_: Piece
        -isPieceSelected_: bool
        -selectable_: bool
        -moveHistory_: MyVector<MyVector<tuple>>
        -viewedMoveHistory_: MyVector<MyVector<tuple>>
        -timePiecePosition_: MyVector<tuple>
        -dyingPieceList_: MyVector<Piece>
        -numQueenAdditional_: int
        -machineNumber1_: int
        -machineNumber0_: int
        +redKing_: Piece
        +blueKing_: Piece
        +pieceList_: MyVector<Piece>
        
        +Game()
        +start()
        +inputName()
        +end()
        +undoMove(): bool
        +redoMove(): bool
        +press(row: int, col: int): bool
        +switchPlayer()
        +initializeChessPieces()
        +isGameOver(): bool
        +getBoard(): Board
        +getDyingBoard(): Board
        +getPlayers(): MyVector<Player>
        +getCurrentPlayer(): Player
        +isValidMove(): bool
        +updateDeathTime()
        +isKingAlive(): bool
        +getStep(): int
        +getDyingPieceList(): MyVector<Piece>
        +setSelectable(bool)
        +pass()
        +confirm()
        -isPathClear(): bool
        -initializePlayers()
        -updateMoveHistory()
        -undoBoard()
        -undoHistory()
        -redoBoard()
        -redoHistory()
        -updateTimePiecePosition()
        -handlePieceCrashing()
    }

    class Board {
        -rows_: int
        -cols_: int
        -grid_: MyVector<MyVector<Piece>>
        +Board(rows: int, cols: int)
        +initialize()
        +setPiece(row: int, col: int, piece: Piece): bool
        +getPiece(row: int, col: int): Piece
        +isValidPosition(row: int, col: int): bool
        +getRows(): int
        +getCols(): int
    }

    class Piece {
        -color_: string
        -type_: string
        -row_: int
        -col_: int
        -isTimePiece_: bool
        -isAlive_: bool
        -deathTime_: int
        +Piece(color: string, type: string, isTimePiece: bool, row: int, col: int)
        +getColor(): string
        +getType(): string
        +isTimePiece(): bool
        +isAlive(): bool
        +setAlive(isAlive: bool, deathTime: int)
        +getDeathTime(): int
        +setDeathTime(int)
        +setRow(int)
        +setCol(int)
        +getRow(): int
        +getCol(): int
        +setColor(string)
        +setType(string)
        +setTimePiece(bool)
    }

    class Player {
        +name_: string
        #color_: string
        +Player(name: string, color: string)
        +getName(): string
        +getColor(): string
        +setName(name: string)
        +setColor(color: string)
    }

    %% 继承关系
    QObject <|-- Game

    %% 关联关系
    Game "1" *-- "1" Board
    Game "1" *-- "2" Player
    Game "1" o-- "*" Piece
    Board "1" o-- "*" Piece
    



