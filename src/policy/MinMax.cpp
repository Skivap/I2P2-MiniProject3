#include <cstdlib>
#include <iostream>

#include "../state/state.hpp"
#include "./MinMax.hpp"

const int INF = 2e9;

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move MinMax::get_move(State *state, int depth){
  //Starting Alpha as -Infinity and Beta as Infinity
  //For white we want to maximize value
  //For black we do the opposite

  if(!state->legal_actions.size())
    state->get_legal_actions();
  Move retval = state->legal_actions[0];
  int curval;
  
  if(state->player == 0){
    curval = -INF;
    for(auto legal : state->legal_actions){
      int tmp;
      tmp = AlphaBeta(state->next_state(legal), depth - 1, -INF, INF, false);
      //std::cout << "=== TESTING : " << legal.first.first << " " << legal.first.second << " " << legal.second.first << " " << legal.second.second << " Value : "<< tmp << std::endl;
      if(tmp > curval){
        curval = tmp;
        retval = legal;
      }
    }
  }
  else{
    curval = INF;
    for(auto legal : state->legal_actions){
      int tmp;
      tmp = AlphaBeta(state->next_state(legal), depth - 1, -INF, INF, true);
      //std::cout << "=== TESTING : " << legal.first.first << " " << legal.first.second << " " << legal.second.first << " " << legal.second.second << " Value : "<< tmp << std::endl;
      if(tmp < curval){
        curval = tmp;
        retval = legal;
      }
    }
  }
  return retval;
}

int MinMax::AlphaBeta(State *state, int depth, int alpha, int beta, bool maximizePlayer){
  if(depth == 0){
    //If depth = 0 then return the heuristic value
    int retval = state->evaluate();
    delete state;
    return retval;
  }
  else{
    int value;
    if(!state->legal_actions.size())
      state->get_legal_actions();

    if(!state->legal_actions.size()){
      return state->evaluate();
    }

    if(state->game_state == WIN){
      return maximizePlayer ? INF : -INF;
    }

    if(maximizePlayer){
      value = -INF;
      for(auto legal : state->legal_actions){
        value = valMax(value, AlphaBeta(state->next_state(legal), depth - 1, alpha, beta, false));
        alpha = valMax(alpha, value);
        if(alpha >= beta)
          break;
      }
    }
    else{
      value = INF;
      for(auto legal : state->legal_actions){
        value = valMin(value, AlphaBeta(state->next_state(legal), depth - 1, alpha, beta, true));
        beta = valMin(beta, value);
        if(alpha >= beta)
          break;
      }
    }
    delete state;
    return value;
  }
}

int MinMax::valMax(int a, int b){
  if(a > b) return a;
  return b;
}

int MinMax::valMin(int a, int b){
  if(a > b) return b;
  return a;
}