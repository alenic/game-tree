#ifndef BFS_H
#define BFS_H

#include <algorithm>
#include <chrono>
#include <limits>
#include <list>

#include "game_engine.hpp"
#include "game_state.hpp"
#include "tree.hpp"

class BFS {
 public:
  double mMaxTime;
  double mCriticalTime;
  int mMaxLevel;
  double mMaxValue;
  Node *mMaxValueNode;
  Node *mRootNode;
  int mBeamK;
  int mBeamLevel;
  chrono::high_resolution_clock::time_point mStartTime;

  BFS() {
    mMaxTime = 0;
    mCriticalTime = 0;
    mMaxLevel = 0;
    mMaxValueNode = nullptr;
    mRootNode = nullptr;
    mBeamK = 0;
    mBeamLevel = 0;
  }

  void cleanTree() {
    if (mRootNode != nullptr) {
      delete mRootNode;
      mRootNode = nullptr;
      mMaxValueNode = nullptr;
    }
  }

  void startTiming(chrono::high_resolution_clock::time_point startTime) {
    mStartTime = startTime;
  }

  Node *formNode(GameState *rootState, bool copyRootState) {
    Node *node = new Node();
    // To prevent rootState erasing
    if (copyRootState) {
      GameState *rootStateCopy = rootState->clone();
      node->setState(rootStateCopy);
    } else {
      node->setState(rootState);
    }
    node->setParentNode(nullptr);
    node->setLevel(0);
    node->setValue(0);

    return node;
  }

  list<int> run(GameState *rootState, GameEngine *gameEngine,
                bool copyRootState) {
    Node *rootNode = formNode(rootState, copyRootState);
    return run(rootNode, gameEngine);
  }

  list<int> run(Node *rootNode, GameEngine *gameEngine) {
    list<int> bestActionTrace;
    list<Node *> queue;
    vector<Node *> nextNodes;
    vector<int> validAction;
    nextNodes.reserve(gameEngine->getActionNum());
    validAction.reserve(gameEngine->getActionNum());

    bool stopVisit;

    mRootNode = rootNode;

    queue.push_front(rootNode);

    mMaxValue = -numeric_limits<double>::max();

    while (!queue.empty()) {
      if (mCriticalTime) {
        double elapsedMillis =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - mStartTime)
                .count();
        if (elapsedMillis > mCriticalTime) {
          // Force exit: remove all queue
          while (!queue.empty()) {
            queue.pop_back();
          }
          break;
        }
      }

      Node *currentNode = queue.back();
      queue.pop_back();

      stopVisit = false;

      if (mMaxLevel > 0 && currentNode->getLevel() >= mMaxLevel) {
        stopVisit = true;
      }

      if (currentNode->getState()->isTerminal()) {
        stopVisit = true;
      }

      if (mMaxTime) {
        double elapsedMillis =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - mStartTime)
                .count();
        if (elapsedMillis > mMaxTime) {
          stopVisit = true;
        }
      }

      if (stopVisit) {
        if (currentNode->getValue() > mMaxValue) {
          mMaxValue = currentNode->getValue();
          mMaxValueNode = currentNode;
        }
        continue;
      }

      // Check if rootNode is itself a tree
      if (1) {
        if (currentNode->mChilds.size() > 0) {
          // Push the nodes
          list<Node *>::iterator child;
          for (child = currentNode->mChilds.begin();
               child != currentNode->mChilds.end(); ++child) {
            if (*child != nullptr) {
              queue.push_front(*child);
            }
          }
          // skip the expansion (already expanded)
          continue;
        }
      }

      // Expand node
      GameState *currentState = currentNode->getState();
      validAction.clear();
      gameEngine->getValidActions(currentState, validAction);

      // If no more actions it should be a terminal (GameEngine problem)
      if (validAction.size() == 0) {
        if (currentNode->getValue() > mMaxValue) {
          mMaxValue = currentNode->getValue();
          mMaxValueNode = currentNode;
        }
        continue;
      }

      nextNodes.clear();
      for (int i = 0; i < validAction.size(); i++) {
        double reward;
        GameState *nextState;
        gameEngine->getNextStateAndReward(currentState, validAction[i],
                                          nextState, reward);
        Node *nextNode = new Node();
        nextNode->setState(nextState);
        nextNode->setParentNode(currentNode);
        nextNode->setParentAction(validAction[i]);
        nextNode->setLevel(currentNode->getLevel() + 1);
        nextNode->setValue(nextState->getValue());
        nextNodes.push_back(nextNode);
      }

      if (mBeamK > 0 && currentNode->getLevel() >= mBeamLevel) {
        // Sort best k value
        sort(nextNodes.begin(), nextNodes.end(), [](Node *lhs, Node *rhs) {
          return lhs->getValue() > rhs->getValue();
        });

        // delete the othe nodes
        for (int i = mBeamK; i < validAction.size(); i++) {
          delete nextNodes[i];
          nextNodes.pop_back();
        }
      }

      for (int i = 0; i < nextNodes.size(); i++) {
        currentNode->addChild(nextNodes[i]);
        queue.push_front(nextNodes[i]);
      }
    }

    Node *visitNode = mMaxValueNode;
    while (visitNode != nullptr) {
      if (visitNode->getLevel() == 0) break;
      if (visitNode->getParentNode() == nullptr) break;
      bestActionTrace.push_front(visitNode->getParentAction());
      visitNode = visitNode->getParentNode();
    }

    return bestActionTrace;
  }
};

#endif