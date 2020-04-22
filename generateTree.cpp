#include "generateTree.h"

Node generateRoot(int rootCost, int numberOfAgents) {
    list<int> rootData;
    for (int i = 0; i < numberOfAgents; i++)
    {
        rootData.push_back(rootCost);
    }
    Node root = Node();
    root.data = rootData;
    root.parent = nullptr;
    return root;
}

list<Node> generateNextLevelOfChildren(Node tree) {
    list<Node> children;
    for(int outer = 0; outer < tree.data.size(); ++outer) {
        list<int> childData;
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
        cout << "gen. children..." << endl;
        tree->children = generateNextLevelOfChildren(*tree);
    } else {
        cout << "children exist, #children: " << tree->children.size() << ", passing the call to the children" << endl;
        for(list<Node>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
            generateNextLevel(&(*it));
        } 
        // for(auto child : tree->children) {
        //     generateNextLevel(&child);
        // }
    }
}