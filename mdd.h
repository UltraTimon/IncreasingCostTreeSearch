#include "icts.h"

class MDD {
    public:
        int data;
        vector<MDD> children;
        MDD *parent;
        MDD(int i) {
            data = i;
        }
};

class Destination {
    public:
        int data;
        vector<MDD> parents;
        Destination(int i) {
            data = i;
        }
};