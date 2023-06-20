#pragma once
#include "../state/state.hpp"


/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
class MinMax{
public:
  static Move get_move(State *state, int depth);
  static int AlphaBeta(State *state, int depth, int alpha, int beta, bool maximizePlayer);

  static int valMax(int a, int b);
  static int valMin(int a, int b); 
};