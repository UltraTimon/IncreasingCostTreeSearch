#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Node {
    public: 
        list<int> data;
        Node(list<int> givenData) {
            data = givenData;
        }
        Node *parent;
        list<Node> children;
};