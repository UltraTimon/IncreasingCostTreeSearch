#include "generateTree.h"

Node generateRoot(int rootCost, int numberOfAgents) {
    list<int> rootData;
    for (int i = 0; i < numberOfAgents; i++)
    {
        rootData.push_back(rootCost);
    }
    Node root = Node();
    root.data = rootData;
    cout << "after generateRoot " << root.data.size() << endl;
    root.parent = nullptr;
    return root;
}

list<Node> generateNextLevelOfChildren(Node tree) {
    cout << "hi from gen!" << endl;
    list<Node> children;
    cout << "data length " << tree.data.size() << endl;
    for(int outer = 0; outer < tree.data.size(); ++outer) {
        list<int> childData;
        int inner = 0;
        cout << "outer loop" << endl;
        for (auto parentDataIt = tree.data.begin(); parentDataIt != tree.data.end(); ++parentDataIt)
        {
            cout << "outer " << outer << " inner " << inner << endl;
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
        children.push_back(childNode);
        childNode.parent = &tree;
    }
    return children;
}