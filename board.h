#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
//#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <cstring>
#include <fstream>
#include <string>

    struct NumberAndTypeOfArea {
    int cnt;
    bool TouchedBlack;
    bool TouchedWhite;
};
class Board {
public:
    Board();
    bool PlaceStone(int row, int col);
    void PrintBoard();
    bool Capture(int row, int col);
    std::pair<int,int> CountArea() const ;
    std::vector<std::pair<int,int>> AllValidMove() const;
    // cham diem the co
    int evaluateBoard(char AI_player)const ;
    void Undo_Board();
    void Redo_Board();
    Board& operator=(const Board& other);
    bool SaveGame(std::string filename);
    bool LoadGame(std::string filename);
private:
    char board[20][20];
    std::pair<int,std::vector<std::pair<int,int>>> bfs (int row, int col, char Stone);
    NumberAndTypeOfArea BfsArea(int row, int col, bool VisitedLiberties[20][20])const;
    static const int step_i[4];
    static const int step_j[4];
    int BlackStoneCapture;
    int WhiteStoneCapture;
    char player;
    std::stack<Board> undo;
    std::stack<Board>redo;
};
#endif // BOARD_H_INCLUDED
