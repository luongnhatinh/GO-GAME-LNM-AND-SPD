#include "AI.h"
#include "board.h"
//#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>
#include <algorithm>
#include <chrono>
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
                return {-1,-1};
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
                return {-1,-1};
            }
            int depth=2;
            if(Move.size()>20) {
                depth=2;
            }
            else if(Move.size()<5) {
                depth++;
            }
            int Maxpoint=-100000;
            pair<int,int> BestMove={-1,-1};
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
            std::vector<std::pair<int, int>> Move = board.quickMove();
            if (Move.empty()) return {-1, -1};

            // Nếu chỉ có 1 nước đi duy nhất, đánh luôn khỏi tính
            if (Move.size() == 1) return Move[0];

            long long timeLimitMs = 900;
            if (level == Difficulty::Medium) timeLimitMs = 500;
            if (level == Difficulty::Easy) timeLimitMs = 10;

            // Bắt đầu bấm giờ
            auto startTime = std::chrono::high_resolution_clock::now();

            std::pair<int, int> bestMoveSoFar = Move[0]; // Lưu nước đi tốt nhất tìm được
            int maxDepth = (level == Difficulty::Hard) ? 5 : 2;
            for (int currentDepth = 1; currentDepth <= maxDepth; currentDepth++) {

                int alpha = -2000000000;
                int beta = 2000000000;
                int maxVal = -2000000000;
                std::pair<int, int> currentDepthBestMove = {-1, -1};
                Board tempBoard = board;

                for (auto p : Move) {
                    if (tempBoard.PlaceStone(p.first, p.second)) {
                        int val = alpha_beta(tempBoard, currentDepth - 1, alpha, beta, false, AI_player);
                        tempBoard.Undo_Board();

                        if (val > maxVal) {
                            maxVal = val;
                            currentDepthBestMove = p;
                        }
                        if (maxVal > alpha) {
                            alpha = maxVal;
                        }
                    }
                    auto now = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
                    if (duration > timeLimitMs) {
                        std::cout << "Time out at Depth " << currentDepth << "!" << std::endl;
                        return bestMoveSoFar;
                    }
                }
                if (currentDepthBestMove.first != -1) {
                    bestMoveSoFar = currentDepthBestMove;
                    std::cout << "Finished Depth " << currentDepth << " => Best: " << bestMoveSoFar.first << "," << bestMoveSoFar.second << std::endl;
                }

                //Đưa nước đi tốt nhất vừa tìm được lên đầu danh sách Move
                for (int i=0; i<Move.size(); i++) {
                    if (Move[i] == bestMoveSoFar) {
                        std::swap(Move[0], Move[i]);
                        break;
                    }
                }
            }

            return bestMoveSoFar;
        }
        default: break;
    }
}

