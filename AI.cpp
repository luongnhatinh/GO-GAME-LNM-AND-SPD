//#include "AI.h"
//#include "board.h"
////#include <bits/stdc++.h>
//#include <cstdlib>
//#include <ctime>
//#include <random>
//#include <utility>
//#include <algorithm>
//using namespace std;
//AI::AI() {
//
//}
//int AI::minimax(Board board,int depth, bool isAI, char AIplayer) {
//    std::vector<std::pair<int,int>> Move=board.AllValidMove();
//    if(depth==0 || Move.empty()) {
//        return board.evaluateBoard(AIplayer);
//    }
//    char opponent=(AIplayer=='B') ? 'W':'B';
//    if(isAI) {
//        int maxpoint=-100000;
//        //We'll try to maximize AI's points in this session
//        for(pair<int,int> x: Move) {
//            Board tempBoard=board;
//            if(tempBoard.PlaceStone(x.first,x.second)) {
//                int movevalue=minimax(tempBoard,depth-1,false,AIplayer);
//                maxpoint=max(maxpoint,movevalue);
//            }
//        }
//        return maxpoint;
//
//    }
//    else {
//        int minpoint=100000;
//        for(pair<int,int> x:Move) {
//            Board tempBoard=board;
//            tempBoard.PlaceStone(x.first,x.second);
//            int movevalue =minimax(tempBoard,depth-1,true,AIplayer);
//            minpoint=min(minpoint,movevalue);
//        }
//        return minpoint;
//    }
//}
//int AI::alpha_beta(Board board, int depth, int alpha, int beta, isAI, char AIplayer) {
//    vector<pair<int,int>> Move=board.AllValidMove();=
//    if(depth==0 || Move.empty()) {
//        return board.evaluateBoard(AIplayer);
//    }
//    char opponent = (AIplayer=='B')? 'W':'B';
//    if(isAI) {
//        int maxpoint=-1000000;
//        for(pair<int,int> x: Move) {
//            Board tempboard=board;
//            tempboard.PlaceStone(x.first,x.second,AIplayer);
//            value=max(value,alpha_beta(tempboard, depth-1, alpha))
//        }
//    }
//}
//std::pair<int,int> AI::FindBestMove(const Board&  board,char AI_player,Difficulty level) {
//    switch(level) {
//        case Difficulty::Easy:
//        {
//            std::vector<std::pair<int,int>> ValidMove=board.AllValidMove();
//            std::random_device rd;
//            std::mt19937 gen(rd());
//            std::uniform_int_distribution<> distrib(0, ValidMove.size() - 1);
//            int random_index = distrib(gen);
//            return ValidMove[random_index];
//        }
//        case Difficulty::Medium: {
//            vector<pair<int,int>> Move=board.AllValidMove();
//            if(Move.empty()) {
//                return {0,0};
//            }
//            int Maxpoint=-100000;
//            pair<int,int> BestMove=Move[0];
//            for (pair<int,int> x:Move) {
//                Board tempboard = board;
//                if(tempboard.PlaceStone(x.first,x.second)) {
//                    int PointOfCurrentMove = minimax(tempboard,2,false,AI_player);
//                    if(PointOfCurrentMove>Maxpoint) {
//                        BestMove=x;
//                        Maxpoint=PointOfCurrentMove;
//                    }
//                }
//            }
//            return BestMove;
//        }
//        case Difficulty::Hard: {
//            break;
//        }
//        default: break;
//    }
//}
//
