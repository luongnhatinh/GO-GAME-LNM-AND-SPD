#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
//#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <utility>
    struct NumberAndTypeOfArea {
    int cnt;
    bool TouchedBlack;
    bool TouchedWhite;
};
class Board {
public:
    Board();
    bool PlaceStone(int row, int col, char player);
    void PrintBoard();
    bool Capture(int row, int col, char player);
    std::pair<int,int> CountArea();
    std::vector<std::pair<int,int>> AllValidMove() const;
    // cham diem the co
    int evaluateBoard(const Board& board, char AI_player);
private:
    char board[20][20];
    std::pair<int,std::vector<std::pair<int,int>>> bfs (int row, int col, char Stone);
    NumberAndTypeOfArea BfsArea(int row, int col, bool VisitedLiberties[20][20]);
    static const int step_i[4];
    static const int step_j[4];
    int BlackStoneCapture;
    int WhiteStoneCapture;
};
#endif // BOARD_H_INCLUDED

