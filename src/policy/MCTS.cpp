#include <cstdlib>
#include <cmath>
#include <iostream>

#include "../state/state.hpp"
#include "./MCTS.hpp"


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

// ========================================================
// ============ MONTE CARLO TREE SEARCH ===================
// ========================================================

int playerID;
float RollOutResult; // 1 MEANS WHITE WIN, -1 MEANS BLACK WIN

Move MCTS::get_move(State *state, int depth){
    Move tt;
    Node * Root = createNode(tt);
    playerID = state->player;
    depth = MAX_DEPTH;

    Node * select = NULL;
    for(int i=0; i<MAX_ITERATION; i++){
        State * copy = new State(state);
        doIteration(Root, depth, copy);
    }

    for(Node * child : Root->Child){
        if(select == NULL){
                select = child;
        }
        else{
            if(calculate(child, Root->n) > calculate(select, Root->n)){
                select = child;
            }
        }
        // std::cout << "MOVE OF "
        //             << child->move.first.first << " " << child->move.first.second << " "
        //             << child->move.second.first << " " << child->move.second.second << "==>";
        // std::cout << calculate(child, Root->n) << std::endl;
    }
    return select->move;
}

void MCTS::doIteration(Node * Cur, int depth, State * state){

    if(state->game_state == WIN){
        if(state->player == 0)
            RollOutResult = 1;
        else 
            RollOutResult = -1;

        Cur->sum += findValue(1 - state->player);
        Cur->n++;
        return;
    }
    if(state->game_state == DRAW){
        Cur->n++;
        RollOutResult = 0;
        return;
    }

    if(depth <= 0){
        RollOutResult = state->evaluate();
        Cur->sum += findValue(1 - state->player);
        Cur->n++;
        return;
    }

    if(Cur->Child.empty()){
        if(!state->legal_actions.size())
            state->get_legal_actions();
        
        for(auto legal : state->legal_actions){
            Node * temp = createNode(legal);
            Cur->Child.push_back(temp);
        }
    }

    if(Cur->Child.empty()){
        RollOutResult = state->evaluate();
        Cur->sum += findValue(1 - state->player);
        Cur->n++;
        return;
    }

    //SEARCH MAXIMUM VALUE OF CHILDREN
    Node * search = NULL;
    for(Node * child : Cur->Child){
        if(child->n == 0){
            //HAVENT BEEN VISITED SO DO A ROLL OUT HERE
            State * copy = new State(state);
            RollOut(copy, depth);
            Cur->n++;
            child->n++;
            child->sum += findValue(state->player);
            Cur->sum += findValue(1 - state->player);
            return;
        }
        else if(search == NULL){
            search = child;
        }
        else{
            if(calculate(child, Cur->n) > calculate(search, Cur->n)){
                search = child;
            }
        }
    }

    doIteration(search, depth, state->next_state(search->move));
    Cur->sum += findValue(1 - state->player);
    Cur->n++;
}

void MCTS::RollOut(State * state, int depth){
    if(state->game_state == WIN){
        if(state->player == 0)
            RollOutResult = 1;
        else 
            RollOutResult = -1;
    }
    else if(state->game_state == DRAW){ 
        RollOutResult = 0;
    }
    else if(depth <= 0){
        RollOutResult = state->endEvaluate();
    }
    else{
        if(!state->legal_actions.size())
            state->get_legal_actions();

        auto actions = state->legal_actions;

        if(!state->legal_actions.size()){
            RollOutResult = state->endEvaluate();
        }
        else{
            auto legal = actions[(rand()+depth)%actions.size()];
            RollOut(state->next_state(legal), depth - 1);
        }
    }

    delete state;
}


// ========================================================
// ===================== NODE FUNCTION ====================
// ========================================================

int findValue(int player){
    if(RollOutResult == 0) return 0;
    if(player == 0){
        if(RollOutResult == 1) return 1;
        else return -1;
    }
    else{
        if(RollOutResult == 1) return -1;
        else return 1;
    }
}

Node * createNode(Move move){
    Node * ret = new Node;
    ret->n = ret->sum = 0;
    ret->move = move;

    return ret;
}

float calculate(Node * Cur, float pval){
    if(Cur->n == 0 || pval == 0) return INF;
    // To avoid Floating precision error (On comparing part)
    // We make the value as integer by multiply for large number and round it
    float avg = Cur->sum / Cur->n;
    float pt = sqrt(2 * log(pval) / Cur->n);
    float ans = avg + pt;
    return ans;
}