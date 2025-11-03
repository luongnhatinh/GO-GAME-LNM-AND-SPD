#include <iostream>
#include "board.h"
#include "AI.h"
#include <bits/stdc++.h>
using namespace std;
stack<pair<int,int>> Undo, Redo;
int main() {
    /*
    int turn=0;
    char c[2]={'B','W'};
    Board game;
    AI ai;
    while(true) {
        int i,j; cin>>i>>j;
        if(game.PlaceStone(i,j,c[turn])) {
            turn+=2;
            turn=turn%2;
        }
        else cout<<"Dat lai"<<endl;
        game.PrintBoard();
        cout<<"----------------------------------------------"<<"\n";
        pair<int,int> Move=ai.FindBestMove();
        if(i==0&&j==0) break;
    }  */
    Board game;
    AI ai;
    while(true) {
        int i,j; cin>>i>>j;
        if(!game.PlaceStone(i,j,'B')) cout<<"Dat lai"<<"\n";
        game.PrintBoard();
        cout<<"----------------------------------------------"<<"\n";
        pair<int,int> Move=ai.FindBestMove(game,'W',Difficulty::Easy);
        game.PlaceStone(Move.first,Move.second,'W');
        game.PrintBoard();
        cout<<"----------------------------------------------"<<"\n";
    }
    return 0;
}
