#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Node {
    public: 
        vector<int> data;
        Node() {}
        Node *parent;
        vector<Node> children;
};