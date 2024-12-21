#ifndef UTILS_HPP
#define UTILS_HPP

#include "node.hpp"
#include <list>

using namespace std;

namespace gametree {

class GameTreeUtils
{
public:
  static void resetTreeLevel(Node *node, int level)
  {
    if (node->mChilds.size() > 0)
    {
      for (size_t i=0; i<node->mChilds.size(); i++)
      {
        node->mChilds[i]->setLevel(level + 1);
        resetTreeLevel(node->mChilds[i], level + 1);
      }
    }
  }

  static void cutNodeFromTree(Node *node, bool resetLevel)
  {
    // Remove node from the parent (without deleting node)
    Node *parentNode = node->getParentNode();
    if (parentNode != nullptr)
    {
      parentNode->removeChild(node, false);
    }
    // remove dipendence from the parent
    node->setParentNode(nullptr);

    if (resetLevel)
    {
      resetTreeLevel(node, 0);
    }

    // How to do with parent action ? TODO
  }

  static Node *getNodeFromActionTrace(Node *rootNode,
                                      list<int> &bestActionTrace,
                                      int maxLevel)
  {
    Node *node = rootNode;
    list<int>::iterator it;
    if (bestActionTrace.size() > 0)
    {
      int k = 0;
      for (it = bestActionTrace.begin(); it != bestActionTrace.end(); it++)
      {
        node = node->getChildFromAction(*it);
        if (node == nullptr)
        {
          cerr << "getNodeFromActionTrace error" << endl;
          return nullptr;
        }
        k++;
        if (k == maxLevel)
          break;
      }
    }
    return node;
  }
};

} // namespace gametree

#endif