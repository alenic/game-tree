#ifndef NODE_H
#define NODE_H

#include "game_environment.hpp"

namespace gametree {

class Node
{
protected:
  State *mState;
  unsigned int mParentActionId;
  Node *mParentNode;
  unsigned int mLevel;
  double mValue;

public:
  vector<Node *> mChilds;

  Node()
  {
    mState = nullptr;
    mValue = 0;
    mLevel = 0;
    mParentActionId = 0;
    mState = nullptr;
    mParentNode = nullptr;
  }

  ~Node()
  {
    vector<Node *>::iterator it;
    for (size_t i=0; i<mChilds.size(); i++)
    {
      if (mChilds[i] != nullptr)
      {
        delete mChilds[i];
        mChilds[i] = nullptr;
      }
    }
    if (mState != nullptr)
    {
      delete mState;
    }
    mChilds.clear();
  }

  void setState(State *s) { mState = s; }

  State *getState() const { return mState; }

  void addChild(Node *c) { mChilds.push_back(c); }

  void removeChild(Node *childPtr, bool deleteNode)
  {
    if (mChilds.size() > 0)
    {
      for (size_t i=0; i<mChilds.size(); i++) {
        if (mChilds[i] == childPtr)
        {
          if (deleteNode)
          {
            delete mChilds[i];
            mChilds[i] = nullptr;
          }
          mChilds.erase(mChilds.begin() + i);
          return;
        }
      }
    }
  }

  Node *getChildFromAction(unsigned int actionId) const
  {
    for (size_t i=0; i<mChilds.size(); i++) {
      if (mChilds[i]->getParentActionId() == actionId)
      {
        return mChilds[i];
      }
    }

    return nullptr;
  }

  void setValue(double v) { mValue = v; }

  double getValue() { return mValue; }

  void setParentAction(unsigned int actionId) { mParentActionId = actionId; }

  unsigned int getParentActionId() { return mParentActionId; }

  void setParentNode(Node *parent) { mParentNode = parent; }

  Node *getParentNode() const { return mParentNode; }

  void setLevel(unsigned int level) { mLevel = level; }

  int getLevel() { return mLevel; }

  friend class GameTreeUtils;
};

#endif // NODE_H

} // namespace gametree