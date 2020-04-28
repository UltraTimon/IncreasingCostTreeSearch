#include "icts.h"
#include <deque>


class MDD
{
public:
    int data;
    bool pathLeadsToDestination;
    vector<int> children;
    MDD *parent;
    MDD(int i)
    {
        data = i;
        pathLeadsToDestination = false;
    }
};

class Destination
{
public:
    int data;
    vector<MDD> parents;
    Destination(int i)
    {
        data = i;
    }
};

void convertMDDToPathList(int start, deque<MDD> & endPoints, vector<vector<int>> & pathList);