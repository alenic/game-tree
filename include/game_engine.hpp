#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <vector>

using namespace std;

// =============================== Game State ==========================
struct GameState {
  bool done;
  
  GameState() {
    done = false;
  }
  // Allocate new clone object and return the pointer
  virtual GameState *clone() = 0;
  // Default destructor
  virtual ~GameState() {}
};


// =============================== Game Action ==========================
struct GameAction {
  // Default destructor
  virtual ~GameAction() {}
};

struct IntGameAction : public GameAction {
  int value;

  IntGameAction() { value=-1; }
  IntGameAction(int valueIn) { value=valueIn; }
};


// =============================== Game Engine ==========================
struct GameEngine {
  GameEngine() {}

  void setActionMap(const vector<GameAction *> &actionMapIn) {
    actionMap = actionMapIn;
  }

  // Set Integer Action Map (from 0 to numActions-1)
  void setIntActionMap(int numActions) {
    actionMap.clear();

    for(int i=0; i<numActions; i++) {
      actionMap.push_back(new IntGameAction(i));
    }
  }

  /* 
  Get the next state from state 
  ! action should be a valid action (check)
  ! set terminal state
  ! set value for heuristics (0 otherwise)
  */
  virtual void getNextStateRewardDone(const GameState *state, const GameAction *action, GameState *nextState, double &reward) = 0;

  // state should be non terminal
  virtual vector<unsigned int> *getValidActionsIds(const GameState *state) = 0;

  // return number fo actions
  int getActionNum() {
    return (int)actionMap.size();
  }

  GameAction *getActionFromId(unsigned int actionId) {
    return actionMap[actionId];
  }

  private:
    vector<GameAction *> actionMap;

};

#endif  // GAME_ENGINE_H