#include "icts.h"

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
    void printMDDMDD()
    {
    }

private:
    int depthOfMDD(MDD *mdd)
    {
        if (mdd->children.size() > 0)
            return 1 + depthOfMDD(&(mdd->children.front()));
        else
            return 1;
    }

    void printMDDRecursive(MDD *mdd, int level)
    {
        if (level == 0)
        {
            cout << mdd->data << " ";
        }
        else
        {
            for (auto child : mdd->children)
            {
                printMDDRecursive(&child, level - 1);
            }
        }
    }

    void printMDDMDD(MDD *mdd)
    {
        int depth = depthOfMDD(mdd);
        for (int i = 0; i < depth; i++)
        {
            printMDDRecursive(mdd, i);
            cout << endl;
        }
    }
};
