
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <vector>

#include "game_state.h"

class GameEngine {
 private:
  vector<int> mActionSet;

 public:
  GameEngine() {}

  // Get the next state from current state (set terminal state also)
  virtual void getNextStateAndReward(const GameState &state, int action,
                                     GameState *nextState, double &reward) = 0;

  // state should be non terminal
  virtual bool isValidAction(const GameState &state, int action) = 0;

  void setActionSet(const vector<int> &actionSet) { mActionSet = actionSet; }

  void setActionSet(int numActions) {
    for (int i = 0; i < numActions; i++) {
      mActionSet.push_back(i);
    }
  }

  int getActionNum() { return mActionSet.size(); }

  virtual void getValidActions(GameState *state, vector<int> &validAction) {
    if (state->isTerminal()) {
      return;
    }
    for (int i = 0; i < mActionSet.size(); i++) {
      if (isValidAction(*state, mActionSet[i])) {
        validAction.push_back(mActionSet[i]);
      }
    }
  }
};

#endif  // GAME_ENGINE_H