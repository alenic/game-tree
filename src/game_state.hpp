#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <list>
#include <vector>

using namespace std;

class GameState {
 protected:
  bool mTerminal;
  double mValue;

 public:
  GameState() {
    mTerminal = false;
    mValue = 0.0;
  }

  // REMEMBER: Assign mTerminal, mValid and mValue
  virtual GameState *clone() = 0;

  virtual ~GameState() {}

  bool isTerminal() { return mTerminal; }

  void setTerminal(bool v) { mTerminal = v; }

  void setValue(double v) { mValue = v; }

  double getValue() { return mValue; }
};

#endif  // GAME_NODE_H