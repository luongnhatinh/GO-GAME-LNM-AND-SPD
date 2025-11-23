#include "UI.h"
#include <raylib.h>
#include <string>

// ========== CONSTRUCTOR/DESTRUCTOR ==========
UI::UI() {
    // Khởi tạo màu sắc
    boardColor = Color{220, 179, 92, 255};      // Màu gỗ cho bàn cờ
    lineColor = BLACK;
    starPointColor = BLACK;
    blackStoneColor = Color{30, 30, 30, 255};
    whiteStoneColor = Color{245, 245, 245, 255};
    hoverColor = Color{100, 100, 255, 100};     // Màu xanh hover
}

UI::~UI() {
    // Destructor (không cần làm gì)
}

// ========== KHỞI TẠO VÀO WINDOW RAYLIB ==========
void UI::init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Go Game - Cờ Vây");
    SetTargetFPS(60);
    initButtons();
}

void UI::cleanup() {
    CloseWindow();
}

// ========== KHỞI TẠO VỊ TRÍ CÁC NÚT ==========
void UI::initButtons() {
    int buttonX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 50;
    int buttonY = BOARD_OFFSET_Y;
    int buttonWidth = 150;
    int buttonHeight = 50;
    int buttonSpacing = 60;

    undoButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    redoButton = {(float)buttonX, (float)(buttonY + buttonSpacing), (float)buttonWidth, (float)buttonHeight};
    passButton = {(float)buttonX, (float)(buttonY + buttonSpacing * 2), (float)buttonWidth, (float)buttonHeight};
    newGameButton = {(float)buttonX, (float)(buttonY + buttonSpacing * 3), (float)buttonWidth, (float)buttonHeight};
}

// ========== BẮT ĐẦU VẼ/KẾT THÚC VẼ ==========
void UI::beginDrawing() {
    BeginDrawing();
    ClearBackground(Color{180, 140, 80, 255});  // Màu nền gỗ
}

void UI::endDrawing() {
    EndDrawing();
}

bool UI::shouldClose() {
    return WindowShouldClose();
}

// ========== VẼ BÀN CỜ ==========
void UI::drawBoard() {
    // Vẽ nền bàn cờ
    int boardPixelSize = (BOARD_SIZE - 1) * CELL_SIZE;
    DrawRectangle(
        BOARD_OFFSET_X - 20,
        BOARD_OFFSET_Y - 20,
        boardPixelSize + 40,
        boardPixelSize + 40,
        boardColor
    );

    // Vẽ grid lines
    drawGridLines();

    // Vẽ star points (9 điểm đặc biệt)
    drawStarPoints();

    // Vẽ tọa độ
    drawCoordinates();
}

void UI::drawGridLines() {
    int boardPixelSize = (BOARD_SIZE - 1) * CELL_SIZE;

    // Vẽ 19 đường ngang
    for (int i = 0; i < BOARD_SIZE; i++) {
        int y = BOARD_OFFSET_Y + i * CELL_SIZE;
        DrawLine(
            BOARD_OFFSET_X,
            y,
            BOARD_OFFSET_X + boardPixelSize,
            y,
            lineColor
        );
    }

    // Vẽ 19 đường dọc
    for (int i = 0; i < BOARD_SIZE; i++) {
        int x = BOARD_OFFSET_X + i * CELL_SIZE;
        DrawLine(
            x,
            BOARD_OFFSET_Y,
            x,
            BOARD_OFFSET_Y + boardPixelSize,
            lineColor
        );
    }
}

void UI::drawStarPoints() {
    // 9 điểm sao trên bàn cờ 19x19: (4,4), (4,10), (4,16), (10,4), (10,10), (10,16), (16,4), (16,10), (16,16)
    int starPositions[9][2] = {
        {4, 4}, {4, 10}, {4, 16},
        {10, 4}, {10, 10}, {10, 16},
        {16, 4}, {16, 10}, {16, 16}
    };

    for (int i = 0; i < 9; i++) {
        int row = starPositions[i][0];
        int col = starPositions[i][1];
        Vector2 pos = boardToScreen(row, col);
        DrawCircle((int)pos.x, (int)pos.y, 4, starPointColor);
    }
}

void UI::drawCoordinates() {
    // Vẽ chữ A-T (cột)
    const char* colLabels = "ABCDEFGHJKLMNOPQRST";
    for (int col = 1; col <= BOARD_SIZE; col++) {
        Vector2 pos = boardToScreen(0, col);  // row = 0 (ngoài cùng bàn cờ)
        DrawText(
            TextFormat("%c", colLabels[col - 1]),
            (int)pos.x - 5,
            BOARD_OFFSET_Y - 30,
            20,
            BLACK
        );
    }

    // Vẽ số 1-19 cho bàn cờ
    for (int row = 1; row <= BOARD_SIZE; row++) {
        Vector2 pos = boardToScreen(row, 0);  // col = 0 (ngoài bàn cờ)
        DrawText(
            TextFormat("%d", BOARD_SIZE - row + 1),
            BOARD_OFFSET_X - 30,
            (int)pos.y - 10,
            20,
            BLACK
        );
    }
}

// ========== VẼ QUÂN CỜ ==========
void UI::drawStones(const Board& board) {
    for (int row = 1; row <= BOARD_SIZE; row++) {
        for (int col = 1; col <= BOARD_SIZE; col++) {
            char cell = board.getCell(row, col);

            if (cell == 'B' || cell == 'W') {
                Vector2 pos = boardToScreen(row, col);
                Color stoneColor = (cell == 'B') ? blackStoneColor : whiteStoneColor;

                // Vẽ quân cờ với hiệu ứng đổ bóng
                DrawCircle((int)pos.x + 2, (int)pos.y + 2, STONE_RADIUS, Color{0, 0, 0, 80});  // đổ bóng
                DrawCircle((int)pos.x, (int)pos.y, STONE_RADIUS, stoneColor);  // Quân cờ

                // Vẽ viền cho quân trắng
                if (cell == 'W') {
                    DrawCircleLines((int)pos.x, (int)pos.y, STONE_RADIUS, Color{100, 100, 100, 255});
                }
            }
        }
    }
}

// ========== VẼ HIỆU ỨNG HOVER ==========
void UI::drawHoverEffect(int row, int col) {
    if (isInsideBoard(row, col)) {
        Vector2 pos = boardToScreen(row, col);
        DrawCircle((int)pos.x, (int)pos.y, STONE_RADIUS, hoverColor);
    }
}

// ========== VẼ THÔNG TIN LƯỢT CHƠI ==========
void UI::drawPlayerTurn(char currentPlayer) {
    const char* turnText = (currentPlayer == 'B') ? "Black's Turn" : "White's Turn";
    Color textColor = (currentPlayer == 'B') ? BLACK : Color{100, 100, 100, 255};

    int textX = BOARD_OFFSET_X + (BOARD_SIZE - 1) * CELL_SIZE + 50;
    int textY = BOARD_OFFSET_Y + 400;

    DrawText(turnText, textX, textY, 30, textColor);

    // VẼ QUÂN CỜ MINH HỌA
    DrawCircle(textX + 70, textY + 60, 25, (currentPlayer == 'B') ? blackStoneColor : whiteStoneColor);
    if (currentPlayer == 'W') {
        DrawCircleLines(textX + 70, textY + 60, 25, Color{100, 100, 100, 255});
    }
}

// ========== VẼ ĐIỂM SỐ ==========
void UI::drawScore(int blackArea, int whiteArea, int blackCapture, int whiteCapture) {
    int textX = BOARD_OFFSET_X + (BOARD_SIZE - 1) * CELL_SIZE + 50;
    int textY = BOARD_OFFSET_Y + 500;

    DrawText("Score:", textX, textY, 25, BLACK);

    // Black score
    DrawText(TextFormat("Black: %d + %d", blackArea, blackCapture), textX, textY + 35, 20, BLACK);

    // White score (+ Komi 7.5)
    DrawText(TextFormat("White: %d + %d + 7", whiteArea, whiteCapture), textX, textY + 65, 20, Color{100, 100, 100, 255});
}

// ========== VẼ CÁC NÚT ==========
void UI::drawButtons() {
    // Undo button
    DrawRectangleRec(undoButton, Color{100, 150, 255, 255});
    DrawRectangleLinesEx(undoButton, 2, BLACK);
    DrawText("UNDO (U)", (int)undoButton.x + 30, (int)undoButton.y + 15, 20, WHITE);

    // Redo button
    DrawRectangleRec(redoButton, Color{100, 255, 150, 255});
    DrawRectangleLinesEx(redoButton, 2, BLACK);
    DrawText("REDO (R)", (int)redoButton.x + 30, (int)redoButton.y + 15, 20, BLACK);

    // Pass button
    DrawRectangleRec(passButton, Color{255, 200, 100, 255});
    DrawRectangleLinesEx(passButton, 2, BLACK);
    DrawText("PASS (P)", (int)passButton.x + 30, (int)passButton.y + 15, 20, BLACK);

    // New Game button
    DrawRectangleRec(newGameButton, Color{255, 100, 100, 255});
    DrawRectangleLinesEx(newGameButton, 2, BLACK);
    DrawText("NEW GAME", (int)newGameButton.x + 20, (int)newGameButton.y + 15, 20, WHITE);
}

// ========== VẼ MÀN HÌNH KẾT THÚC ==========
void UI::drawGameOver(char winner, int blackScore, int whiteScore) {
    // VẼ OVERLAY TỔNG THỂ
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 150});

    // VẼ BẢNG KẾT QUẢ
    int boxWidth = 500;
    int boxHeight = 350;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{240, 220, 180, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 5, BLACK);

    // TIÊU ĐỀ
    DrawText("GAME OVER", boxX + 120, boxY + 30, 40, BLACK);

    // KẾT QUẢ
    const char* winnerText = (winner == 'B') ? "BLACK WINS!" :
                            (winner == 'W') ? "WHITE WINS!" : "DRAW!";
    Color winnerColor = (winner == 'B') ? BLACK : Color{100, 100, 100, 255};
    DrawText(winnerText, boxX + 140, boxY + 100, 35, winnerColor);

    // ĐIỂM SỐ
    DrawText(TextFormat("Black: %d points", blackScore), boxX + 120, boxY + 160, 25, BLACK);
    DrawText(TextFormat("White: %d points", whiteScore), boxX + 120, boxY + 195, 25, Color{100, 100, 100, 255});

    // NÚT NEW GAME Ở GIỮA PHÍA DƯỚI
    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonX = boxX + (boxWidth - buttonWidth) / 2;
    int buttonY = boxY + boxHeight - buttonHeight - 30;

    Rectangle gameOverNewGameButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};

    DrawRectangleRec(gameOverNewGameButton, Color{100, 200, 100, 255});
    DrawRectangleLinesEx(gameOverNewGameButton, 3, BLACK);
    DrawText("NEW GAME", buttonX + 40, buttonY + 18, 25, WHITE);
}

// ========== CHUYỂN ĐỔI TỌA ĐỘ ==========
void UI::screenToBoard(Vector2 mousePos, int& row, int& col) {
    // chuyển từ pixel sang tọa độ bàn cờ
    int relX = (int)mousePos.x - BOARD_OFFSET_X;
    int relY = (int)mousePos.y - BOARD_OFFSET_Y;

    // Làm tròn đến ô gần nhất
    col = (relX + CELL_SIZE / 2) / CELL_SIZE + 1;
    row = (relY + CELL_SIZE / 2) / CELL_SIZE + 1;

    // Đảm bảo trong phạm vi 1-19
    if (col < 1) col = 1;
    if (col > BOARD_SIZE) col = BOARD_SIZE;
    if (row < 1) row = 1;
    if (row > BOARD_SIZE) row = BOARD_SIZE;
}

Vector2 UI::boardToScreen(int row, int col) {
    // Chuyển tọa độ từ bàn cờ sang pixel
    float x = BOARD_OFFSET_X + (col - 1) * CELL_SIZE;
    float y = BOARD_OFFSET_Y + (row - 1) * CELL_SIZE;
    return Vector2{x, y};
}

// ========== KIỂM TRA CLICK CHUỘT ==========
bool UI::isUndoButtonClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, undoButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isRedoButtonClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, redoButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isPassButtonClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, passButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isNewGameButtonClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, newGameButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isNewGameButtonGameOverClicked(Vector2 mousePos) {
    // Tính toán vị trí nút NEW GAME trong game over box (giống với drawGameOver)
    int boxWidth = 500;
    int boxHeight = 350;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonX = boxX + (boxWidth - buttonWidth) / 2;
    int buttonY = boxY + boxHeight - buttonHeight - 30;

    Rectangle gameOverNewGameButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};

    return CheckCollisionPointRec(mousePos, gameOverNewGameButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// ========== HELPER FUNCTIONS ==========
bool UI::isInsideBoard(int row, int col) {
    return (row >= 1 && row <= BOARD_SIZE && col >= 1 && col <= BOARD_SIZE);
}