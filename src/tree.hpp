#ifndef TREE_H
#define TREE_H

#include "game_state.hpp"

class Node {
 protected:
  GameState *mState;
  int mParentAction;
  Node *mParentNode;
  int mLevel;
  double mValue;

 public:
  list<Node *> mChilds;

  Node() {
    mState = nullptr;
    mValue = 0;
    mLevel = 0;
    mParentAction = 0;
    mState = nullptr;
    mParentNode = nullptr;
  }

  ~Node() {
    list<Node *>::iterator it;
    for (it = mChilds.begin(); it != mChilds.end(); ++it) {
      if (*it != nullptr) {
        delete *it;
        *it = nullptr;
      }
    }

    if (mState != nullptr) {
      delete mState;
    }
    mChilds.clear();
  }

  void setState(GameState *s) { mState = s; }

  GameState *getState() { return mState; }

  void addChild(Node *c) { mChilds.push_back(c); }

  void removeChild(Node *childPtr, bool deleteNode) {
    if (mChilds.size() > 0) {
      list<Node *>::iterator it;
      for (it = mChilds.begin(); it != mChilds.end(); ++it) {
        if ((*it) == childPtr) {
          if (deleteNode) {
            delete *it;
            *it = nullptr;
          }
          mChilds.remove(*it);
          return;
        }
      }
    }
  }

  Node *getChildFromAction(int action) {
    list<Node *>::iterator it;
    for (it = mChilds.begin(); it != mChilds.end(); ++it) {
      if ((*it)->getParentAction() == action) {
        return *it;
      }
    }
    return nullptr;
  }

  void setValue(double v) { mValue = v; }

  double getValue() { return mValue; }

  void setParentAction(int parent) { mParentAction = parent; }

  int getParentAction() { return mParentAction; }

  void setParentNode(Node *parent) { mParentNode = parent; }

  Node *getParentNode() { return mParentNode; }

  void setLevel(int level) { mLevel = level; }

  int getLevel() { return mLevel; }

  friend class GameTreeUtils;
};

// GameTree utils
class GameTreeUtils {
 public:
  static void resetTreeLevel(Node *node, int level) {
    if (node->mChilds.size() > 0) {
      list<Node *>::iterator it;
      for (it = node->mChilds.begin(); it != node->mChilds.end(); ++it) {
        (*it)->setLevel(level + 1);
        resetTreeLevel(*it, level + 1);
      }
    }
  }

  static void cutNodeFromTree(Node *node, bool resetLevel) {
    // Remove node from the parent (without deleting node)
    Node *parentNode = node->getParentNode();
    if (parentNode != nullptr) {
      parentNode->removeChild(node, false);
    }
    // remove dipendence from the parent
    node->setParentNode(nullptr);

    if (resetLevel) {
      resetTreeLevel(node, 0);
    }

    // How to do with parent action ? TODO
  }

  static Node *getNodeFromActionTrace(Node *rootNode,
                                      list<int> &bestActionTrace,
                                      int maxLevel) {
    Node *node = rootNode;
    list<int>::iterator it;
    if (bestActionTrace.size() > 0) {
      int k = 0;
      for (it = bestActionTrace.begin(); it != bestActionTrace.end(); it++) {
        node = node->getChildFromAction(*it);
        if (node == nullptr) {
          cerr << "getNodeFromActionTrace error" << endl;
          return nullptr;
        }
        k++;
        if (k == maxLevel) break;
      }
    }
    return node;
  }
};

#endif  // TREE_H