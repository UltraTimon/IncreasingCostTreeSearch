#ifndef GENTREE_H
#define GENTREE_H

#include "tree.h"
#include "basics.h"

Node generateRoot(vector<int> rootCostList);

vector<Node> generateNextLevelOfChildren(Node tree);

void generateNextLevel(Node *tree);

void printTree(Node *tree);

void printTree(Node *tree, int depth);

#endif