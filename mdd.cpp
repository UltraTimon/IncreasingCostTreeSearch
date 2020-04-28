#include "mdd.h"

list<int> constructPath(MDD *current, int start, list<int> path) {

    cout << "before read" << endl;
    path.push_front(current->data);
    cout << "after read" << endl;

    cout << "tempPath: " << endl;
    for (int i : path) 
        cout << i << " ";
    cout << endl;

    cout << "upcoming parent: " << current->parent->data << endl;

    if(current->parent == 0) {
        cout << "parent null, aborting" << endl;
        return path;
    }

    if(current->data == start) {
        return path;
    } 
    
    return constructPath(current->parent, start, path);
}

void convertMDDToPathList(int start, int end, deque<MDD> *MDDList, vector<vector<int>> *pathList) {
    list<int> tempPath;

    for(auto children : *endPoints) {
        list<int> result = constructPath(&endPoint, start, tempPath);
        cout << "result size: " << result.size() << endl;
        vector<int> pathVector(result.begin(), result.end());
        pathList.push_back(pathVector);
    }
}