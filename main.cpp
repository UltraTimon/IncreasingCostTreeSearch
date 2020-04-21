// C++ program to print all paths from a source to destination.
#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "generatePaths.h"

using namespace std;

int main() {

    list<list<int>> pathsTaken = generatePaths("graph.txt", 0, 5);

    int printPathCounter = 0;
    for (auto list : pathsTaken)
    {
        cout << "path " << printPathCounter << ": ";
        printPathCounter++;
        for (auto i : list)
        {
            cout << i << " ";
        }
        cout << endl;
    }

    return 0;
}