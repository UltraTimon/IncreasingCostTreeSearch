#ifndef TREE_H
#define TREE_H

#include "basics.h"

class Node {
    public: 
        vector<int> data;
        Node() {}
        Node *parent;
        vector<Node> children;
};

#endif