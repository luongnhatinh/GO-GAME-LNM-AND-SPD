#include "Game.h"
#include "board.h"
#include "UI.h"
#include <raylib.h>
#include <iostream>

// GAME LOOP CHÍNH
void Game::run() {
    init();

    //chạy cho đến khi đóng window
    while (!ui.shouldClose()) {
        handleInput();
        update();
        render();
    }
    cleanup();
}

//KHỞI TẠO
void Game::init() {
    // tạo UI
    ui.init();
    currentState = MENU;

    // ========== KHỞI TẠO MUSIC SYSTEM ==========
    InitAudioDevice();

    // ========== LOAD MENU BACKGROUND MUSIC ==========
    const char* menuMusicPaths[] = {
        "media/menu_background_music.mp3",
        "../../media/menu_background_music.mp3",
        "../media/menu_background_music.mp3"
    };

    menuBackgroundMusic = {0};
    for (int i = 0; i < 3; i++) {
        menuBackgroundMusic = LoadMusicStream(menuMusicPaths[i]);
        if (menuBackgroundMusic.stream.buffer != NULL) {
            printf("Loaded menu background music from: %s\n", menuMusicPaths[i]);
            break;
        }
    }

    if (menuBackgroundMusic.stream.buffer == NULL) {
        printf("WARNING: Failed to load menu_background_music.mp3\n");
    } else {
        menuBackgroundMusic.looping = true;
        PlayMusicStream(menuBackgroundMusic);
        printf("Playing menu background music\n");
    }

    // ========== LOAD 3 IN-GAME SONGS ==========
    const char* musicPaths[][3] = {
        {"media/Song1.mp3", "../../media/Song1.mp3", "../media/Song1.mp3"},
        {"media/Song2.mp3", "../../media/Song2.mp3", "../media/Song2.mp3"},
        {"media/Song3.mp3", "../../media/Song3.mp3", "../media/Song3.mp3"}
    };

    for (int i = 0; i < 3; i++) {
        Music loadedMusic = {0};
        for (int j = 0; j < 3; j++) {
            loadedMusic = LoadMusicStream(musicPaths[i][j]);
            if (loadedMusic.stream.buffer != NULL) {
                printf("Loaded %s from path: %s\n",
                       (i == 0 ? "Song1" : i == 1 ? "Song2" : "Song3"),
                       musicPaths[i][j]);
                break;
            }
        }
        musicList.push_back(loadedMusic);
    }

    // Đặt tên hiển thị cho các bài hát in-game
    songNames.push_back("Bai hat 1");
    songNames.push_back("Bai hat 2");
    songNames.push_back("Bai hat 3");

    // Mặc định chọn bài 1 cho in-game (chưa phát, chỉ phát khi vào PLAYING)
    currentSongIndex = 0;
    hoveredSongIndex = 0;
    isPreviewingMusic = false;

    // Bật loop cho tất cả bài hát in-game
    for (auto& music : musicList) {
        music.looping = true;
    }

    // Khởi tạo game state
    resetGame();
}

//  RESET GAME STATE
void Game::resetGame() {
    // Reset game state
    board = Board();                           // Tạo bàn cờ mới
    currentPlayer = board.getCurrentPlayer();
    isGameOver = false;
    winner = ' ';
    consecutivePasses = 0;
    hoverRow = -1;
    hoverCol = -1;
    blackFinalScore = 0;
    whiteFinalScore = 0;

    // Reset AI mode (mặc định là PVP)
    isAIMode = false;
    aiDifficulty = Difficulty::Easy;
    aiPlayer = 'W';  // AI mặc định chơi quân trắng

    // Clear history
    while (!history.empty()) history.pop();
    while (!redoStack.empty()) redoStack.pop();


    std::cout << "=== RESET GAME ===" << std::endl;
    std::cout << "Black Capture: " << board.getBlackCapture() << std::endl;
    std::cout << "White Capture: " << board.getWhiteCapture() << std::endl;
    std::pair<int, int> area = board.CountArea();
    std::cout << "Black Area: " << area.first << std::endl;
    std::cout << "White Area: " << area.second << std::endl;

    // Reset unsaved changes flag (game mới chưa có gì để lưu)
    ui.hasUnsavedChanges = false;
}

//  XỬ LÝ INPUT
void Game::handleInput() {
    // Lấy vị trí chuột
    Vector2 mousePos = GetMousePosition();

    //XỬ LÝ TEXT INPUT
    if (ui.showSavePopup) {
        int key = GetCharPressed();
        while (key > 0) {
            ui.handleTextInput(key);
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            ui.handleTextInput(KEY_BACKSPACE);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            std::string savePath = ui.getSaveGameName();
            if (!savePath.empty()) {
                ui.saveRequested = true;
                ui.showSavePopup = false;
            }
        }
        return;
    }

    // XỬ LÝ SAVE
    if (ui.saveRequested) {
        std::string savePath = ui.getSaveGameName();
        std::cout << "=== SAVE REQUEST ===" << std::endl;
        std::cout << "Save path: '" << savePath << "'" << std::endl;
        std::cout << "Save name length: " << ui.getSaveGameNameLength() << std::endl;
        std::cout << "Save name buffer: '" << ui.getSaveGameNameBuffer() << "'" << std::endl;

        if (!savePath.empty()) {
            std::cout << "Calling board.SaveGame()..." << std::endl;
            if (board.SaveGame(savePath)) {
                std::cout << "SUCCESS! Game saved to: " << savePath << std::endl;
                ui.hasUnsavedChanges = false;  // Đánh dấu đã lưu game
            } else {
                std::cout << "ERROR! Failed to save game to: " << savePath << std::endl;
            }
        } else {
            std::cout << "ERROR! Save path is empty!" << std::endl;
        }
        ui.saveRequested = false;
    }

    //XỬ LÝ LOAD
    if (ui.loadRequested) {
        std::string loadPath = ui.getSelectedSaveFile();
        std::cout << "=== LOAD REQUEST ===" << std::endl;
        std::cout << "Load path: '" << loadPath << "'" << std::endl;
        std::cout << "Selected index: " << ui.selectedSaveIndex << std::endl;

        if (!loadPath.empty()) {
            std::cout << "Calling board.LoadGame()..." << std::endl;
            if (board.LoadGame(loadPath)) {
                std::cout << "SUCCESS! Game loaded from: " << loadPath << std::endl;
                // Sync game state with loaded board
                currentPlayer = board.getCurrentPlayer();
                isGameOver = false;
                consecutivePasses = 0;
                // Clear history after loading
                while (!history.empty()) history.pop();
                while (!redoStack.empty()) redoStack.pop();
                // Game vừa load không có thay đổi chưa lưu
                ui.hasUnsavedChanges = false;
            } else {
                std::cout << "ERROR! Failed to load game from: " << loadPath << std::endl;
            }
        } else {
            std::cout << "ERROR! Load path is empty!" << std::endl;
        }
        ui.loadRequested = false;
        ui.selectedSaveIndex = -1;
    }

    //XỬ LÝ LOAD POPUP
    if (ui.showLoadPopup) {
        return;
    }

    // ===== XỬ LÝ MENU CONFIRM DIALOG =====
    if (ui.showMenuConfirmDialog) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();

            // Click vào "MAIN MENU" → quay về main menu
            if (ui.isConfirmMainMenuClicked(mousePos)) {
                ui.showMenuConfirmDialog = false;
                ui.resetToMainMenu();
                currentState = MENU;

                // Dừng in-game music, phát lại menu music
                if (musicList[currentSongIndex].stream.buffer != NULL) {
                    StopMusicStream(musicList[currentSongIndex]);
                }
                if (menuBackgroundMusic.stream.buffer != NULL) {
                    PlayMusicStream(menuBackgroundMusic);
                    printf("Returned to menu, playing menu background music\n");
                }
                return;
            }

            // Click vào "CLOSE" → đóng dialog
            if (ui.isCloseDialogClicked(mousePos)) {
                ui.showMenuConfirmDialog = false;
                return;
            }
        }
        return;  // Don't process other input while dialog is open
    }

    // ===== XỬ LÝ INPUT THEO STATE =====
    if (currentState == MENU) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (ui.isPlayerVsPlayerClicked(mousePos)) {
                // Chuyển sang chế độ chơi
                currentState = PLAYING;
                resetGame();

                // Dừng menu music, phát in-game music
                StopMusicStream(menuBackgroundMusic);
                if (musicList[currentSongIndex].stream.buffer != NULL) {
                    PlayMusicStream(musicList[currentSongIndex]);
                    printf("Switched to in-game music: %s\n", songNames[currentSongIndex].c_str());
                }
                return;
            }
            // Xử lý click vào button AI để mở difficulty menu
            ui.isPlayerVsAIClicked(mousePos);

            // CHỈ xử lý click difficulty buttons KHI đang ở trong difficulty menu
            if (ui.showDifficultyMenu) {
                // Xử lý click vào các nút difficulty
                if (ui.isEasyButtonClicked(mousePos)) {
                    printf("EASY difficulty selected!\n");
                    ui.resetToMainMenu();  // Reset trước khi vào game
                    currentState = PLAYING;
                    resetGame();
                    // Bật AI mode với độ khó Easy
                    isAIMode = true;
                    aiDifficulty = Difficulty::Easy;
                    aiPlayer = 'W';  // AI chơi quân trắng, player chơi quân đen
                    printf("AI Mode: ON | Difficulty: EASY | AI plays: WHITE\n");

                    // Dừng menu music, phát in-game music
                    StopMusicStream(menuBackgroundMusic);
                    if (musicList[currentSongIndex].stream.buffer != NULL) {
                        PlayMusicStream(musicList[currentSongIndex]);
                        printf("Switched to in-game music: %s\n", songNames[currentSongIndex].c_str());
                    }
                    return;
                }

                if (ui.isMediumButtonClicked(mousePos)) {
                    printf("MEDIUM difficulty selected!\n");
                    // TODO: Khởi tạo game với AI độ khó Medium
                    ui.resetToMainMenu();  // Reset trước khi vào game
                    currentState = PLAYING;
                    resetGame();
                    // Bật AI mode với độ khó Medium
                    isAIMode = true;
                    aiDifficulty = Difficulty::Medium;
                    aiPlayer = 'W';  // AI chơi quân trắng, player chơi quân đen
                    printf("AI Mode: ON | Difficulty: MEDIUM | AI plays: WHITE\n");

                    // Dừng menu music, phát in-game music
                    StopMusicStream(menuBackgroundMusic);
                    if (musicList[currentSongIndex].stream.buffer != NULL) {
                        PlayMusicStream(musicList[currentSongIndex]);
                        printf("Switched to in-game music: %s\n", songNames[currentSongIndex].c_str());
                    }
                    return;
                }

                if (ui.isHardButtonClicked(mousePos)) {
                    printf("HARD difficulty selected!\n");
                    // TODO: Khởi tạo game với AI độ khó Hard
                    ui.resetToMainMenu();  // Reset trước khi vào game
                    currentState = PLAYING;
                    resetGame();
                    // Bật AI mode với độ khó Hard
                    isAIMode = true;
                    aiDifficulty = Difficulty::Hard;
                    aiPlayer = 'W';  // AI chơi quân trắng, player chơi quân đen
                    printf("AI Mode: ON | Difficulty: HARD | AI plays: WHITE\n");

                    // Dừng menu music, phát in-game music
                    StopMusicStream(menuBackgroundMusic);
                    if (musicList[currentSongIndex].stream.buffer != NULL) {
                        PlayMusicStream(musicList[currentSongIndex]);
                        printf("Switched to in-game music: %s\n", songNames[currentSongIndex].c_str());
                    }
                    return;
                }

                // Xử lý click nút Back (quay về menu chính từ difficulty menu)
                if (ui.isBackButtonClicked(mousePos)) {
                    ui.resetToMainMenu();
                    return;
                }
            }

            // Settings button chỉ ở main menu, không ở difficulty menu
            if (!ui.showDifficultyMenu && ui.isPlayerVsSettingClicked(mousePos)) {
                printf("Settings button clicked! Opening Settings menu...\n");
                currentState = SETTINGS;
                hoveredSongIndex = currentSongIndex;  // Reset hover về bài đang phát
                isPreviewingMusic = false;

                // Stop menu music khi vào Settings (để preview nhạc in-game)
                StopMusicStream(menuBackgroundMusic);
                printf("Stopped menu music for preview mode\n");
                return;
            }

            // Click vào bất kỳ đâu để đóng notification (sẽ xử lý trong UI)
        }
        return;
    }

    // ===== XỬ LÝ INPUT TRONG SETTINGS MENU =====
    if (currentState == SETTINGS) {
        handleSettingsInput();
        return;
    }

    // XỬ LÝ INPUT KHI ĐANG CHƠI
    // Chuyển đổi pixel → (row, col)
    ui.screenToBoard(mousePos, hoverRow, hoverCol);

    //XỬ LÝ CLICK CHUỘT
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Kiểm tra click nút MENU trong game over screen trước
        if (isGameOver && ui.isNewGameButtonGameOverClicked(mousePos)) {
            // Quay về menu chính (reset về main menu, không phải difficulty menu)
            ui.resetToMainMenu();
            currentState = MENU;

            // Dừng in-game music, phát lại menu music
            if (musicList[currentSongIndex].stream.buffer != NULL) {
                StopMusicStream(musicList[currentSongIndex]);
            }
            if (menuBackgroundMusic.stream.buffer != NULL) {
                PlayMusicStream(menuBackgroundMusic);
                printf("Returned to menu from game over, playing menu background music\n");
            }
            return;
        }
        if (ui.isUndoButtonClicked(mousePos)) {
            handleUndo();
            return;
        }
        if (ui.isRedoButtonClicked(mousePos)) {
            handleRedo();
            return;
        }
        if (ui.isPassButtonClicked(mousePos)) {
            handlePass();
            return;
        }
        if (ui.isNewGameButtonClicked(mousePos)) {
            handleNewGame();
            return;
        }
        if (ui.isSaveGameButtonClicked(mousePos)) {
            // Open save popup (handled in UI)
            return;
        }
        if (ui.isLoadGameButtonClicked(mousePos)) {
            // Open load popup (handled in UI)
            return;
        }
        if (ui.isMenuButtonClicked(mousePos)) {
            // Open menu confirm dialog (handled in UI)
            return;
        }

        // Nếu không click nút → đặt quân
        if (!isGameOver) {
            handleStonePlace(hoverRow, hoverCol);
        }
    }

    //XỬ LÝ PHÍM TẮT
    if (IsKeyPressed(KEY_U)) {
        handleUndo();
    }
    if (IsKeyPressed(KEY_R)) {
        handleRedo();
    }
    if (IsKeyPressed(KEY_P)) {
        handlePass();
    }
}

//XỬ LÝ ĐẶT QUÂN
void Game::handleStonePlace(int row, int col) {
    history.push(board);  // Lưu trạng thái hiện tại
    // Thử đặt quân vào Board (Board tự quản lý player và tự switch)
    bool success = board.PlaceStone(row, col);

    if (success) {
        consecutivePasses = 0;
        currentPlayer = board.getCurrentPlayer();  // Đồng bộ với Board
        checkGameOver();

        // Clear redo stack
        while (!redoStack.empty()) redoStack.pop();

        // Đánh dấu có thay đổi chưa lưu
        ui.hasUnsavedChanges = true;

        // Nếu đang ở AI mode và chưa game over → cho AI đánh
        if (isAIMode && !isGameOver) {
            handleAITurn();
        }
    } else {
        // Đặt quân thất bại
        history.pop();
    }
}

//XỬ LÝ UNDO
void Game::handleUndo() {
    // Trong AI mode, undo 2 lần (AI move + Player move) để không rơi vào lượt AI
    int undoCount = isAIMode ? 2 : 1;

    for (int i = 0; i < undoCount; i++) {
        // Kiểm tra có history không
        if (history.empty()) {
            std::cout << "No moves to undo!" << std::endl;
            return;
        }

        // Lưu trạng thái hiện tại vào redo stack
        redoStack.push(board);

        // Khôi phục trạng thái trước đó
        board = history.top();
        history.pop();

        // Đồng bộ currentPlayer với Board (Board đã có player cũ)
        currentPlayer = board.getCurrentPlayer();

        // Reset game over nếu đang ở trạng thái kết thúc
        if (isGameOver) {
            isGameOver = false;
            consecutivePasses = 0;
        }
    }

    // Đánh dấu có thay đổi chưa lưu
    ui.hasUnsavedChanges = true;
}

// XỬ LÝ REDO
void Game::handleRedo() {
    // Trong AI mode, redo 2 lần (Player move + AI move) để không rơi vào lượt AI
    int redoCount = isAIMode ? 2 : 1;

    for (int i = 0; i < redoCount; i++) {
        // Kiểm tra có redo stack không
        if (redoStack.empty()) {
            std::cout << "No moves to redo!" << std::endl;
            return;
        }

        // Lưu trạng thái hiện tại vào history
        history.push(board);

        // Khôi phục trạng thái từ redo stack
        board = redoStack.top();
        redoStack.pop();

        // Đồng bộ currentPlayer với Board
        currentPlayer = board.getCurrentPlayer();
    }

    // Đánh dấu có thay đổi chưa lưu
    ui.hasUnsavedChanges = true;
}

// XỬ LÝ PASS
void Game::handlePass() {
    if (isGameOver) {
        return;
    }

    consecutivePasses++;
    board.switchPlayer();
    currentPlayer = board.getCurrentPlayer();

    checkGameOver();

    std::cout << "Player passed. Consecutive passes: " << consecutivePasses << std::endl;

    // Đánh dấu có thay đổi chưa lưu
    ui.hasUnsavedChanges = true;
}

// XỬ LÝ NEW GAME
void Game::handleNewGame() {
    resetGame();  // Reset game state
}

// ========== XỬ LÝ LƯỢT CHƠI CỦA AI ==========
void Game::handleAITurn() {
    // Chỉ chạy khi đang ở AI mode và đến lượt AI
    if (!isAIMode || isGameOver) {
        return;
    }

    // Kiểm tra có phải lượt của AI không
    if (currentPlayer != aiPlayer) {
        return;
    }

    std::cout << "AI is thinking..." << std::endl;

    // Gọi AI để tìm nước đi tốt nhất
    std::pair<int, int> aiMove = ai.FindBestMove(board, aiPlayer, aiDifficulty);
    int row = aiMove.first;
    int col = aiMove.second;

    std::cout << "AI plays at: (" << row << ", " << col << ")" << std::endl;

    // Lưu vào history trước khi đặt quân
    history.push(board);

    // Thực hiện nước đi của AI
    bool success = board.PlaceStone(row, col);

    if (success) {
        // Đặt quân thành công
        consecutivePasses = 0;
        currentPlayer = board.getCurrentPlayer();  // Đồng bộ với Board (đã switch về player)
        checkGameOver();

        // Clear redo stack
        while (!redoStack.empty()) redoStack.pop();

        // Đánh dấu có thay đổi chưa lưu
        ui.hasUnsavedChanges = true;
    } else {
        // Đặt quân thất bại (không nên xảy ra nếu AI hoạt động đúng)
        std::cout << "ERROR: AI tried to place invalid move!" << std::endl;
        history.pop();
    }
}

//XỬ LÝ INPUT TRONG SETTINGS MENU
void Game::handleSettingsInput() {
    Vector2 mousePos = GetMousePosition();

    // ========== TỌA ĐỘ KHỚP VỚI UI.CPP ==========
    const int SCREEN_WIDTH = 1400;
    const int SCREEN_HEIGHT = 900;

    // Vùng danh sách bài hát (khớp với UI.cpp)
    int listStartY = 470;
    int itemHeight = 50;
    int itemSpacing = 20;
    int listWidth = 450;
    int listX = (SCREEN_WIDTH - listWidth) / 2;  // = 475

    // Xử lý hover trên các bài hát
    hoveredSongIndex = -1;
    for (int i = 0; i < (int)songNames.size(); i++) {
        int itemY = listStartY + i * (itemHeight + itemSpacing);
        Rectangle itemRect = {(float)listX, (float)itemY, (float)listWidth, (float)itemHeight};

        if (CheckCollisionPointRec(mousePos, itemRect)) {
            hoveredSongIndex = i;

            // Click vào bài hát → preview nhạc ngay lập tức
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Dừng bài preview hiện tại (nếu có)
                if (isPreviewingMusic && musicList[currentSongIndex].stream.buffer != NULL) {
                    StopMusicStream(musicList[currentSongIndex]);
                }

                // Preview bài mới
                currentSongIndex = i;
                isPreviewingMusic = true;
                if (musicList[currentSongIndex].stream.buffer != NULL) {
                    PlayMusicStream(musicList[currentSongIndex]);
                    printf("Previewing %s\n", songNames[i].c_str());
                }
            }
        }
    }

    // Nút "CHOOSE THIS MUSIC" (confirm button) - khớp với UI.cpp
    int buttonWidth = 300;
    int buttonHeight = 60;
    int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;  // Center horizontally
    int buttonY = 740;  // Vị trí cuối danh sách bài hát
    Rectangle confirmButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};

    if (CheckCollisionPointRec(mousePos, confirmButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Stop preview music
        if (isPreviewingMusic && musicList[currentSongIndex].stream.buffer != NULL) {
            StopMusicStream(musicList[currentSongIndex]);
        }

        // Quay về menu chính, phát lại menu background music
        currentState = MENU;
        isPreviewingMusic = false;
        if (menuBackgroundMusic.stream.buffer != NULL) {
            PlayMusicStream(menuBackgroundMusic);
            printf("Settings confirmed, returned to menu with menu music\n");
        }
    }

    // Nút Back (góc trên bên trái) - khớp với UI.cpp
    int backButtonSize = 80;
    int backButtonX = (int)(SCREEN_WIDTH * 0.1);   // = 140
    int backButtonY = (int)(SCREEN_HEIGHT * 0.3);  // = 270
    Rectangle backButtonRect = {(float)backButtonX, (float)backButtonY, (float)backButtonSize, (float)backButtonSize};

    if (CheckCollisionPointRec(mousePos, backButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Stop preview music
        if (isPreviewingMusic && musicList[currentSongIndex].stream.buffer != NULL) {
            StopMusicStream(musicList[currentSongIndex]);
        }

        // Quay về menu chính, phát lại menu background music
        currentState = MENU;
        isPreviewingMusic = false;
        if (menuBackgroundMusic.stream.buffer != NULL) {
            PlayMusicStream(menuBackgroundMusic);
            printf("Back button clicked, returned to menu with menu music\n");
        }
    }
}

//CHUYỂN LƯỢT CHƠI
void Game::switchPlayer() {
    currentPlayer = (currentPlayer == 'B') ? 'W' : 'B';
}

//KIỂM TRA KẾT THÚC GAME
void Game::checkGameOver() {
    // Điều kiện kết thúc: 2 người chơi đều pass liên tiếp
    if (consecutivePasses >= 2) {
        isGameOver = true;
        calculateFinalScore();

        std::cout << "Game Over!" << std::endl;
        std::cout << "Black: " << blackFinalScore << " points" << std::endl;
        std::cout << "White: " << whiteFinalScore << " points" << std::endl;
    }
}

//TÍNH ĐIỂM CUỐI CÙNG
void Game::calculateFinalScore() {
    // Lấy số lãnh thổ từ Board
    std::pair<int, int> area = board.CountArea();
    int blackArea = area.first;
    int whiteArea = area.second;

    // Lấy số quân bị bắt
    int blackCapture = board.getBlackCapture();
    int whiteCapture = board.getWhiteCapture();

    // Tính tổng điểm
    // Black: Lãnh thổ + Quân bắt được
    blackFinalScore = blackArea + blackCapture;

    // White: Lãnh thổ + Quân bắt được +  6.5 (làm tròn = 7)
    // Komi là điểm bù cho White vì Black đi trước
    whiteFinalScore = whiteArea + whiteCapture + 7;

    // Xác định người thắng
    if (blackFinalScore >= whiteFinalScore) {
        winner = 'B';
    } else if (whiteFinalScore > blackFinalScore) {
        winner = 'W';
    } else {
        winner = 'D';
    }
}
void Game::update() {
    // Cập nhật music stream tùy theo state hiện tại
    if (currentState == MENU) {
        // Ở menu → update menu background music
        if (menuBackgroundMusic.stream.buffer != NULL) {
            UpdateMusicStream(menuBackgroundMusic);
        }
    } else if (currentState == SETTINGS) {
        // Ở Settings → update preview music (nếu đang preview)
        if (isPreviewingMusic && currentSongIndex >= 0 && currentSongIndex < (int)musicList.size()) {
            if (musicList[currentSongIndex].stream.buffer != NULL) {
                UpdateMusicStream(musicList[currentSongIndex]);
            }
        }
    } else if (currentState == PLAYING) {
        // Đang chơi game → update in-game music
        if (currentSongIndex >= 0 && currentSongIndex < (int)musicList.size()) {
            if (musicList[currentSongIndex].stream.buffer != NULL) {
                UpdateMusicStream(musicList[currentSongIndex]);
            }
        }
    }
}

//VẼ UI
void Game::render() {
    ui.beginDrawing();
    static int frameCount = 0;
    if (frameCount++ % 60 == 0) {
        std::cout << "Current State: " << (currentState == MENU ? "MENU" : currentState == SETTINGS ? "SETTINGS" : "PLAYING") << std::endl;
    }

    if (currentState == MENU) {
        ui.drawMenu();
    } else if (currentState == SETTINGS) {
        // Vẽ Settings menu với music list
        ui.drawSettingsMenu(songNames, currentSongIndex, hoveredSongIndex);
    } else {
        ui.drawBoard();
        ui.drawStones(board);
        if (!isGameOver) {
            ui.drawHoverEffect(hoverRow, hoverCol);
        }
        ui.drawPlayerTurn(currentPlayer);
        std::pair<int, int> area = board.CountArea();
        ui.drawScore(area.first, area.second,
                     board.getBlackCapture(), board.getWhiteCapture());
        ui.drawButtons();
        if (isGameOver) {
            ui.drawGameOver(winner, blackFinalScore, whiteFinalScore);
        }
        if (ui.showSavePopup) {
            ui.drawSaveGamePopup();
        }
        if (ui.showLoadPopup) {
            ui.drawLoadGamePopup();
        }

        // Vẽ menu confirm dialog nếu đang mở
        if (ui.showMenuConfirmDialog) {
            ui.drawMenuConfirmDialog();
        }
    }

    ui.endDrawing();
}
void Game::cleanup() {
    // Unload menu background music
    if (menuBackgroundMusic.stream.buffer != NULL) {
        UnloadMusicStream(menuBackgroundMusic);
    }

    // Unload tất cả in-game music streams
    for (auto& music : musicList) {
        if (music.stream.buffer != NULL) {
            UnloadMusicStream(music);
        }
    }

    CloseAudioDevice();
    ui.cleanup();
}
