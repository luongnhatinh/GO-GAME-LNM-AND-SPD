#include "AI.h"
#include "board.h"
//#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>

using namespace std;
AI::AI() {

}
std::pair<int,int> AI::FindBestMove(const Board&  board,char AI_player,Difficulty level) {
    switch(level) {
        case Difficulty::Easy:
        {
            std::vector<std::pair<int,int>> ValidMove=board.AllValidMove();
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, ValidMove.size() - 1);
            int random_index = distrib(gen);
            return ValidMove[random_index];
            break;
        }
        case Difficulty::Medium: {
            int CurrentPoint=board.evaluateBoard(AI_player);
            break;
        }
        case Difficulty::Hard: {
            break;
        }
        default: break;
    }
}

