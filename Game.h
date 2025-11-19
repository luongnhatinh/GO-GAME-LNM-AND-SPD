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

class Game {
public:
    // Hàm duy nhất được gọi từ main()
    void run();

private:
    // ========== COMPONENTS ==========
    Board board;                    // Logic bàn cờ
    UI ui;                          // Giao diện UI

    // ========== GAME STATE ==========
    char currentPlayer;             // 'B' (Black) hoặc 'W' (White)
    bool isGameOver;                // Game đã kết thúc chưa?
    char winner;                    // 'B', 'W', hoặc 'D' (Draw)

    int consecutivePasses;          // Đếm số lần pass liên tiếp
    int hoverRow, hoverCol;         // Ô đang hover chuột

    // ========== SCORE TRACKING ==========
    int blackFinalScore;
    int whiteFinalScore;

    // ========== GAME LOOP ==========
    void init();                    // Khởi tạo game (window + state)
    void resetGame();               // Reset game state (không tạo window mới)
    void handleInput();             // Xử lý input từ người chơi
    void update();                  // Cập nhật logic game
    void render();                  // Vẽ UI
    void cleanup();                 // Dọn dẹp khi thoát

    // ========== INPUT HANDLERS ==========
    void handleStonePlace(int row, int col);   // Xử lý đặt quân
    void handleUndo();                          // Xử lý Undo
    void handleRedo();                          // Xử lý Redo
    void handlePass();                          // Xử lý Pass
    void handleNewGame();                       // Xử lý New Game

    // ========== GAME LOGIC ==========
    void switchPlayer();                        // Chuyển lượt chơi
    void checkGameOver();                       // Kiểm tra điều kiện kết thúc
    void calculateFinalScore();                 // Tính điểm cuối cùng
};

#endif // GAME_H_INCLUDED
