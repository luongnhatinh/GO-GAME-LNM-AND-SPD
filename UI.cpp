#include "UI.h"
#include <raylib.h>
#include <string>
#include <unistd.h>   // Cho getcwd()
#include <stdio.h>    // Cho printf()
#include <filesystem>
#include <ctime>
#include <sys/stat.h>

// ========== CONSTRUCTOR/DESTRUCTOR ==========
UI::UI() {
    // Khởi tạo màu sắc
    boardColor = Color{220, 179, 92, 255};      // Màu gỗ cho bàn cờ
    lineColor = BLACK;
    starPointColor = BLACK;
    blackStoneColor = Color{30, 30, 30, 255};
    whiteStoneColor = Color{245, 245, 245, 255};
    hoverColor = Color{100, 100, 255, 100};     // Màu xanh hover

    // Khởi tạo menu state
    showAINotification = false;
    notificationFrameCounter = 0;
    showDifficultyMenu = false;

    // Khởi tạo save/load state
    showSavePopup = false;
    showLoadPopup = false;
    selectedSaveIndex = -1;
    saveGameNameLength = 0;
    saveGameNameBuffer[0] = '\0';
    scrollOffset = 0;
    saveRequested = false;
    loadRequested = false;
    loadPopupFrameCounter = 0;
    saveGameDirectory = detectSaveGameDirectory();  // Tự động phát hiện đường dẫn

    // Khởi tạo menu dialog state
    showMenuConfirmDialog = false;
    hasUnsavedChanges = true;  // Mặc định có thay đổi chưa lưu
    menuConfirmRequested = false;

    // Khởi tạo difficulty buttons về empty (chỉ tồn tại khi menu hiển thị)
    easyButton = Rectangle{0, 0, 0, 0};
    mediumButton = Rectangle{0, 0, 0, 0};
    hardButton = Rectangle{0, 0, 0, 0};
    backButton = Rectangle{0, 0, 0, 0};
}

UI::~UI() {
    // Destructor (không cần làm gì)
}

// ========== KHỞI TẠO VÀO WINDOW RAYLIB ==========
void UI::init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Go Game - Cờ Vây");
    SetTargetFPS(60);
    initButtons();
    initMenuButtons();

    // Debug: In working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }

    // ========== LOAD MENU TEXTURES ==========
    printf("Loading menu textures...\n");

    // Thử nhiều path khác nhau để load menu.png
    const char* menuPaths[] = {
        "media/menu.png",
        "../../media/menu.png",
        "../media/menu.png",
        "menu.png",
        "../../../media/menu.png"
    };

    menuTexture.id = 0;
    for (int i = 0; i < 5; i++) {
        printf("Trying path %d: %s\n", i+1, menuPaths[i]);
        menuTexture = LoadTexture(menuPaths[i]);
        if (menuTexture.id != 0) {
            printf("SUCCESS! Menu background loaded from: %s (%dx%d)\n",
                   menuPaths[i], menuTexture.width, menuTexture.height);
            break;
        }
    }

    if (menuTexture.id == 0) {
        printf("ERROR: Failed to load menu.png from all paths!\n");
    }

    // Load button Player
    const char* playerPaths[] = {
        "media/button_player.png",
        "../../media/button_player.png",
        "../media/button_player.png"
    };

    buttonPlayerTexture.id = 0;
    for (int i = 0; i < 3; i++) {
        buttonPlayerTexture = LoadTexture(playerPaths[i]);
        if (buttonPlayerTexture.id != 0) {
            printf("Button Player loaded from: %s (%dx%d)\n",
                   playerPaths[i], buttonPlayerTexture.width, buttonPlayerTexture.height);
            break;
        }
    }
    if (buttonPlayerTexture.id == 0) {
        printf("ERROR: Failed to load button_player.png\n");
    }

    // Load button AI
    const char* aiPaths[] = {
        "media/button_AI.png",
        "../../media/button_AI.png",
        "../media/button_AI.png"
    };

    buttonAITexture.id = 0;
    for (int i = 0; i < 3; i++) {
        buttonAITexture = LoadTexture(aiPaths[i]);
        if (buttonAITexture.id != 0) {
            printf("Button AI loaded from: %s (%dx%d)\n",
                   aiPaths[i], buttonAITexture.width, buttonAITexture.height);
            break;
        }
    }
    if (buttonAITexture.id == 0) {
        printf("ERROR: Failed to load button_AI.png\n");
    }

    // Load button Setting
    const char* settingPaths[] = {
        "media/button_setting.png",
        "../../media/button_setting.png",
        "../media/button_setting.png"
    };

    buttonSettingTexture.id = 0;
    for (int i = 0; i < 3; i++) {
        buttonSettingTexture = LoadTexture(settingPaths[i]);
        if (buttonSettingTexture.id != 0) {
            printf("Button Setting loaded from: %s (%dx%d)\n",
                   settingPaths[i], buttonSettingTexture.width, buttonSettingTexture.height);
            break;
        }
    }
    if (buttonSettingTexture.id == 0) {
        printf("ERROR: Failed to load button_setting.png\n");
    }

    // Load Menu_Blanks background
    const char* menuBlanksPaths[] = {
        "media/Menu_Blanks.png",
        "../../media/Menu_Blanks.png",
        "../media/Menu_Blanks.png"
    };

    menuBlanksTexture.id = 0;
    for (int i = 0; i < 3; i++) {
        menuBlanksTexture = LoadTexture(menuBlanksPaths[i]);
        if (menuBlanksTexture.id != 0) {
            printf("Menu Blanks loaded from: %s (%dx%d)\n",
                   menuBlanksPaths[i], menuBlanksTexture.width, menuBlanksTexture.height);
            break;
        }
    }
    if (menuBlanksTexture.id == 0) {
        printf("ERROR: Failed to load Menu_Blanks.png\n");
    }

    // Load button_blanks
    const char* buttonBlanksPaths[] = {
        "media/button_blanks.png",
        "../../media/button_blanks.png",
        "../media/button_blanks.png"
    };

    buttonBlanksTexture.id = 0;
    for (int i = 0; i < 3; i++) {
        buttonBlanksTexture = LoadTexture(buttonBlanksPaths[i]);
        if (buttonBlanksTexture.id != 0) {
            printf("Button Blanks loaded from: %s (%dx%d)\n",
                   buttonBlanksPaths[i], buttonBlanksTexture.width, buttonBlanksTexture.height);
            break;
        }
    }
    if (buttonBlanksTexture.id == 0) {
        printf("ERROR: Failed to load button_blanks.png\n");
    }

    // Load button_back
    const char* buttonBackPaths[] = {
        "media/button_back.png",
        "../../media/button_back.png",
        "../media/button_back.png"
    };

    buttonBackTexture.id = 0;
    for (int i = 0; i < 3; i++) {
        buttonBackTexture = LoadTexture(buttonBackPaths[i]);
        if (buttonBackTexture.id != 0) {
            printf("Button Back loaded from: %s (%dx%d)\n",
                   buttonBackPaths[i], buttonBackTexture.width, buttonBackTexture.height);
            break;
        }
    }
    if (buttonBackTexture.id == 0) {
        printf("ERROR: Failed to load button_back.png\n");
    }
}

void UI::cleanup() {
    UnloadTexture(menuTexture);
    UnloadTexture(menuBlanksTexture);
    UnloadTexture(buttonPlayerTexture);
    UnloadTexture(buttonAITexture);
    UnloadTexture(buttonSettingTexture);
    UnloadTexture(buttonBlanksTexture);
    UnloadTexture(buttonBackTexture);
    CloseWindow();
}

// ========== KHỞI TẠO VỊ TRÍ CÁC NÚT ==========
void UI::initButtons() {
    int buttonX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 50;
    int buttonY = BOARD_OFFSET_Y;
    int buttonWidth = 150;
    int buttonHeight = 50;
    int buttonSpacing = 60;  // Khoảng cách dọc giữa các buttons
    int columnSpacing = 20;  // Khoảng cách ngang giữa 2 cột

    // ========== CỘT 1 (bên trái) ==========
    undoButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    redoButton = {(float)buttonX, (float)(buttonY + buttonSpacing), (float)buttonWidth, (float)buttonHeight};
    passButton = {(float)buttonX, (float)(buttonY + buttonSpacing * 2), (float)buttonWidth, (float)buttonHeight};
    newGameButton = {(float)buttonX, (float)(buttonY + buttonSpacing * 3), (float)buttonWidth, (float)buttonHeight};

    // ========== CỘT 2 (bên phải) ==========
    int buttonX2 = buttonX + buttonWidth + columnSpacing;
    saveGameButton = {(float)buttonX2, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    loadGameButton = {(float)buttonX2, (float)(buttonY + buttonSpacing), (float)buttonWidth, (float)buttonHeight};
    menuButton = {(float)buttonX2, (float)(buttonY + buttonSpacing * 2), (float)buttonWidth, (float)buttonHeight};
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

void UI::resetToMainMenu() {
    // Reset về menu chính, không phải difficulty menu
    showDifficultyMenu = false;
    showAINotification = false;
    notificationFrameCounter = 0;

    // XÓA các difficulty buttons (reset về empty rectangle)
    // Chỉ vẽ lại khi difficulty menu được hiển thị
    easyButton = Rectangle{0, 0, 0, 0};
    mediumButton = Rectangle{0, 0, 0, 0};
    hardButton = Rectangle{0, 0, 0, 0};
    backButton = Rectangle{0, 0, 0, 0};
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
    int textY = BOARD_OFFSET_Y + 300;  // Khôi phục vị trí (sau buttons 2 cột)

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
    int textY = BOARD_OFFSET_Y + 400;  // Khôi phục vị trí (sau Player Turn)

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

    // Save Game button
    DrawRectangleRec(saveGameButton, Color{100, 200, 100, 255});
    DrawRectangleLinesEx(saveGameButton, 2, BLACK);
    DrawText("SAVE", (int)saveGameButton.x + 45, (int)saveGameButton.y + 15, 20, WHITE);

    // Load Game button
    DrawRectangleRec(loadGameButton, Color{200, 100, 200, 255});
    DrawRectangleLinesEx(loadGameButton, 2, BLACK);
    DrawText("LOAD", (int)loadGameButton.x + 45, (int)loadGameButton.y + 15, 20, WHITE);

    // Menu button
    DrawRectangleRec(menuButton, Color{150, 100, 200, 255});
    DrawRectangleLinesEx(menuButton, 2, BLACK);
    DrawText("MENU", (int)menuButton.x + 45, (int)menuButton.y + 15, 20, WHITE);
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

    // NÚT BACK TO MENU Ở GIỮA PHÍA DƯỚI
    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonX = boxX + (boxWidth - buttonWidth) / 2;
    int buttonY = boxY + boxHeight - buttonHeight - 30;

    Rectangle gameOverNewGameButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};

    DrawRectangleRec(gameOverNewGameButton, Color{100, 150, 255, 255});
    DrawRectangleLinesEx(gameOverNewGameButton, 3, BLACK);
    DrawText("MENU", buttonX + 65, buttonY + 18, 25, WHITE);
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

// ========== KHỞI TẠO NÚT MENU ==========
void UI::initMenuButtons() {
    // Vị trí các nút sẽ được tính khi vẽ dựa trên kích thước texture
    // Placeholder - sẽ được update trong drawMenu()
}

// ========== VẼ MÀN HÌNH MENU ==========
void UI::drawMenu() {
    // Nếu đang ở menu chọn độ khó, vẽ menu đó thay vì menu chính
    if (showDifficultyMenu) {
        drawDifficultyMenu();
        return;
    }

    // Kiểm tra xem texture có được load không
    if (menuTexture.id == 0) {
        ClearBackground(Color{180, 140, 80, 255});
        DrawText("ERROR: menu.png not found!", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 50, 30, RED);
        return;
    }

    // Vẽ ảnh menu background FULL SCREEN
    DrawTexturePro(
        menuTexture,
        Rectangle{0, 0, (float)menuTexture.width, (float)menuTexture.height},
        Rectangle{0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
        Vector2{0, 0},
        0.0f,
        WHITE
    );

    // ========== VẼ BUTTON PLAYER VS PLAYER ==========
    if (buttonPlayerTexture.id != 0) {
        // Tính vị trí button (ở giữa màn hình, phía dưới, bên trái)
        int buttonWidth = 400;  // Kích thước mong muốn
        int buttonHeight = 100;
        int spacing = 50;       // Khoảng cách giữa các button

        int totalWidth = buttonWidth * 2 + spacing;  // Chỉ tính cho 2 buttons chính
        int startX = (SCREEN_WIDTH - totalWidth) / 2;
        int startY = SCREEN_HEIGHT - buttonHeight - 100;  // 100px từ dưới lên

        // Vị trí button Player
        playerVsPlayerButton = Rectangle{
            (float)startX,
            (float)startY,
            (float)buttonWidth,
            (float)buttonHeight
        };

        // Vẽ button Player
        DrawTexturePro(
            buttonPlayerTexture,
            Rectangle{0, 0, (float)buttonPlayerTexture.width, (float)buttonPlayerTexture.height},
            playerVsPlayerButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );
    }

    // ========== VẼ BUTTON PLAYER VS AI ==========
    if (buttonAITexture.id != 0) {
        // Tính vị trí button AI (bên phải button Player)
        int buttonWidth = 400;
        int buttonHeight = 100;
        int spacing = 50;

        int totalWidth = buttonWidth * 2 + spacing;  // Chỉ tính cho 2 buttons chính
        int startX = (SCREEN_WIDTH - totalWidth) / 2;
        int startY = SCREEN_HEIGHT - buttonHeight - 100;

        // Vị trí button AI
        playerVsAIButton = Rectangle{
            (float)(startX + buttonWidth + spacing),
            (float)startY,
            (float)buttonWidth,
            (float)buttonHeight
        };

        // Vẽ button AI
        DrawTexturePro(
            buttonAITexture,
            Rectangle{0, 0, (float)buttonAITexture.width, (float)buttonAITexture.height},
            playerVsAIButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );
    }

    // ========== VẼ BUTTON SETTINGS ==========
    if (buttonSettingTexture.id != 0) {
        // Button Settings nhỏ gọn, đặt bên phải button AI
        int buttonWidth = 400;
        int buttonHeight = 100;
        int spacing = 50;

        // Kích thước button Settings (nhỏ hơn, giữ tỉ lệ gốc)
        int settingSize = 100;  // Button vuông 100x100

        int totalWidth = buttonWidth * 2 + spacing;
        int startX = (SCREEN_WIDTH - totalWidth) / 2;
        int startY = SCREEN_HEIGHT - buttonHeight - 100;

        // Vị trí button Settings: bên phải button AI, với khoảng cách nhỏ
        int settingX = startX + buttonWidth * 2 + spacing + 30;  // 30px từ button AI
        int settingY = startY;  // Cùng hàng với các button khác

        playerVsSettingButton = Rectangle{
            (float)settingX,
            (float)settingY,
            (float)settingSize,
            (float)settingSize
        };

        // Vẽ button Settings với tỉ lệ gốc
        DrawTexturePro(
            buttonSettingTexture,
            Rectangle{0, 0, (float)buttonSettingTexture.width, (float)buttonSettingTexture.height},
            playerVsSettingButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );
    }

    // Vẽ thông báo AI nếu cần
    if (showAINotification) {
        drawAINotification();
        // Xử lý đóng notification sẽ được handle trong drawAINotification()
    }
}

// ========== VẼ MENU CHỌN ĐỘ KHÓ ==========
void UI::drawDifficultyMenu() {
    // Vẽ background Menu_Blanks
    if (menuBlanksTexture.id != 0) {
        DrawTexturePro(
            menuBlanksTexture,
            Rectangle{0, 0, (float)menuBlanksTexture.width, (float)menuBlanksTexture.height},
            Rectangle{0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
            Vector2{0, 0},
            0.0f,
            WHITE
        );
    } else {
        ClearBackground(Color{180, 140, 80, 255});
    }

    // ========== VẼ NÚT BACK ==========
    // Padding trái 20% (280px), padding trên 30% (270px)
    int backButtonX = (int)(SCREEN_WIDTH * 0.1);  // 20% từ trái = 280px
    int backButtonY = (int)(SCREEN_HEIGHT * 0.3); // 30% từ trên = 270px
    int backButtonSize = 100;

    if (buttonBackTexture.id != 0) {
        backButton = Rectangle{(float)backButtonX, (float)backButtonY, (float)backButtonSize, (float)backButtonSize};

        DrawTexturePro(
            buttonBackTexture,
            Rectangle{0, 0, (float)buttonBackTexture.width, (float)buttonBackTexture.height},
            backButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );
    }

    // Vẽ tiêu đề "CHOOSE DIFFICULTY" - chỉ hiển thị trong 60% phần dưới
    // 60% phần dưới bắt đầu từ Y = 360px (40% của 900)
    const char* titleText = "CHOOSE DIFFICULTY";
    int titleFontSize = 60;
    int titleWidth = MeasureText(titleText, titleFontSize);
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;
    int titleY = 380;  // Đặt trong vùng 60% dưới

    // Vẽ chữ dày hơn bằng cách tăng outline thickness và vẽ nhiều lớp
    drawTextWithOutline(titleText, titleX, titleY, titleFontSize, WHITE, BLACK, 4);

    // Thiết lập vị trí và kích thước cho 3 buttons
    // Giảm 10% size: 400x100 → 360x90
    int buttonWidth = 360;
    int buttonHeight = 90;
    int buttonSpacing = 40;  // Điều chỉnh spacing
    int startY = 480;  // Bắt đầu buttons
    // Bottom padding 10%: không vượt quá Y = 810 (900 - 90)

    // ========== VẼ 3 BUTTONS ==========
    if (buttonBlanksTexture.id != 0) {
        // EASY Button
        int easyX = (SCREEN_WIDTH - buttonWidth) / 2;
        easyButton = Rectangle{(float)easyX, (float)startY, (float)buttonWidth, (float)buttonHeight};

        DrawTexturePro(
            buttonBlanksTexture,
            Rectangle{0, 0, (float)buttonBlanksTexture.width, (float)buttonBlanksTexture.height},
            easyButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );

        // Text "EASY" trên button (điều chỉnh vị trí cho button nhỏ hơn)
        const char* easyText = "EASY";
        int easyTextWidth = MeasureText(easyText, 40);
        int easyTextX = easyX + (buttonWidth - easyTextWidth) / 2;
        int easyTextY = (int)easyButton.y + 25;  // Điều chỉnh cho height 90px
        drawTextWithOutline(easyText, easyTextX, easyTextY, 40, WHITE, BLACK, 2);

        // MEDIUM Button
        int mediumX = (SCREEN_WIDTH - buttonWidth) / 2;
        int mediumY = startY + buttonHeight + buttonSpacing;
        mediumButton = Rectangle{(float)mediumX, (float)mediumY, (float)buttonWidth, (float)buttonHeight};

        DrawTexturePro(
            buttonBlanksTexture,
            Rectangle{0, 0, (float)buttonBlanksTexture.width, (float)buttonBlanksTexture.height},
            mediumButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );

        // Text "MEDIUM" trên button (điều chỉnh vị trí cho button nhỏ hơn)
        const char* mediumText = "MEDIUM";
        int mediumTextWidth = MeasureText(mediumText, 40);
        int mediumTextX = mediumX + (buttonWidth - mediumTextWidth) / 2;
        int mediumTextY = (int)mediumButton.y + 25;  // Điều chỉnh cho height 90px
        drawTextWithOutline(mediumText, mediumTextX, mediumTextY, 40, WHITE, BLACK, 2);

        // HARD Button
        int hardX = (SCREEN_WIDTH - buttonWidth) / 2;
        int hardY = mediumY + buttonHeight + buttonSpacing;
        hardButton = Rectangle{(float)hardX, (float)hardY, (float)buttonWidth, (float)buttonHeight};

        DrawTexturePro(
            buttonBlanksTexture,
            Rectangle{0, 0, (float)buttonBlanksTexture.width, (float)buttonBlanksTexture.height},
            hardButton,
            Vector2{0, 0},
            0.0f,
            WHITE
        );

        // Text "HARD" trên button (điều chỉnh vị trí cho button nhỏ hơn)
        const char* hardText = "HARD";
        int hardTextWidth = MeasureText(hardText, 40);
        int hardTextX = hardX + (buttonWidth - hardTextWidth) / 2;
        int hardTextY = (int)hardButton.y + 25;  // Điều chỉnh cho height 90px
        drawTextWithOutline(hardText, hardTextX, hardTextY, 40, WHITE, BLACK, 2);
    }
}

// ========== VẼ THÔNG BÁO AI ==========
void UI::drawAINotification() {
    // Tăng frame counter
    notificationFrameCounter++;

    // Vẽ overlay tối (full screen)
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 180});

    // Vẽ box thông báo
    int boxWidth = 600;
    int boxHeight = 200;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{240, 220, 180, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 5, BLACK);

    // ========== NÚT X Ở GÓC TRÊN PHẢI ==========
    int closeButtonSize = 40;
    int closeButtonX = boxX + boxWidth - closeButtonSize - 10;
    int closeButtonY = boxY + 10;

    Rectangle closeButton = {(float)closeButtonX, (float)closeButtonY, (float)closeButtonSize, (float)closeButtonSize};

    // Vẽ nút X
    DrawRectangleRec(closeButton, Color{200, 80, 80, 255});
    DrawRectangleLinesEx(closeButton, 2, BLACK);
    DrawText("X", closeButtonX + 12, closeButtonY + 8, 25, WHITE);

    // Title
    DrawText("NOTIFICATION", boxX + 180, boxY + 30, 30, BLACK);

    // Content
    DrawText("Feature under development.", boxX + 140, boxY + 100, 20, BLACK);
    DrawText("Please come back later!", boxX + 160, boxY + 130, 20, BLACK);

    // ========== XỬ LÝ ĐÓNG THÔNG BÁO ==========
    // CHỈ cho phép đóng SAU KHI đã hiển thị đủ 60 frames (~1 giây)
    const int DELAY_FRAMES = 60;  // 1 giây với 60 FPS

    if (notificationFrameCounter > DELAY_FRAMES && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        // Check click vào nút X
        if (CheckCollisionPointRec(mousePos, closeButton)) {
            showAINotification = false;
            notificationFrameCounter = 0;  // Reset counter
            return;
        }

        // Check click vào vùng ngoài box (overlay)
        Rectangle notificationBox = {(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight};
        if (!CheckCollisionPointRec(mousePos, notificationBox)) {
            // Click vào vùng ngoài box → đóng
            showAINotification = false;
            notificationFrameCounter = 0;  // Reset counter
        }
    }
}

// ========== VẼ DIALOG XÁC NHẬN QUAY VỀ MENU ==========
void UI::drawMenuConfirmDialog() {
    // Vẽ overlay tối (full screen)
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 180});

    // Vẽ box dialog
    int boxWidth = 600;
    int boxHeight = 300;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{240, 220, 180, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 5, BLACK);

    // Title và content tùy thuộc vào trạng thái lưu game
    if (hasUnsavedChanges) {
        // Trường hợp chưa lưu game
        drawTextWithOutline("UNSAVED PROGRESS", boxX + 140, boxY + 30, 30, BLACK, WHITE, 2);

        DrawText("You haven't saved this game yet.", boxX + 120, boxY + 100, 20, BLACK);
        DrawText("All progress will be lost!", boxX + 150, boxY + 130, 20, Color{200, 0, 0, 255});
    } else {
        // Trường hợp đã lưu game
        drawTextWithOutline("RETURN TO MAIN MENU", boxX + 110, boxY + 30, 30, BLACK, WHITE, 2);

        DrawText("Are you sure you want to return", boxX + 120, boxY + 100, 20, BLACK);
        DrawText("to the main menu?", boxX + 180, boxY + 130, 20, BLACK);
    }

    // Vẽ 2 nút: MAIN MENU và CLOSE
    int buttonWidth = 200;
    int buttonHeight = 60;
    int buttonSpacing = 50;
    int totalButtonWidth = buttonWidth * 2 + buttonSpacing;
    int buttonStartX = boxX + (boxWidth - totalButtonWidth) / 2;
    int buttonY = boxY + boxHeight - buttonHeight - 30;

    // Nút "MAIN MENU" (màu đỏ/cam)
    confirmMainMenuButton = {(float)buttonStartX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    DrawRectangleRec(confirmMainMenuButton, Color{255, 100, 100, 255});
    DrawRectangleLinesEx(confirmMainMenuButton, 3, BLACK);
    DrawText("MAIN MENU", buttonStartX + 35, buttonY + 18, 20, WHITE);

    // Nút "CLOSE" (màu xanh)
    closeDialogButton = {(float)(buttonStartX + buttonWidth + buttonSpacing), (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    DrawRectangleRec(closeDialogButton, Color{100, 150, 255, 255});
    DrawRectangleLinesEx(closeDialogButton, 3, BLACK);
    DrawText("CLOSE", closeDialogButton.x + 65, buttonY + 18, 20, WHITE);
}

// ========== KIỂM TRA CLICK NÚT MENU ==========
bool UI::isPlayerVsPlayerClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, playerVsPlayerButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isPlayerVsAIClicked(Vector2 mousePos) {
    // Chuyển sang menu chọn độ khó khi click vào button AI
    if (!showDifficultyMenu && CheckCollisionPointRec(mousePos, playerVsAIButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showDifficultyMenu = true;
        return true;
    }
    return false;
}

bool UI::isPlayerVsSettingClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, playerVsSettingButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isEasyButtonClicked(Vector2 mousePos) {
    // Button chỉ tồn tại khi difficulty menu được vẽ
    return CheckCollisionPointRec(mousePos, easyButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isMediumButtonClicked(Vector2 mousePos) {
    // Button chỉ tồn tại khi difficulty menu được vẽ
    return CheckCollisionPointRec(mousePos, mediumButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isHardButtonClicked(Vector2 mousePos) {
    // Button chỉ tồn tại khi difficulty menu được vẽ
    return CheckCollisionPointRec(mousePos, hardButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isBackButtonClicked(Vector2 mousePos) {
    // Button chỉ tồn tại khi difficulty menu được vẽ
    return CheckCollisionPointRec(mousePos, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isSaveGameButtonClicked(Vector2 mousePos) {
    if (CheckCollisionPointRec(mousePos, saveGameButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showSavePopup = true;
        saveGameNameLength = 0;
        saveGameNameBuffer[0] = '\0';
        return true;
    }
    return false;
}

bool UI::isLoadGameButtonClicked(Vector2 mousePos) {
    if (CheckCollisionPointRec(mousePos, loadGameButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showLoadPopup = true;
        loadSavedGamesList();  // Load danh sách game khi mở popup
        selectedSaveIndex = -1;
        loadPopupFrameCounter = 0;  // Reset frame counter
        return true;
    }
    return false;
}

bool UI::isMenuButtonClicked(Vector2 mousePos) {
    if (CheckCollisionPointRec(mousePos, menuButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showMenuConfirmDialog = true;
        return true;
    }
    return false;
}

bool UI::isConfirmMainMenuClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, confirmMainMenuButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isCloseDialogClicked(Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, closeDialogButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// ========== HELPER FUNCTIONS ==========
bool UI::isInsideBoard(int row, int col) {
    return (row >= 1 && row <= BOARD_SIZE && col >= 1 && col <= BOARD_SIZE);
}

// Vẽ text với viền (outline)
void UI::drawTextWithOutline(const char* text, int x, int y, int fontSize, Color textColor, Color outlineColor, int outlineThickness) {
    // Vẽ viền (outline) bằng cách vẽ text ở các vị trí xung quanh
    for (int offsetX = -outlineThickness; offsetX <= outlineThickness; offsetX++) {
        for (int offsetY = -outlineThickness; offsetY <= outlineThickness; offsetY++) {
            if (offsetX != 0 || offsetY != 0) {  // Không vẽ ở vị trí trung tâm
                DrawText(text, x + offsetX, y + offsetY, fontSize, outlineColor);
            }
        }
    }

    // Vẽ text chính ở giữa
    DrawText(text, x, y, fontSize, textColor);
}

std::string UI::detectSaveGameDirectory() {
    namespace fs = std::filesystem;

    // Try current directory first (Code::Blocks case - running from root)
    if (fs::exists("save_game")) {
        return "save_game";
    }

    // Try parent directory (VS Code case - running from build/)
    if (fs::exists("../save_game")) {
        return "../save_game";
    }

    // If neither exists, default to current directory
    // (will create folder when needed)
    return "save_game";
}

// ========== SAVE/LOAD POPUP IMPLEMENTATIONS ==========


void UI::drawSaveGamePopup() {
    // Overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 180});

    // Popup box
    int boxWidth = 500;
    int boxHeight = 250;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{240, 220, 180, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 5, BLACK);

    // Title
    DrawText("SAVE GAME", boxX + 160, boxY + 30, 30, BLACK);

    // Instruction
    DrawText("Enter save name:", boxX + 50, boxY + 80, 20, BLACK);

    // Text input box
    Rectangle textBox = {(float)(boxX + 50), (float)(boxY + 110), (float)(boxWidth - 100), 40};
    DrawRectangleRec(textBox, WHITE);
    DrawRectangleLinesEx(textBox, 2, BLACK);
    DrawText(saveGameNameBuffer, (int)textBox.x + 10, (int)textBox.y + 10, 20, BLACK);

    // Blinking cursor
    if ((GetTime() * 2) - (int)(GetTime() * 2) < 0.5) {
        int cursorX = (int)textBox.x + 10 + MeasureText(saveGameNameBuffer, 20);
        DrawText("_", cursorX, (int)textBox.y + 10, 20, BLACK);
    }

    // SAVE button
    Rectangle saveButton = {(float)(boxX + 150), (float)(boxY + 180), 100, 40};
    DrawRectangleRec(saveButton, Color{100, 200, 100, 255});
    DrawRectangleLinesEx(saveButton, 2, BLACK);
    DrawText("SAVE", (int)saveButton.x + 25, (int)saveButton.y + 10, 20, WHITE);

    // CANCEL button
    Rectangle cancelButton = {(float)(boxX + 270), (float)(boxY + 180), 100, 40};
    DrawRectangleRec(cancelButton, Color{200, 100, 100, 255});
    DrawRectangleLinesEx(cancelButton, 2, BLACK);
    DrawText("CANCEL", (int)cancelButton.x + 15, (int)cancelButton.y + 10, 20, WHITE);

    // Handle mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        // SAVE button clicked
        if (CheckCollisionPointRec(mousePos, saveButton) && saveGameNameLength > 0) {
            printf("SAVE button clicked! Name: '%s', Length: %d\n", saveGameNameBuffer, saveGameNameLength);
            saveRequested = true;  // Signal that save was requested
            showSavePopup = false;
            printf("saveRequested set to TRUE, popup closed\n");
        }

        // CANCEL button clicked
        if (CheckCollisionPointRec(mousePos, cancelButton)) {
            printf("CANCEL button clicked\n");
            showSavePopup = false;
        }
    }
}

void UI::drawLoadGamePopup() {
    // Increment frame counter
    loadPopupFrameCounter++;

    // Overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{0, 0, 0, 180});

    // Popup box (larger for list)
    int boxWidth = 700;
    int boxHeight = 500;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{240, 220, 180, 255});
    DrawRectangleLinesEx(Rectangle{(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 5, BLACK);

    // Title
    DrawText("LOAD GAME", boxX + 260, boxY + 30, 30, BLACK);

    // Close X button
    int closeButtonSize = 40;
    int closeButtonX = boxX + boxWidth - closeButtonSize - 10;
    int closeButtonY = boxY + 10;
    Rectangle closeButton = {(float)closeButtonX, (float)closeButtonY, (float)closeButtonSize, (float)closeButtonSize};
    DrawRectangleRec(closeButton, Color{200, 80, 80, 255});
    DrawRectangleLinesEx(closeButton, 2, BLACK);
    DrawText("X", closeButtonX + 12, closeButtonY + 8, 25, WHITE);

    // List area
    int listY = boxY + 80;
    int itemHeight = 60;
    int maxVisible = 6;

    if (savedGamesList.empty()) {
        DrawText("No saved games found.", boxX + 220, listY + 100, 20, BLACK);
    } else {
        // Draw saved games list
        for (int i = 0; i < (int)savedGamesList.size() && i < maxVisible; i++) {
            int itemY = listY + i * itemHeight;
            Rectangle itemRect = {(float)(boxX + 30), (float)itemY, (float)(boxWidth - 60), (float)(itemHeight - 10)};

            // Highlight if selected
            Color bgColor = (i == selectedSaveIndex) ? Color{200, 200, 255, 255} : Color{255, 255, 255, 255};
            DrawRectangleRec(itemRect, bgColor);
            DrawRectangleLinesEx(itemRect, 2, BLACK);

            // Draw game name
            DrawText(savedGamesList[i].c_str(), (int)itemRect.x + 10, (int)itemRect.y + 8, 20, BLACK);

            // Draw timestamp
            DrawText(savedGamesTime[i].c_str(), (int)itemRect.x + 10, (int)itemRect.y + 32, 16, Color{100, 100, 100, 255});
        }
    }

    // ========== XỬ LÝ CLICK CHỈ SAU KHI ĐỦ DELAY ==========
    const int DELAY_FRAMES = 30;  // 0.5 giây với 60 FPS

    if (loadPopupFrameCounter > DELAY_FRAMES && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        // Check click vào nút X
        if (CheckCollisionPointRec(mousePos, closeButton)) {
            showLoadPopup = false;
            selectedSaveIndex = -1;
            loadPopupFrameCounter = 0;
            return;
        }

        // Check click vào saved game items
        if (!savedGamesList.empty()) {
            for (int i = 0; i < (int)savedGamesList.size() && i < maxVisible; i++) {
                int itemY = listY + i * itemHeight;
                Rectangle itemRect = {(float)(boxX + 30), (float)itemY, (float)(boxWidth - 60), (float)(itemHeight - 10)};

                if (CheckCollisionPointRec(mousePos, itemRect)) {
                    selectedSaveIndex = i;
                    loadRequested = true;  // Signal that load was requested
                    showLoadPopup = false;
                    loadPopupFrameCounter = 0;
                    printf("Load game requested: index %d\n", i);
                    return;
                }
            }
        }
    }
}

void UI::loadSavedGamesList() {
    savedGamesList.clear();
    savedGamesTime.clear();

    namespace fs = std::filesystem;

    // Create save_game directory if it doesn't exist
    if (!fs::exists(saveGameDirectory)) {
        fs::create_directory(saveGameDirectory);
        return;
    }

    // List all .gogame files
    for (const auto& entry : fs::directory_iterator(saveGameDirectory)) {
        if (entry.path().extension() == ".gogame") {
            std::string filename = entry.path().filename().string();

            // Remove .gogame extension for display
            std::string displayName = filename.substr(0, filename.length() - 7);
            savedGamesList.push_back(displayName);

            // Get file modification time
            struct stat fileInfo;
            if (stat(entry.path().string().c_str(), &fileInfo) == 0) {
                char timeStr[100];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&fileInfo.st_mtime));
                savedGamesTime.push_back(std::string(timeStr));
            } else {
                savedGamesTime.push_back("Unknown time");
            }
        }
    }
}

std::string UI::getSelectedSaveFile() {
    if (selectedSaveIndex >= 0 && selectedSaveIndex < (int)savedGamesList.size()) {
        return saveGameDirectory + "/" + savedGamesList[selectedSaveIndex] + ".gogame";
    }
    return "";
}

std::string UI::getSaveGameName() {
    if (saveGameNameLength > 0) {
        return saveGameDirectory + "/" + std::string(saveGameNameBuffer) + ".gogame";
    }
    return "";
}

void UI::handleTextInput(int key) {
    if (!showSavePopup) return;

    if (key == KEY_BACKSPACE && saveGameNameLength > 0) {
        saveGameNameLength--;
        saveGameNameBuffer[saveGameNameLength] = '\0';
    } else if (key >= 32 && key <= 125 && saveGameNameLength < 99) {
        // Printable characters
        saveGameNameBuffer[saveGameNameLength] = (char)key;
        saveGameNameLength++;
        saveGameNameBuffer[saveGameNameLength] = '\0';
    }
}