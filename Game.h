#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <cstring>
#include <queue>
#include <raylib.h>
#include "board.h"
#include "UI.h"

//GAME STATE ENUM
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    void run();

private:
    Board board;
    UI ui;
    GameState currentState;         // Trạng thái hiện tại
    char currentPlayer;         // 'B' (Black) hoặc 'W' (White)
    bool isGameOver;         // Game đã kết thúc chưa
    char winner;          // 'B', 'W', hoặc 'D'

    int consecutivePasses;       // Đếm số lần pass liên tiếp
    int hoverRow, hoverCol;       // Ô đang hover chuột

    //UNDO/REDO MANAGEMENT
    std::stack<Board> history;    // Lịch sử các nước đi (cho undo)
    std::stack<Board> redoStack;    // Stack cho redo

    //  SCORE TRACKING
    int blackFinalScore;
    int whiteFinalScore;

    //  GAME LOOP
    void init();
    void resetGame();        // Reset game state
    void handleInput();        // Xử lý input từ người chơi
    void update();           // Cập nhật logic game
    void render();            // Vẽ UI
    void cleanup();

    //INPUT HANDLERS
    void handleStonePlace(int row, int col);
    void handleUndo();
    void handleRedo();
    void handlePass();
    void handleNewGame();

    //GAME Logic
    void switchPlayer();       // Chuyển lượt chơi
    void checkGameOver();       // Kiểm tra điều kiện kết thúc
    void calculateFinalScore();       // Tính điểm cuối cùng
};

#endif
