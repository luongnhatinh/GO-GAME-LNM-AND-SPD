#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <raylib.h>
#include "board.h"

class UI {
public:
    // Constructor/Destructor
    UI();
    ~UI();

    // Khßi t¡o và óng Raylib window
    void init();
    void cleanup();

    // V½ các thành ph§n chính
    void drawBoard();                                      // V½ grid 19x19
    void drawStones(const Board& board);                   // V½ quân cÝ të Board
    void drawHoverEffect(int row, int col);                // Highlight ô ang hover
    void drawPlayerTurn(char currentPlayer);               // HiÃn thË "Black's turn"
    void drawScore(int blackArea, int whiteArea,
                   int blackCapture, int whiteCapture);    // HiÃn thË iÃm
    void drawButtons();                                    // V½ các nút Undo/Redo/Pass
    void drawGameOver(char winner, int blackScore, int whiteScore); // Màn hình k¿t thúc

    // ChuyÃn Õi tÍa Ù
    void screenToBoard(Vector2 mousePos, int& row, int& col);  // Pixel ’ (row, col)
    Vector2 boardToScreen(int row, int col);                    // (row, col) ’ Pixel

    // KiÃm tra click nút
    bool isUndoButtonClicked(Vector2 mousePos);
    bool isRedoButtonClicked(Vector2 mousePos);
    bool isPassButtonClicked(Vector2 mousePos);
    bool isNewGameButtonClicked(Vector2 mousePos);

    // B¯t §u/k¿t thúc v½ frame
    void beginDrawing();
    void endDrawing();

    // KiÃm tra window có nên óng không
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

    // ========== COLORS ==========
    Color boardColor;
    Color lineColor;
    Color starPointColor;
    Color blackStoneColor;
    Color whiteStoneColor;
    Color hoverColor;

    // ========== HELPER FUNCTIONS ==========
    void drawGridLines();
    void drawStarPoints();       // V½ các iÃm sao (hoshi) trên bàn cÝ
    void drawCoordinates();      // V½ tÍa Ù A-T, 1-19
    void initButtons();          // Khßi t¡o vË trí các nút
    bool isInsideBoard(int row, int col);  // KiÃm tra tÍa Ù có trong bàn cÝ không
};

#endif // UI_H_INCLUDED