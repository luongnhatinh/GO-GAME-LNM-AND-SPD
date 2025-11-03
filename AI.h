#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED
//#include<bits/stdc++.h>
#include "board.h"
enum class Difficulty{
    Easy,
    Medium,
    Hard,
};
class AI {
public :
    AI();
    std::pair<int,int> FindBestMove(const Board& board, char AI_player,Difficulty level);
private:

};

#endif // AI_H_INCLUDED
