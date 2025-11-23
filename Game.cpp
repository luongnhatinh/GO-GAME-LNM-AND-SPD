#include "Game.h"
#include "board.h"
#include "UI.h"
#include <raylib.h>
#include <iostream>

// ========== GAME LOOP CHÍNH ==========
void Game::run() {
    init();

    // Game loop - chạy cho đến khi đóng window
    while (!ui.shouldClose()) {
        handleInput();
        update();
        render();
    }

    cleanup();
}

// ========== KHỞI TẠO GAME ==========
void Game::init() {
    // Khởi tạo UI (Raylib window) - CHỈ GỌI 1 LẦN
    ui.init();

    // Khởi tạo game state
    resetGame();
}

// ========== RESET GAME STATE ==========
void Game::resetGame() {
    // Reset game state (KHÔNG tạo window mới)
    board = Board();                           // Tạo bàn cờ mới (tự set player = 'B')
    currentPlayer = board.getCurrentPlayer();  // Đồng bộ với Board
    isGameOver = false;
    winner = ' ';
    consecutivePasses = 0;
    hoverRow = -1;
    hoverCol = -1;
    blackFinalScore = 0;
    whiteFinalScore = 0;

    // Clear history
    while (!history.empty()) history.pop();
    while (!redoStack.empty()) redoStack.pop();

    // Debug output
    std::cout << "=== RESET GAME ===" << std::endl;
    std::cout << "Black Capture: " << board.getBlackCapture() << std::endl;
    std::cout << "White Capture: " << board.getWhiteCapture() << std::endl;
    std::pair<int, int> area = board.CountArea();
    std::cout << "Black Area: " << area.first << std::endl;
    std::cout << "White Area: " << area.second << std::endl;
}

// ========== XỬ LÝ INPUT ==========
void Game::handleInput() {
    // Lấy vị trí chuột
    Vector2 mousePos = GetMousePosition();

    // Chuyển đổi pixel → (row, col) qua UI
    ui.screenToBoard(mousePos, hoverRow, hoverCol);

    // ===== XỬ LÝ CLICK CHUỘT =====
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Kiểm tra click nút NEW GAME trong game over screen trước
        if (isGameOver && ui.isNewGameButtonGameOverClicked(mousePos)) {
            handleNewGame();
            return;
        }

        // Kiểm tra click nút trước
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

        // Nếu không click nút → đặt quân
        if (!isGameOver) {
            handleStonePlace(hoverRow, hoverCol);
        }
    }

    // ===== XỬ LÝ PHÍM TẮT =====
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

// ========== XỬ LÝ ĐẶT QUÂN ==========
void Game::handleStonePlace(int row, int col) {
    // LƯU VÀO HISTORY TRƯỚC KHI ĐẶT QUÂN
    history.push(board);  // Lưu trạng thái hiện tại

    // Thử đặt quân vào Board (Board tự quản lý player và tự switch)
    bool success = board.PlaceStone(row, col);

    if (success) {
        // Đặt quân thành công
        consecutivePasses = 0;  // Reset số lần pass
        currentPlayer = board.getCurrentPlayer();  // Đồng bộ với Board (đã switch)
        checkGameOver();         // Kiểm tra kết thúc

        // Clear redo stack (không thể redo sau khi đặt quân mới)
        while (!redoStack.empty()) redoStack.pop();
    } else {
        // Đặt quân thất bại → Xóa snapshot vừa lưu
        history.pop();
    }
}

// ========== XỬ LÝ UNDO ==========
void Game::handleUndo() {
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

// ========== XỬ LÝ REDO ==========
void Game::handleRedo() {
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

// ========== XỬ LÝ PASS ==========
void Game::handlePass() {
    if (isGameOver) {
        return;  // Không cho pass khi game đã kết thúc
    }

    consecutivePasses++;

    // Board tự switch player
    board.switchPlayer();
    currentPlayer = board.getCurrentPlayer();

    checkGameOver();

    std::cout << "Player passed. Consecutive passes: " << consecutivePasses << std::endl;
}

// ========== XỬ LÝ NEW GAME ==========
void Game::handleNewGame() {
    resetGame();  // Reset game state (KHÔNG tạo window mới)
}

// ========== CHUYỂN LƯỢT CHƠI ==========
void Game::switchPlayer() {
    // Không còn cần thiết vì Board tự quản lý player
    // Giữ lại để backward compatibility
    currentPlayer = (currentPlayer == 'B') ? 'W' : 'B';
}

// ========== KIỂM TRA KẾT THÚC GAME ==========
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

// ========== TÍNH ĐIỂM CUỐI CÙNG ==========
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

    // White: Lãnh thổ + Quân bắt được + Komi 7.5 (làm tròn = 7)
    // Komi là điểm bù cho White vì Black đi trước
    whiteFinalScore = whiteArea + whiteCapture + 7;

    // Xác định người thắng
    if (blackFinalScore >= whiteFinalScore) {
        winner = 'B';
    } else if (whiteFinalScore > blackFinalScore) {
        winner = 'W';
    } else {
        winner = 'D';  // Draw (hòa)
    }
}

// ========== CẬP NHẬT LOGIC ==========
void Game::update() {
    // Hiện tại không có logic cần cập nhật mỗi frame
    // Có thể thêm animation, timer, v.v. sau
}

// ========== VẼ UI ==========
void Game::render() {
    ui.beginDrawing();

    // Vẽ bàn cờ
    ui.drawBoard();

    // Vẽ tất cả quân cờ từ Board
    ui.drawStones(board);

    // Vẽ hiệu ứng hover (chỉ khi chưa game over)
    if (!isGameOver) {
        ui.drawHoverEffect(hoverRow, hoverCol);
    }

    // Vẽ thông tin lượt chơi
    ui.drawPlayerTurn(currentPlayer);

    // Vẽ điểm số hiện tại
    std::pair<int, int> area = board.CountArea();
    ui.drawScore(area.first, area.second,
                 board.getBlackCapture(), board.getWhiteCapture());

    // Vẽ các nút
    ui.drawButtons();

    // Nếu game over → vẽ màn hình kết thúc
    if (isGameOver) {
        ui.drawGameOver(winner, blackFinalScore, whiteFinalScore);
    }

    ui.endDrawing();
}

// ========== DỌN DẸP ==========
void Game::cleanup() {
    ui.cleanup();
}
