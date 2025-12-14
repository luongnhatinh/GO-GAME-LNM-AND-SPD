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
#include "AI.h"

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
    // ========== COMPONENTS ==========
    Board board;                    // Logic bàn cờ
    UI ui;                          // Giao diện UI
    AI ai;                          // AI opponent

    int consecutivePasses;       // Đếm số lần pass liên tiếp
    int hoverRow, hoverCol;       // Ô đang hover chuột

    int consecutivePasses;          // Đếm số lần pass liên tiếp
    int hoverRow, hoverCol;         // Ô đang hover chuột

    // ========== AI MODE ==========
    bool isAIMode;                  // Chế độ chơi với AI
    Difficulty aiDifficulty;        // Độ khó AI (Easy/Medium/Hard)
    char aiPlayer;                  // AI chơi quân gì ('B' hoặc 'W')

    // ========== UNDO/REDO MANAGEMENT ==========
    std::stack<Board> history;      // Lịch sử các nước đi (cho undo)
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

    // ========== INPUT HANDLERS ==========
    void handleStonePlace(int row, int col);   // Xử lý đặt quân
    void handleUndo();                          // Xử lý Undo
    void handleRedo();                          // Xử lý Redo
    void handlePass();                          // Xử lý Pass
    void handleNewGame();                       // Xử lý New Game
    void handleAITurn();                        // Xử lý lượt chơi của AI

    //GAME Logic
    void switchPlayer();       // Chuyển lượt chơi
    void checkGameOver();       // Kiểm tra điều kiện kết thúc
    void calculateFinalScore();       // Tính điểm cuối cùng
};

#endif
