#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <raylib.h>
#include "board.h"

class UI {
public:
    // Constructor/Destructor
    UI();
    ~UI();

    // Khởi tạo và đóng Raylib window
    void init();
    void cleanup();

    // Vẽ các thành phần chính
    void drawBoard();                                      // Vẽ grid 19x19
    void drawStones(const Board& board);                   // Vẽ quân cờ từ Board
    void drawHoverEffect(int row, int col);                // Highlight ô đang hover
    void drawPlayerTurn(char currentPlayer);               // Hiển thị "Black's turn"
    void drawScore(int blackArea, int whiteArea,
                   int blackCapture, int whiteCapture);    // Hiển thị điểm
    void drawButtons();                                    // Vẽ các nút Undo/Redo/Pass
    void drawGameOver(char winner, int blackScore, int whiteScore); // Màn hình kết thúc
    void drawMenu();                                       // Vẽ màn hình menu
    void drawAINotification();                             // Vẽ thông báo AI chưa sẵn sàng

    // Chuyển đổi tọa độ
    void screenToBoard(Vector2 mousePos, int& row, int& col);  // Pixel → (row, col)
    Vector2 boardToScreen(int row, int col);                    // (row, col) → Pixel

    // Kiểm tra click nút
    bool isUndoButtonClicked(Vector2 mousePos);
    bool isRedoButtonClicked(Vector2 mousePos);
    bool isPassButtonClicked(Vector2 mousePos);
    bool isNewGameButtonClicked(Vector2 mousePos);
    bool isNewGameButtonGameOverClicked(Vector2 mousePos);  // Nút NEW GAME trong màn hình kết thúc
    bool isPlayerVsPlayerClicked(Vector2 mousePos);         // Nút Player vs Player trong menu
    bool isPlayerVsAIClicked(Vector2 mousePos);             // Nút Player vs AI trong menu

    // Bắt đầu/kết thúc vẽ frame
    void beginDrawing();
    void endDrawing();

    // Kiểm tra window có nên đóng không
    bool shouldClose();

private:
    // ========== UI CONSTANTS ==========
    static const int SCREEN_WIDTH = 1400;
    static const int SCREEN_HEIGHT = 900;
    static const int CELL_SIZE = 40;
    static const int BOARD_OFFSET_X = 100;
    static const int BOARD_OFFSET_Y = 100;
    static const int STONE_RADIUS = 18;
    static const int BOARD_SIZE = 19;

    // ========== BUTTON RECTANGLES ==========
    Rectangle undoButton;
    Rectangle redoButton;
    Rectangle passButton;
    Rectangle newGameButton;

    // ========== MENU ==========
    Texture2D menuTexture;
    Texture2D buttonPlayerTexture;
    Texture2D buttonAITexture;
    Rectangle playerVsPlayerButton;
    Rectangle playerVsAIButton;
    bool showAINotification;
    int notificationFrameCounter;  // Đếm frame từ khi mở notification

    // ========== COLORS ==========
    Color boardColor;
    Color lineColor;
    Color starPointColor;
    Color blackStoneColor;
    Color whiteStoneColor;
    Color hoverColor;

    // ========== HELPER FUNCTIONS ==========
    void drawGridLines();
    void drawStarPoints();       // Vẽ các điểm sao (hoshi) trên bàn cờ
    void drawCoordinates();      // Vẽ tọa độ A-T, 1-19
    void initButtons();          // Khởi tạo vị trí các nút
    void initMenuButtons();      // Khởi tạo vị trí nút menu
    bool isInsideBoard(int row, int col);  // Kiểm tra tọa độ có trong bàn cờ không
};

#endif // UI_H_INCLUDED
