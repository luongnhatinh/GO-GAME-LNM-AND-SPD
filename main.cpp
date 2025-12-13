#include <iostream>
#include "board.h"
#include "Game.h"
#include "AI.h"
#include <vector>
#include <utility>
#include <stack>
#include <cstring>
#include <queue>
#include <chrono> // Thư viện đo thời gian

using namespace std;
using namespace std::chrono;

void TestAIPerformance() {
    ios_base::sync_with_stdio(0);
    Board board; // Bàn cờ trống 19x19
    AI ai;

    cout << "=== BAT DAU TEST TOC DO AI (HARD - DEPTH 3) ===" << endl;
    cout << "Dang suy nghi..." << endl;

    // Bắt đầu bấm giờ
    auto start = high_resolution_clock::now();

    // Gọi AI (Giả sử AI cầm Trắng đi hậu)
    // Lưu ý: Trường hợp xấu nhất là bàn cờ trống, độ phân nhánh là 361
    pair<int, int> bestMove = ai.FindBestMove(board, 'W', Difficulty::Hard);

    // Dừng bấm giờ
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "AI chon nuoc di: " << bestMove.first << ", " << bestMove.second << endl;
    cout << "Thoi gian xu ly: " << duration.count() << " ms (" << duration.count() / 1000.0 << " giay)" << endl;
    cout << "=============================================" << endl;
}

int main() {
    //TestAIPerformance();
    Game game;
    game.run();
    return 0;
}
