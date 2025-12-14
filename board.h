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
    Board(const Board& other);  // Copy constructor
    bool PlaceStone(int row, int col);
    void PrintBoard();
    bool Capture(int row, int col);
    std::pair<int,int> CountArea() const ;
    std::vector<std::pair<int,int>> AllValidMove() const;
    std::vector<std::pair<int,int>> quickMove() const;
    // cham diem the co
    int evaluateBoard(char AI_player)const ;
    int quickeval(char AI_player) const;
    void Undo_Board();
    void Redo_Board();
    Board& operator=(const Board& other);
    char getCell(int row, int col) const {
        return board[row][col];
    }
    int getBlackCapture() const {
        return BlackStoneCapture;
    }
    int getWhiteCapture() const {
        return WhiteStoneCapture;
    }
    char getCurrentPlayer() const {
        return player;
    }
    void switchPlayer() {
        player = (player == 'B') ? 'W' : 'B';
    }
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
    int rateMoveSmart(int r, int c) const;
    char player;
    std::stack<Board> undo;
    std::stack<Board>redo;
};
#endif
