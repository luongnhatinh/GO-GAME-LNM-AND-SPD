#include "AI.h"
#include "board.h"
//#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>
#include <algorithm>
using namespace std;
AI::AI() {
}
int AI::minimax(Board& board,int depth, bool isAI, char AIplayer) {
    std::vector<std::pair<int,int>> Move=board.quickMove();
    if(depth==0 || Move.empty()) {
        return board.quickeval(AIplayer);
    }
    if(isAI) {
        int maxpoint=-100000;
        //We'll try to maximize AI's points in this session
        for(pair<int,int> x: Move) {
            if(board.PlaceStone(x.first,x.second)) {
                int movevalue=minimax(board,depth-1,false,AIplayer);
                maxpoint=max(maxpoint,movevalue);
                board.Undo_Board();
            }
        }
        return maxpoint;

    }
    else {
        int minpoint=100000;
        for(pair<int,int> x:Move) {
            if(board.PlaceStone(x.first,x.second)) {
                int movevalue =minimax(board,depth-1,true,AIplayer);
                minpoint=min(minpoint,movevalue);
                board.Undo_Board();
            }
        }
        return minpoint;
    }
}
int AI::alpha_beta(Board& board, int depth, int alpha, int beta,bool isAI, char AIplayer) {
    vector<pair<int,int>> Move=board.quickMove();
    if(depth==0 || Move.empty()) {
        return board.quickeval(AIplayer);
    }
    if(isAI) {
        int value=-100000;
        for(pair<int,int> x: Move) {
            if (board.PlaceStone(x.first,x.second)) {
                value=max(value,alpha_beta(board, depth-1, alpha, beta, false, AIplayer));
                alpha = max(alpha,value);
                board.Undo_Board();
                if(alpha>=beta) {
                    break;
                }
            }
        }
        return value;
    }
    else {
        int value = 100000;
        for(pair<int,int> x:Move) {
            if(board.PlaceStone(x.first,x.second)) {
                value=min(value,alpha_beta(board,depth-1,alpha,beta,true,AIplayer));
                beta=min(beta,value);
                board.Undo_Board();
                if(beta<=alpha) {
                    break;
                }
            }
        }
        return value;
    }
}
std::pair<int,int> AI::FindBestMove(const Board&  board,char AI_player,Difficulty level) {
    switch(level) {
        case Difficulty::Easy:
        {
            std::vector<std::pair<int,int>> ValidMove=board.AllValidMove();
            if(ValidMove.empty()) {
                return {0,0};
            }
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, ValidMove.size() - 1);
            int random_index = distrib(gen);
            return ValidMove[random_index];
        }
        case Difficulty::Medium: {
            vector<pair<int,int>> Move=board.quickMove();
            if(Move.empty()) {
                return {0,0};
            }
            int depth=2;
            int Maxpoint=-100000;
            pair<int,int> BestMove={0,0};
            Board tempboard = board;
            for (pair<int,int> x:Move) {
                if(tempboard.PlaceStone(x.first,x.second)) {
                    int PointOfCurrentMove = minimax(tempboard,depth,false,AI_player);
                    tempboard.Undo_Board();
                    if(PointOfCurrentMove>Maxpoint) {
                        BestMove=x;
                        Maxpoint=PointOfCurrentMove;
                    }
                }
            }
            return BestMove;
        }
        case Difficulty::Hard: {
            vector<pair<int,int>> Move=board.quickMove();
            if(Move.empty()) {
                return {0,0};
            }
            int depth=3;
            if(Move.size()>20) {
                depth--;
            }
            int Maxpoint=-100000;
            Board tempboard=board;
            pair<int,int> BestMove={0,0};
            for(pair<int,int> x: Move) {
                if(tempboard.PlaceStone(x.first,x.second)) {
                    int PointOfCurrentMove = alpha_beta(tempboard,depth,-100000,+100000,false,AI_player);
                    tempboard.Undo_Board();
                    if(PointOfCurrentMove>Maxpoint) {
                        BestMove=x;
                        Maxpoint=PointOfCurrentMove;
                    }
                }
            }
            return BestMove;
            break;
        }
        default: break;
    }
}

