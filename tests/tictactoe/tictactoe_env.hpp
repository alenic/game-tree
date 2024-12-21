#include "game_environment.hpp"

using namespace gametree;

class TTTGlobalState : public GlobalState {
  public:
    int board[9];

    TTTGlobalState() {
        for (int i = 0; i < 9; i++) {
            board[i] = 0;
        }
    }
};


class TTTState : public State {
  public:
    int currentPlayer; // 1 or 2

    TTTState() {
        currentPlayer = 1;
    }
};

class TTTAction : public Action {
  public:
    int position;   // from 0 to 8

    TTTAction(int position) {
        this->position = position;
    }
};


class TTTEnvironment : public GameEnvironment {
public:

  int checkWin(int board[]) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == board[i * 3 + 1] && board[i * 3] == board[i * 3 + 2] && board[i * 3] != 0) {
            return board[i * 3];
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] == board[i + 3] && board[i] == board[i + 6] && board[i] != 0) {
            return board[i];
        }
    }

    // Check diagonals
    if (board[0] == board[4] && board[0] == board[8] && board[0] != 0) {
        return board[0];
    }

    if (board[2] == board[4] && board[2] == board[6] && board[2] != 0) {
        return board[2];
    }

    return 0;
  }

  // Define the transition function
  virtual void transitionFunction(const State *state,         // state input
                                  const Action *action,       // action input
                                  GlobalState *globalState,   // shared state input/output (update globally)
                                  State *nextState,           // nrext state output
                                  double &reward,             // reward output
                                  bool &done                  // true if the game ended
                                  ) 
  {
    const TTTState *tttState = dynamic_cast<const TTTState *>(state);
    
    TTTState *nextTTTState = new TTTState();
    ((TTTGlobalState *)globalState)->board[((TTTAction *)action)->position] = tttState->currentPlayer;
    
    int winner = checkWin(((TTTGlobalState *)globalState)->board);
    if (winner != 0) {
        done = true;
        reward = (winner == tttState->currentPlayer) ? 1 : -1;
    } else {
        done = false;
        reward = 0;
    }

    nextTTTState->currentPlayer = 3 - tttState->currentPlayer; // 1 or 2
  }

  // Called only if the global state is used
  virtual void restoreGlobalState(const State *state,
                                  const Action *choosedAction,
                                  const State *nextState,
                                  GlobalState *globalState
                                  ) {
    ((TTTGlobalState *)globalState)->board[((TTTAction *)choosedAction)->position] = 0;
  };

  // Gett all valid actions from a state
  virtual vector<unsigned int> *getValidActionsIds(const GlobalState *globalState,
                                                   const State *state,
                                                   const ActionSpace *actionSpace) {

    vector<unsigned int> *validActions = new vector<unsigned int>();
    const TTTGlobalState *tttGlobalState = dynamic_cast<const TTTGlobalState *>(globalState);

    for (int i = 0; i < 9; i++) {
        if (tttGlobalState->board[i] == 0) {
            validActions->push_back(i);
        }
    }

    return validActions;
  }
};

