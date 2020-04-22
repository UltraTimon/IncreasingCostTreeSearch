#include "generatePaths.h"
#include "agent.h"
#include "lowlevelsearch.h"
#include "generateTree.h"

using namespace std;

int main(int argc, char **argv)
{
    // get list of agent objects read form file
    // list<Agent> agentList = importAgents("resources/agents.txt");
    // int numberOfAgents = agentList.size();
    int numberOfAgents = 2;
    // // generate paths for each agent, add paths to agent object
    // // This will first only determine what the optimal cost is for every agent by calculating paths with an iteratively increasing cost until
    // //      each agent has at least one path with that cost
    int optimalCost = 1;
    // bool everyAgentHasAtLeastOnePath = false;
    // list<Agent>::iterator agentIterator;

    // while (!everyAgentHasAtLeastOnePath)
    // {
    //     for (auto agentIterator = agentList.begin(); agentIterator != agentList.end(); ++agentIterator)
    //     {
    //         list<list<int>> generatedPaths = generatePaths("resources/graph.txt", agentIterator->start, agentIterator->end, optimalCost);
    //         if (generatedPaths.size() == 0)
    //         {
    //             optimalCost++;
    //             cout << "optimalCost increased to " << optimalCost << endl;
    //             break;
    //         }
    //         else
    //         {
    //             cout << "every agent has at least one path with length " << optimalCost << endl;
    //             everyAgentHasAtLeastOnePath = true;
    //             break;
    //         }
    //     }
    // }

    /*
        1) Does there exist a path of cost x?
        2) If yes, and we are going to examine this cost node, does there exist a path for each agent of that cost that do not conflict?
    */

    // Create ICT for up to 3 levels!
    // hardcoded value for optimalCost for testing:
    // root
    // list<int> rootData;
    // for (int i = 0; i < numberOfAgents; i++)
    // {
    //     rootData.push_back(optimalCost);
    // }
    // Node root = Node(rootData);
    // root.parent = nullptr;

    // cout << "root data: ";
    // for(int i : root.data) 
    //     cout << i << " ";
    // cout << endl;

    // // first level of children
    // // create n nodes for n agents
    // for (int outer = 0; outer < numberOfAgents; outer++)
    // {
    //     list<int> childData;
    //     // fill childData list with data from root 
    //     int inner = 0;
    //     for (auto it = root.data.begin(); it != root.data.end(); ++it)
    //     {
    //         if (outer == inner) {
    //             childData.push_back(*it + 1);
    //         }
    //         else {
    //             childData.push_back(*it);
    //         }
    //         inner++;
    //     }
    //     Node childNode = Node(childData);
    //     root.children.push_back(childNode);
    //     childNode.parent = &root;
    // }


    // // second level of children
    
    // for (auto parentIt = root.children.begin(); parentIt != root.children.end(); ++parentIt)
    // {
    //     for(int outer = 0; outer < numberOfAgents; outer++) {
    //         list<int> childData;
    //         int inner = 0;
    //         // fill childData list with data from parent agent 
    //         for (auto childIt = parentIt->data.begin(); childIt != parentIt->data.end(); ++childIt)
    //         {
    //             if (outer == inner) {
    //                 childData.push_back(*childIt + 1);
    //             }
    //             else {
    //                 childData.push_back(*childIt);
    //             }
    //             inner++;
    //         }
    //         Node childNode = Node(childData);
    //         parentIt->children.push_back(childNode);
    //         childNode.parent = &(*parentIt);
    //     }
    // }


    // // third level of children
    // for (auto superParentIt = root.children.begin(); superParentIt != root.children.end(); ++superParentIt)
    // {
    //     for(auto parentIt = superParentIt->children.begin(); parentIt != superParentIt->children.end(); ++parentIt)
    //     {
    //         for(int outer = 0; outer < numberOfAgents; outer++) {
    //             list<int> childData;
    //             int inner = 0;
    //             // fill childData list with data from parent agent 
    //             for (auto childIt = parentIt->data.begin(); childIt != parentIt->data.end(); ++childIt)
    //             {
    //                 if (outer == inner) {
    //                     childData.push_back(*childIt + 1);
    //                 }
    //                 else {
    //                     childData.push_back(*childIt);
    //                 }
    //                 inner++;
    //             }
    //             Node childNode = Node(childData);
    //             parentIt->children.push_back(childNode);
    //             childNode.parent = &(*parentIt);
    //         }
    //     }
    // }


    optimalCost = 10;
    
    Node root = generateRoot(optimalCost, numberOfAgents);
    root.children = generateNextLevelOfChildren(root);
    list<Node> firstGenChilderenA, firstGenChilderenB;

    for(list<Node>::iterator fgIt = root.children.begin(); fgIt != root.children.end(); ++fgIt) {
        fgIt->children = generateNextLevelOfChildren(*fgIt);
    } 

    for(list<Node>::iterator fgIt = root.children.begin(); fgIt != root.children.end(); ++fgIt) {
        for(list<Node>::iterator sgIt = fgIt->children.begin(); sgIt != fgIt->children.end(); ++sgIt) {
            sgIt->children = generateNextLevelOfChildren(*sgIt);
        }
    }

    cout << "root level children data: ";
    for(auto it2 : root.data) {
        cout << it2 << " ";
    }
    cout << endl;  

    cout << "1st level children data: ";
    for(auto it : root.children) {
        for(auto it2 : it.data) {
            cout << it2 << " ";
        }
        cout << "- ";
    }
    cout << endl;  

    cout << "2nd level children data: ";
    for(auto parentIt : root.children) {
        for(auto childIt : parentIt.children) {
            for(int data : childIt.data) {
                cout << data << " ";
            }
            cout << " - ";
        }
    }
    cout << endl;    

    return 0;
}