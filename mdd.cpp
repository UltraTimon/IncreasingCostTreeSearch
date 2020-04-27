#include "mdd.h"

list<int> constructPath(MDD *current, int start, list<int> path) {
    cout << "before read" << endl;
    path.push_front(current->data);
    cout << "after read" << endl;

    cout << "tempPath: " << endl;
    for (int i : path) 
        cout << i << " ";
    cout << endl;

    if(current->data == start) {
        return path;
    } 
    
    return constructPath(current->parent, start, path);
}

void convertMDDToPathList(int start, Destination & dest, vector<vector<int>> & pathList) {
    list<int> tempPath;

    cout << "dest parents: " << endl;
    for (auto i : dest.parents) 
        cout << i.data << " ";
    cout << endl;

    
    tempPath.push_front(dest.data);
    

    // cout << "#parents: " << dest.parents.size() << endl;
    for(auto parent : dest.parents) {
        list<int> result = constructPath(&parent, start, tempPath);
        cout << "result size: " << result.size() << endl;
        vector<int> pathVector(result.begin(), result.end());
        pathList.push_back(pathVector);
    }

    // cout << "mdd converted, #paths: " << pathList.size() << endl;

    // return pathList;
}