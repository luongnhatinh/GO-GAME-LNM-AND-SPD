#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <raylib.h>
#include "board.h"

class UI {
public:
    // Constructor/Destructor
    UI();
    ~UI();

    // Kh�i t�o v� �ng Raylib window
    void init();
    void cleanup();

    // V� c�c th�nh ph�n ch�nh
    void drawBoard();                                      // V� grid 19x19
    void drawStones(const Board& board);                   // V� qu�n c� t� Board
    void drawHoverEffect(int row, int col);                // Highlight � ang hover
    void drawPlayerTurn(char currentPlayer);               // Hi�n th� "Black's turn"
    void drawScore(int blackArea, int whiteArea,
                   int blackCapture, int whiteCapture);    // Hi�n th� i�m
    void drawButtons();                                    // V� c�c n�t Undo/Redo/Pass
    void drawGameOver(char winner, int blackScore, int whiteScore); // M�n h�nh k�t th�c

    // Chuy�n �i t�a �
    void screenToBoard(Vector2 mousePos, int& row, int& col);  // Pixel � (row, col)
    Vector2 boardToScreen(int row, int col);                    // (row, col) � Pixel

    // Ki�m tra click n�t
    bool isUndoButtonClicked(Vector2 mousePos);
    bool isRedoButtonClicked(Vector2 mousePos);
    bool isPassButtonClicked(Vector2 mousePos);
    bool isNewGameButtonClicked(Vector2 mousePos);
    bool isNewGameButtonGameOverClicked(Vector2 mousePos);  // N�t NEW GAME trong m�n h�nh k�t th�c

    // B�t �u/k�t th�c v� frame
    void beginDrawing();
    void endDrawing();

    // Ki�m tra window c� n�n �ng kh�ng
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
    void drawStarPoints();       // V� c�c i�m sao (hoshi) tr�n b�n c�
    void drawCoordinates();      // V� t�a � A-T, 1-19
    void initButtons();          // Kh�i t�o v� tr� c�c n�t
    bool isInsideBoard(int row, int col);  // Ki�m tra t�a � c� trong b�n c� kh�ng
};

#endif // UI_H_INCLUDED