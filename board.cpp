#include <iostream>
//#include <bits/stdc++.h>
#include "board.h"
#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <cstring>
#include <queue>
#include <string>
#include <fstream>
using namespace std;
const int Board::step_i[4]={-1,0,1,0};
const int Board::step_j[4]={0,1,0,-1};

Board::Board() {
    for(int i=1;i<=19;i++) {
        for(int j=1;j<=19;j++) {
            board[i][j]='O';
        }
    }
    BlackStoneCapture=0;
    WhiteStoneCapture=0;
    player='B';
    undo.push(*this);
}

// Copy constructor - FIX MEMORY LEAK
// CHỈ copy game state, KHÔNG copy stack undo/redo để tránh đệ quy vô hạn
Board::Board(const Board& other) {
    memcpy(this->board, other.board, sizeof(this->board));
    this->BlackStoneCapture = other.BlackStoneCapture;
    this->WhiteStoneCapture = other.WhiteStoneCapture;
    this->player = other.player;  // Copy player
    // KHÔNG copy undo và redo
    // Stack undo/redo của object mới sẽ TRỐNG
}
std::vector<std::pair<int,int>> Board::AllValidMove() const {
    std::vector<std::pair<int,int>>v;
    for(int i=1;i<=19;i++) {
        for(int j=1;j<=19;j++) {
            if(board[i][j]=='O') {
               v.push_back({i,j});
            }
        }
    }
    return v;
}
void Board::PrintBoard() {
    for(int i=1;i<=19;i++) {
        for(int j=1;j<=19;j++) {
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
}
bool Board::PlaceStone(int row, int col) {
    if(board[row][col]=='O') {
        board[row][col]=player;
        bool result=Capture(row,col);
        undo.push(*this);
        //kiem tra neu khong bat duoc quan va do la nuoc di tu sat
        if (!result && bfs(row,col,player).first==0) {
            *this = undo.top();
            undo.pop();
            return false;
        }
        while(!redo.empty()) redo.pop();
        player= (player=='B')? 'W' : 'B';
        return true;
    }
    else {
        return false;
    }
}
bool Board::Capture(int row, int col) {
    //Duyệt qua các đinh kề của điểm vừa đặt
    //Nếu có 1 quân cờ khác màu, kiểm tra coi nhóm quân đó có bị ăn bởi nước đi vừa rồi không
    //Đếm số khí của nhóm quân đó và vecotr chứa các phần tử của nhóm quân đó
    bool check=false; //check neu nuoc di do co bat duoc quan khong
    for(int k=0;k<4;k++) {
        int i1=row+step_i[k];
        int j1=col+step_j[k];
        if ((i1>0 && i1<=19) && (j1>0&&j1<=19) && board[i1][j1]!='O' && board[i1][j1]!=player) {
            std::pair<int,std::vector<std::pair<int,int>>> Kq_bfs;
            if(player=='B') {
                Kq_bfs=bfs(i1,j1,'W');
            }
            else {
                Kq_bfs=bfs(i1,j1,'B');
            }
            if (Kq_bfs.first==0) {
                check=true;
                // neu bat duoc quan thi cong them vao so quan da bat do
                if (player=='W') WhiteStoneCapture+=Kq_bfs.second.size();
                else BlackStoneCapture+=Kq_bfs.second.size();
                for(auto x:Kq_bfs.second) board[x.first][x.second]='O';
            }
        }
    }
    if (check) return true;
    return false;
}
// Dem so lanh tho cua tung nguoi choi
std::pair<int,int> Board::CountArea() const{
    int BlackPlayerArea=0;
    int WhitePlayerArea=0;
    bool VisitedLiberties[20][20]={false};
    for(int i=1;i<=19;i++) {
        for(int j=1;j<=19;j++) {
            // neu do la o trong va chua duoc tham
            if(!VisitedLiberties[i][j] && board[i][j]=='O') {
                NumberAndTypeOfArea Kq = BfsArea(i,j,VisitedLiberties);
                // tinh so lanh tho cho tung nguoi choi
                if(Kq.TouchedBlack == true && Kq.TouchedWhite==false) BlackPlayerArea+=Kq.cnt;
                else if (Kq.TouchedBlack==false && Kq.TouchedWhite==true) WhitePlayerArea+=Kq.cnt;
            }
        }
    }
    return {BlackPlayerArea,WhitePlayerArea};
}
int Board::evaluateBoard(char AI_player) const {
    std::pair<int,int> Area=CountArea();
    int BlackArea=Area.first;
    int WhiteArea=Area.second;
    return (AI_player=='B')? BlackArea+BlackStoneCapture : WhiteArea+WhiteStoneCapture;
}
NumberAndTypeOfArea Board::BfsArea(int row, int col,bool VisitedLiberties[20][20]) const{
    std::queue<std::pair<int,int>> q;
    q.push({row,col});
    bool TouchedBlack=false, TouchedWhite=false;
    VisitedLiberties[row][col]=true;
    int cnt=1;
    while(!q.empty()) {
        std::pair<int,int> u=q.front();
        q.pop();
        for(int k=0;k<4;k++) {
            int i1=u.first+step_i[k];
            int j1=u.second+step_j[k];
            if((i1>0&&i1<=19) && (j1>0&&j1<=19)) {
                if(board[i1][j1]=='O'&& VisitedLiberties[i1][j1]==false) {
                    VisitedLiberties[i1][j1]=true;
                    q.push({i1,j1});
                    cnt++;
                }
                else if (board[i1][j1]=='B') {
                    TouchedBlack=true;
                }
                else if (board[i1][j1]=='W') {  // ← FIX: Chỉ khi THỰC SỰ là quân trắng
                    TouchedWhite=true;
                }
            }
        }
    }
    return {cnt,TouchedBlack,TouchedWhite};
}
std::pair<int,std::vector<pair<int,int>>> Board::bfs(int row, int col, char Stone) {
    //bfs kiem tra coi co the bat nhom quan nay khong
    bool VistedStone[20][20]={false};
    bool VisitedLiberties[20][20]={false};
    std::vector<pair<int,int>> Save_node;
    int Cnt_Liberties=0;
    VistedStone[row][col]=true;
    queue<pair<int,int>> q;
    q.push({row,col});
    Save_node.push_back({row,col});
    while(!q.empty()) {
        pair<int,int> u=q.front();
        q.pop();
        for(int k=0;k<4;k++){
            int i1=u.first + step_i[k];
            int j1=u.second + step_j[k];
            if( (i1>0&&i1<=19) && (j1>0&&j1<=19) ) {
                // bfs tiep qua cac quan co
                if(board[i1][j1]==Stone && VistedStone[i1][j1]==false) {
                    VistedStone[i1][j1]=true;
                    Save_node.push_back({i1,j1});
                    q.push({i1,j1});
                }
                // kiểm tra và đếm các khí
                else if (board[i1][j1]=='O' && VisitedLiberties[i1][j1]==false) {
                    VisitedLiberties[i1][j1]=true;
                    Cnt_Liberties++;
                }
            }
        }
    }
    return {Cnt_Liberties,Save_node};
}
void Board::Undo_Board() {
    if(undo.empty()) {
        return;
    }
    redo.push(*this);
    *this=undo.top();
    undo.pop();
}
void Board::Redo_Board() {
    if(redo.empty()) {
        return;
    }
    undo.push(*this);
    *this=redo.top();
    redo.pop();
}
Board& Board::operator=(const Board& other) {
    memcpy(this -> board,other.board, sizeof(this->board));
    this->BlackStoneCapture=other.BlackStoneCapture;
    this->WhiteStoneCapture=other.WhiteStoneCapture;
    this->player=other.player;
    return *this;
}
bool Board::SaveGame(string filename) {
    ofstream fout;
    fout.open(filename);
    if(!fout.is_open()) {
        return false;
    }
    fout<<player<<endl;
    fout<<BlackStoneCapture<<" "<<WhiteStoneCapture<<endl;
    for(int i=1;i<=19;i++) {
        for(int j=1;j<=19;j++) {
            fout<<board[i][j];
        }
    }
    fout<<"\n";
    fout.close();
    return true;
}
bool Board::LoadGame(string filename) {
    ifstream fin;
    fin.open(filename);
    if(!fin.is_open()) {
        return false;
    }
    fin>>player;
    fin>>BlackStoneCapture>>WhiteStoneCapture;
    for(int i=1;i<=19;i++) {
        for(int j=1;j<=19;j++) {
            fin>>board[i][j];
        }
    }
    while(!undo.empty()) undo.pop();
    while(!redo.empty()) redo.pop();
    undo.push(*this);
    fin.close();
    return true;
}
