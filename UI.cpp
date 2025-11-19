#include "UI.h"
#include <raylib.h>
#include <string>

// ========== CONSTRUCTOR/DESTRUCTOR ==========
UI::UI() {
    // Khßi t¡o màu s¯c
    boardColor = Color{220, 179, 92, 255};      // Màu g× cho bàn cÝ
    lineColor = BLACK;
    starPointColor = BLACK;
    blackStoneColor = Color{30, 30, 30, 255};
    whiteStoneColor = Color{245, 245, 245, 255};
    hoverColor = Color{100, 100, 255, 100};     // Màu xanh trong suÑt
}

UI::~UI() {
    // Destructor (không c§n làm gì)
}

// ========== KHÞI T O VÀ ÓNG RAYLIB ==========
void UI::init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Go Game - CÝ Vây");
    SetTargetFPS(60);
    initButtons();
}

void UI::cleanup() {
    CloseWindow();
}

// ========== KHÞI T O VÊ TRÍ CÁC NÚT ==========
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

// ========== B®T ¦U/K¾T THÚC V¼ ==========
void UI::beginDrawing() {
    BeginDrawing();
    ClearBackground(Color{180, 140, 80, 255});  // Màu nÁn nâu nh¡t
}

void UI::endDrawing() {
    EndDrawing();
}

bool UI::shouldClose() {
    return WindowShouldClose();
}

// ========== V¼ BÀN CÜ ==========
void UI::drawBoard() {
    // V½ nÁn bàn cÝ
    int boardPixelSize = (BOARD_SIZE - 1) * CELL_SIZE;
    DrawRectangle(
        BOARD_OFFSET_X - 20,
        BOARD_OFFSET_Y - 20,
        boardPixelSize + 40,
        boardPixelSize + 40,
        boardColor
    );

    // V½ grid lines
    drawGridLines();

    // V½ star points (9 iÃm ·c biÇt)
    drawStarPoints();

    // V½ tÍa Ù
    drawCoordinates();
}

void UI::drawGridLines() {
    int boardPixelSize = (BOARD_SIZE - 1) * CELL_SIZE;

    // V½ 19 °Ýng ngang
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

    // V½ 19 °Ýng dÍc
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
    // 9 iÃm sao trên bàn cÝ 19x19: (4,4), (4,10), (4,16), (10,4), (10,10), (10,16), (16,4), (16,10), (16,16)
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
    // V½ chï A-T (bÏ I) cho cÙt
    const char* colLabels = "ABCDEFGHJKLMNOPQRST";
    for (int col = 1; col <= BOARD_SIZE; col++) {
        Vector2 pos = boardToScreen(0, col);  // row = 0 (ngoài bàn cÝ)
        DrawText(
            TextFormat("%c", colLabels[col - 1]),
            (int)pos.x - 5,
            BOARD_OFFSET_Y - 30,
            20,
            BLACK
        );
    }

    // V½ sÑ 1-19 cho hàng
    for (int row = 1; row <= BOARD_SIZE; row++) {
        Vector2 pos = boardToScreen(row, 0);  // col = 0 (ngoài bàn cÝ)
        DrawText(
            TextFormat("%d", BOARD_SIZE - row + 1),
            BOARD_OFFSET_X - 30,
            (int)pos.y - 10,
            20,
            BLACK
        );
    }
}

// ========== V¼ QUÂN CÜ ==========
void UI::drawStones(const Board& board) {
    for (int row = 1; row <= BOARD_SIZE; row++) {
        for (int col = 1; col <= BOARD_SIZE; col++) {
            char cell = board.getCell(row, col);

            if (cell == 'B' || cell == 'W') {
                Vector2 pos = boardToScreen(row, col);
                Color stoneColor = (cell == 'B') ? blackStoneColor : whiteStoneColor;

                // V½ quân cÝ vÛi hiÇu éng bóng
                DrawCircle((int)pos.x + 2, (int)pos.y + 2, STONE_RADIUS, Color{0, 0, 0, 80});  // Bóng
                DrawCircle((int)pos.x, (int)pos.y, STONE_RADIUS, stoneColor);  // Quân cÝ

                // V½ viÁn cho quân tr¯ng
                if (cell == 'W') {
                    DrawCircleLines((int)pos.x, (int)pos.y, STONE_RADIUS, Color{100, 100, 100, 255});
                }
            }
        }
    }
}

// ========== V¼ HIÆU èNG HOVER ==========
void UI::drawHoverEffect(int row, int col) {
    if (isInsideBoard(row, col)) {
        Vector2 pos = boardToScreen(row, col);
        DrawCircle((int)pos.x, (int)pos.y, STONE_RADIUS, hoverColor);
    }
}

// ========== V¼ THÔNG TIN L¯âT CH I ==========
void UI::drawPlayerTurn(char currentPlayer) {
    const char* turnText = (currentPlayer == 'B') ? "Black's Turn" : "White's Turn";
    Color textColor = (currentPlayer == 'B') ? BLACK : Color{100, 100, 100, 255};

    int textX = BOARD_OFFSET_X + (BOARD_SIZE - 1) * CELL_SIZE + 50;
    int textY = BOARD_OFFSET_Y + 400;

    DrawText(turnText, textX, textY, 30, textColor);

    // V½ quân cÝ minh hÍa
    DrawCircle(textX + 70, textY + 60, 25, (currentPlayer == 'B') ? blackStoneColor : whiteStoneColor);
    if (currentPlayer == 'W') {
        DrawCircleLines(textX + 70, textY + 60, 25, Color{100, 100, 100, 255});
    }
}

// ========== V¼ IÂM SÐ ==========
void UI::drawScore(int blackArea, int whiteArea, int blackCapture, int whiteCapture) {
    int textX = BOARD_OFFSET_X + (BOARD_SIZE - 1) * CELL_SIZE + 50;
    int textY = BOARD_OFFSET_Y + 500;

    DrawText("Score:", textX, textY, 25, BLACK);

    // Black score
    DrawText(TextFormat("Black: %d + %d", blackArea, blackCapture), textX, textY + 35, 20, BLACK);

    // White score (+ Komi 7.5)
    DrawText(TextFormat("White: %d + %d + 7", whiteArea, whiteCapture), textX, textY + 65, 20, Color{100, 100, 100, 255});
}

// ========== V¼ CÁC NÚT ==========
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

// ========== V¼ MÀN HÌNH K¾T THÚC ==========
void UI::drawGameOver(char winner, int blackScore, int whiteScore) {
    // V½ overlay tÑi
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 150});

    // V½ b£ng k¿t qu£
    int boxWidth = 500;
    int boxHeight = 300;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{240, 220, 180, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 5, BLACK);

    // Tiêu Á
    DrawText("GAME OVER", boxX + 120, boxY + 30, 40, BLACK);

    // K¿t qu£
    const char* winnerText = (winner == 'B') ? "BLACK WINS!" :
                            (winner == 'W') ? "WHITE WINS!" : "DRAW!";
    Color winnerColor = (winner == 'B') ? BLACK : Color{100, 100, 100, 255};
    DrawText(winnerText, boxX + 140, boxY + 100, 35, winnerColor);

    // iÃm sÑ
    DrawText(TextFormat("Black: %d points", blackScore), boxX + 120, boxY + 160, 25, BLACK);
    DrawText(TextFormat("White: %d points", whiteScore), boxX + 120, boxY + 195, 25, Color{100, 100, 100, 255});

    // H°Ûng d«n
    DrawText("Click NEW GAME to play again", boxX + 80, boxY + 250, 20, Color{150, 150, 150, 255});
}

// ========== CHUYÂN ÔI TÌA Ø ==========
void UI::screenToBoard(Vector2 mousePos, int& row, int& col) {
    // ChuyÃn të pixel sang tÍa Ù bàn cÝ
    int relX = (int)mousePos.x - BOARD_OFFSET_X;
    int relY = (int)mousePos.y - BOARD_OFFSET_Y;

    // Làm tròn ¿n ô g§n nh¥t
    col = (relX + CELL_SIZE / 2) / CELL_SIZE + 1;
    row = (relY + CELL_SIZE / 2) / CELL_SIZE + 1;

    // £m b£o trong ph¡m vi 1-19
    if (col < 1) col = 1;
    if (col > BOARD_SIZE) col = BOARD_SIZE;
    if (row < 1) row = 1;
    if (row > BOARD_SIZE) row = BOARD_SIZE;
}

Vector2 UI::boardToScreen(int row, int col) {
    // ChuyÃn të tÍa Ù bàn cÝ sang pixel
    float x = BOARD_OFFSET_X + (col - 1) * CELL_SIZE;
    float y = BOARD_OFFSET_Y + (row - 1) * CELL_SIZE;
    return Vector2{x, y};
}

// ========== KIÂM TRA CLICK NÚT ==========
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

// ========== HELPER FUNCTIONS ==========
bool UI::isInsideBoard(int row, int col) {
    return (row >= 1 && row <= BOARD_SIZE && col >= 1 && col <= BOARD_SIZE);
}