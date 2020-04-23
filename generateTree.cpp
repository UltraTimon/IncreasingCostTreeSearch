#include "generateTree.h"

Node generateRoot(vector<int> rootCostList) {
    Node root = Node();
    root.data = rootCostList;
    root.parent = nullptr;
    return root;
}

vector<Node> generateNextLevelOfChildren(Node tree) {
    vector<Node> children;
    for(int outer = 0; outer < tree.data.size(); ++outer) {
        vector<int> childData;
        int inner = 0;
        for (auto parentDataIt = tree.data.begin(); parentDataIt != tree.data.end(); ++parentDataIt)
        {
            if (outer == inner) {
                childData.push_back(*parentDataIt + 1);
            }
            else {
                childData.push_back(*parentDataIt);
            }
            inner++;
        }
        Node childNode = Node();
        childNode.data = childData;
        childNode.parent = &tree;
        children.push_back(childNode);
    }
    return children;
}

void generateNextLevel(Node *tree) {
    if(tree->children.size() == 0) {
        tree->children = generateNextLevelOfChildren(*tree);
    } else {
        for(vector<Node>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
            generateNextLevel(&(*it));
        } 
    }
}

int depthOfTree(Node *tree) {
    if(tree->children.size() > 0) 
        return 1 + depthOfTree(&(tree->children.front()));
    else
        return 1;
}

void printTreeRecursive(Node *tree, int level) {
    if(level == 0) {
        for(auto i : tree->data) {
            cout << i << " ";
        }
        cout << " - ";
    } else {
        for(auto child : tree->children) {
            printTreeRecursive(&child, level - 1);
        }
    }
}

void printTree(Node *tree) {
    int depth = depthOfTree(tree);
    for(int i = 0; i < depth; i++) {
        printTreeRecursive(tree, i);
        cout << endl;
    } 
}

void printTree(Node *tree, int depth) {
    for(int i = 0; i < depth; i++) {
        printTreeRecursive(tree, i);
        cout << endl;
    } 
}
