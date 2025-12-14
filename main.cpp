#include <iostream>
#include "board.h"
#include "Game.h"
#include "AI.h"
#include <vector>
#include <utility>
#include <stack>
#include <cstring>
#include <queue>
#include <random>
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
void SetupMidGame(Board& board, int stonesToPlace) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 19);

    int count = 0;
    while(count < stonesToPlace) {
        int r = distrib(gen);
        int c = distrib(gen);
        // Chỉ đặt nếu ô trống và hợp lệ
        if(board.getCell(r, c) == 'O') {
            if(board.PlaceStone(r, c)) {
                count++;
                // Không cần switch player vì PlaceStone đã tự switch
            }
        }
    }
}

void AdvancedBenchmark() {
    AI ai;
    Board board;

    cout << "\n=======================================================" << endl;
    cout << "   AI PERFORMANCE BENCHMARK (With QuickMove & QuickEval)" << endl;
    cout << "=======================================================\n" << endl;

    // --- TEST 1: KHAI CUỘC (OPENING) ---
    cout << ">>> TEST 1: KHAI CUOC (Empty Board)" << endl;
    cout << "Muc tieu: Kiem tra toc do chon diem Sao (Star Points)." << endl;

    auto start1 = high_resolution_clock::now();

    // AI cầm Trắng, mức Khó
    pair<int, int> move1 = ai.FindBestMove(board, 'W', Difficulty::Hard);

    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(stop1 - start1);

    cout << "-> AI Move: (" << move1.first << ", " << move1.second << ")" << endl;
    cout << "-> Thoi gian: " << duration1.count() << " ms" << endl;
    if (duration1.count() < 10) cout << "=> DANH GIA: SIÊU NHANH (Instant)" << endl;
    else cout << "=> DANH GIA: OK" << endl;
    cout << "-------------------------------------------------------\n" << endl;


    // --- TEST 2: TRUNG CUỘC (MID-GAME STRESS TEST) ---
    cout << ">>> TEST 2: TRUNG CUOC (Heavy Load)" << endl;
    cout << "Gia lap: Dat 50 quan co ngau nhien len ban." << endl;
    cout << "Muc tieu: Test suc chiu dung cua Alpha-Beta voi Neighbor Pruning." << endl;

    // Reset bàn cờ và rải quân
    board = Board();
    SetupMidGame(board, 50); // Giả lập 50 nước đi ngẫu nhiên

    cout << "Trang thai ban co: Da co 50 quan." << endl;
    cout << "Dang suy nghi..." << endl;

    auto start2 = high_resolution_clock::now();

    // AI cầm Trắng (hoặc Đen tùy lượt hiện tại), mức Khó
    char currentTurn = board.getCurrentPlayer();
    pair<int, int> move2 = ai.FindBestMove(board, currentTurn, Difficulty::Hard);

    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(stop2 - start2);

    cout << "-> AI Move: (" << move2.first << ", " << move2.second << ")" << endl;
    cout << "-> Thoi gian: " << duration2.count() << " ms (" << duration2.count()/1000.0 << "s)" << endl;

    // Đánh giá dựa trên tiêu chuẩn thực tế
    if (duration2.count() < 500)
        cout << "=> DANH GIA: XUAT SAC (< 0.5s)" << endl;
    else if (duration2.count() < 2000)
        cout << "=> DANH GIA: TOT (< 2s)" << endl;
    else
        cout << "=> DANH GIA: HOI CHAM (> 2s)" << endl;

    cout << "=======================================================" << endl;
}

int main() {
    //TestAIPerformance();
    Game game;
    game.run();
//    AdvancedBenchmark();
    return 0;
}
