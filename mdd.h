#include "icts.h"


class MDD
{
public:
    int data;
    bool pathLeadsToDestination;
    vector<MDD> children;
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