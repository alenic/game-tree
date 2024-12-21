#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <iostream>
#include <vector>

using namespace std;

namespace gametree {

// =============================== Game State ==========================
class GlobalState {
  public:
    // Default destructor
    virtual ~GlobalState() {}

    // Print shared state
    friend ostream &operator<<(ostream &ostr, GlobalState &state) {
      state.print(ostr);
      return ostr;
    }

  protected:
    // Override the print function to customize your shared state string
    virtual void print(ostream &ostr) {
      ostr << this;
    }
};

class State {
  public:
    // Default destructor
    virtual ~State() {}

    // Print state
    friend ostream &operator<<(ostream &ostr, State &state) {
      state.print(ostr);
      return ostr;
    }

  protected:
    // Override the print function to customize your state string
    virtual void print(ostream &ostr) {
      ostr << this;
    }
};


// =============================== Game Action ==========================
class Action {
  public:
    // Default destructor
    virtual ~Action() {}

    // Print action
    friend ostream &operator<<(ostream &ostr, Action &action) {
      action.print(ostr);
      return ostr;
    }

  protected:
    // Override the print function to customize your action string
    virtual void print(ostream &ostr) {
      ostr << this;
    }
};

class ActionSpace {
  public:

  ActionSpace() {}

  ActionSpace(vector<Action *> actionMap) {
    this->actionMap = actionMap;
  }

  void addAction(Action *action) {
    actionMap.push_back(action);
  }

  Action *getAction(unsigned int actionId) {
    return actionMap[actionId];
  }

  protected:
    vector<Action *> actionMap;
};


// =============================== Game Environment ==========================
struct GameEnvironment {
  GameEnvironment() {}

  // Define the transition function
  virtual void transitionFunction(const State *state,         // state input
                                  const Action *action,       // action input
                                  GlobalState *globalState,   // shared state input/output (update globally)
                                  State *nextState,           // nrext state output
                                  double &reward,             // reward output
                                  bool &done                  // true if the game ended
                                  ) = 0;

  // Called only if a shared state is used
  virtual void restoreGlobalState(const State *state,
                                  const Action *choosedAction,
                                  const State *nextState,
                                  GlobalState *globalState
                                  ) {};

  // Gett all valid actions from a state
  virtual vector<unsigned int> *getValidActionsIds(const GlobalState *globalState,
                                                   const State *state,
                                                   const ActionSpace *actionSpace
                                                   ) = 0;
};

};  // namespace gametree

#endif  // GAME_ENGINE_H