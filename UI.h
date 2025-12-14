#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <raylib.h>
#include <vector>
#include <string>
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
    void drawBoard();// Vẽ grid 19x19
    void drawStones(const Board& board);  // Vẽ quân cờ từ Board
    void drawHoverEffect(int row, int col);   // Highlight ô đang hover
    void drawPlayerTurn(char currentPlayer);  // Hiển thị "Black's turn"
    void drawScore(int blackArea, int whiteArea,
                   int blackCapture, int whiteCapture);// Hiển thị điểm
    void drawButtons();// Vẽ các nút Undo/Redo/Pass
    void drawGameOver(char winner, int blackScore, int whiteScore); // Màn hình kết thúc
    void drawMenu();                                       // Vẽ màn hình menu
    void drawDifficultyMenu();                             // Vẽ màn hình chọn độ khó
    void drawSettingsMenu(const std::vector<std::string>& songNames, int currentSongIndex, int hoveredSongIndex);  // Vẽ màn hình Settings (chọn nhạc)
    void drawAINotification();                             // Vẽ thông báo AI chưa sẵn sàng
    void drawSaveGamePopup();                              // Vẽ popup save game
    void drawLoadGamePopup();                              // Vẽ popup load game
    void drawMenuConfirmDialog();                          // Vẽ dialog xác nhận quay về menu

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
    bool isPlayerVsSettingClicked(Vector2 mousePos);        // Nút Settings trong menu
    bool isEasyButtonClicked(Vector2 mousePos);             // Nút Easy trong difficulty menu
    bool isMediumButtonClicked(Vector2 mousePos);           // Nút Medium trong difficulty menu
    bool isHardButtonClicked(Vector2 mousePos);             // Nút Hard trong difficulty menu
    bool isBackButtonClicked(Vector2 mousePos);             // Nút Back trong difficulty menu
    bool isSaveGameButtonClicked(Vector2 mousePos);         // Nút Save Game
    bool isLoadGameButtonClicked(Vector2 mousePos);         // Nút Load Game
    bool isMenuButtonClicked(Vector2 mousePos);             // Nút Menu trong game
    bool isConfirmMainMenuClicked(Vector2 mousePos);        // Nút "MAIN MENU" trong dialog
    bool isCloseDialogClicked(Vector2 mousePos);            // Nút "CLOSE" trong dialog

    void beginDrawing();
    void endDrawing();

    bool shouldClose();

    // Reset về menu chính (không phải difficulty menu)
    void resetToMainMenu();

    // ========== SAVE/LOAD MANAGEMENT ==========
    void loadSavedGamesList();                              // Load danh sách game đã lưu từ folder
    std::string getSelectedSaveFile();                      // Trả về file được chọn trong load popup
    std::string getSaveGameName();                          // Trả về tên game từ save popup
    void handleTextInput(int key);                          // Xử lý text input cho save name
    int getSaveGameNameLength() const { return saveGameNameLength; }  // Debug helper
    const char* getSaveGameNameBuffer() const { return saveGameNameBuffer; }  // Debug helper
    bool showSavePopup;// Hiển thị popup save
    bool showLoadPopup;// Hiển thị popup load
    int selectedSaveIndex;// Index của save được chọn trong list
    bool saveRequested;// Flag để báo hiệu save được request
    bool loadRequested;// Flag để báo hiệu load được request

    // ========== MENU DIALOG STATE ==========
    bool showMenuConfirmDialog;                             // Hiển thị dialog xác nhận quay về menu
    bool hasUnsavedChanges;                                 // Có thay đổi chưa lưu hay không
    bool menuConfirmRequested;                              // Flag để báo hiệu user confirm quay về menu

    // ========== MENU STATE ==========
    bool showDifficultyMenu;                                // Hiển thị menu chọn độ khó
    bool showAINotification;                                // Hiển thị thông báo AI chưa sẵn sàng
    int notificationFrameCounter;                           // Đếm frame từ khi mở notification

private:
    //  UI CONSTANTS
    static const int SCREEN_WIDTH = 1400;
    static const int SCREEN_HEIGHT = 900;
    static const int CELL_SIZE = 40;
    static const int BOARD_OFFSET_X = 100;
    static const int BOARD_OFFSET_Y = 100;
    static const int STONE_RADIUS = 18;
    static const int BOARD_SIZE = 19;

    //BUTTON RECTANGLES
    Rectangle undoButton;
    Rectangle redoButton;
    Rectangle passButton;
    Rectangle newGameButton;
    Rectangle saveGameButton;
    Rectangle loadGameButton;
    Rectangle menuButton;           // Nút Menu trong game để về main menu
    Rectangle confirmMainMenuButton;  // Nút "MAIN MENU" trong dialog
    Rectangle closeDialogButton;      // Nút "CLOSE" trong dialog

    //MENU
    Texture2D menuTexture;
    Texture2D menuBlanksTexture;        // Background cho difficulty menu
    Texture2D buttonPlayerTexture;
    Texture2D buttonAITexture;
    Texture2D buttonSettingTexture;
    Texture2D buttonBlanksTexture;      // Texture cho các button blank
    Texture2D buttonBackTexture;        // Texture cho nút Back
    Rectangle playerVsPlayerButton;
    Rectangle playerVsAIButton;
    Rectangle playerVsSettingButton;

    // Difficulty Menu
    Rectangle easyButton;
    Rectangle mediumButton;
    Rectangle hardButton;
    Rectangle backButton;               // Nút Back trong difficulty menu

    //SAVE/LOAD STATE
    int loadPopupFrameCounter; // Đếm frame từ khi mở load popup
    char saveGameNameBuffer[100];// Buffer cho text input
    int saveGameNameLength;  // Độ dài tên hiện tại
    std::vector<std::string> savedGamesList;  // Danh sách file đã lưu
    std::vector<std::string> savedGamesTime;  // Danh sách thời gian của các file
    int scrollOffset;  // Offset cho scroll trong load popup
    std::string saveGameDirectory;  // Đường dẫn tới save_game folder (tự động phát hiện)

    // COLORS
    Color boardColor;
    Color lineColor;
    Color starPointColor;
    Color blackStoneColor;
    Color whiteStoneColor;
    Color hoverColor;

    //HELPER FUNCTIONS
    void drawGridLines();
    void drawStarPoints();       // Vẽ các điểm sao (hoshi) trên bàn cờ
    void drawCoordinates();      // Vẽ tọa độ A-T, 1-19
    void initButtons();          // Khởi tạo vị trí các nút
    void initMenuButtons();      // Khởi tạo vị trí nút menu
    void drawTextWithOutline(const char* text, int x, int y, int fontSize, Color textColor, Color outlineColor, int outlineThickness);  // Vẽ text có viền
    bool isInsideBoard(int row, int col);  // Kiểm tra tọa độ có trong bàn cờ không
    std::string detectSaveGameDirectory();  // Tự động phát hiện đường dẫn save_game
};

#endif // UI_H_INCLUDED
