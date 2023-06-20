#pragma once
#include "../state/state.hpp"
#include <vector>
#include <cmath>
#include <list>

/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */

const int INF = 2e9;
const int MAX_ITERATION = 200000;
const int MAX_DEPTH = 12;

struct Node{
    float sum, n;
    Move move;
    std::list<Node *> Child;
};

int findValue(int player);
Node * createNode(Move move);
float calculate(Node * Cur, float pval);

class MCTS{
public:
    static Move get_move(State *state, int depth);
    static void doIteration(Node * Cur, int depth, State * state);
    static void RollOut(State * state, int depth);
};