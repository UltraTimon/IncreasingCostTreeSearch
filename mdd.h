#include "icts.h"

class MDD {
    public:
        int data;
        vector<MDD> children;
        vector<MDD> parents;
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