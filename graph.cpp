#include "graph.h"

Graph importGraph(string filename) {
    // Reading in the edges from a file
    ifstream myfile(filename);
    int nodeA, nodeB;
    int nrOfNodes = 999;
    if (myfile.is_open())
    {
        myfile >> nrOfNodes;
    }
    Graph g = Graph(nrOfNodes);
    if (myfile.is_open())
    {
        while (myfile >> nodeA >> nodeB)
        {
            g.nodes[nodeA].edges.push_back(nodeB);
            g.nodes[nodeB].edges.push_back(nodeA);
        }
        myfile.close();
    }
    else
    {
        cout << "generatePaths: Unable to open file. Sorry" << endl;
    }

    // add self edges
    for (int i = 0; i < g.nodes.size(); i++)
    {
        g.nodes[i].edges.push_back(i);
    }
    

    return g;
}

Graph importGraphFromPythonOutput(string filename) {
    // Reading in the edges from a file
    ifstream myfile(filename);
    int width, height; // width along the x-axis, height along the y-axis

    if (myfile.is_open())
    {
        myfile >> width;
        myfile >> height;
    }

    int nrOfNodes = width * height;

    
    Graph g = Graph(nrOfNodes);
    g.width = width;
    printf("width: %d, height: %d, #nodes: %lu\n", width, height, g.nodes.size());

    int grid[height * width];
    int wallData;

    if (myfile.is_open())
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {   
                if(myfile >> wallData) {
                    grid[y * width + x] = wallData;
                }
            }
            
        }
        myfile.close();
    }
    else
    {
        cout << "generatePaths: Unable to open file. Sorry" << endl;
    }

    // add self edges
    for (int i = 0; i < g.nodes.size(); i++)
    {
        g.nodes[i].edges.push_back(i);
    }
    /*
        pos X, Y = grid[y][x] = g->nodes[y * width + x]
    */
    // convert grid array into graph edges
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {   
            printf("bef y: %d, x: %d\n", y, x);

            if(grid[y * width + x] == 1) {
                printf("skip node %d\n", y * width + x);
                continue;
            }
            if(x == width - 1 && y == height - 1)
                continue;


            printf("after y: %d, x: %d\n", y, x);
            int currentNodeIndex = y * width + x;
            int downNodeIndex = (y + 1) * width + x;
            int rightNodeIndex = y * width + x + 1;

            // add right edge
            if(x < width - 1 && grid[rightNodeIndex] == 0) {
                g.nodes[currentNodeIndex].edges.push_back(rightNodeIndex);
                g.nodes[rightNodeIndex].edges.push_back(currentNodeIndex);
            } else {
                printf("%d: skip right edge to node %d\n", currentNodeIndex, rightNodeIndex);
            }

            // add down edge
            if(y < height - 1 && grid[downNodeIndex] == 0) {
                cout << "if" << endl;
                g.nodes[currentNodeIndex].edges.push_back(downNodeIndex);
                g.nodes[downNodeIndex].edges.push_back(currentNodeIndex);
            } else {
                cout << "else" << endl;
                printf("%d: skip down edge to node %d\n", currentNodeIndex, downNodeIndex);
            }
                
        }   
    } 
    

    return g;
}

Agent::Agent(int givenStart, int givenWaypoint, int givenEnd) {
    start = givenStart;
    waypoint = givenWaypoint;
    end = givenEnd;
}

void Agent::setPaths(vector<vector<int>> givenPaths) {
    paths = givenPaths;
}


vector<Agent> importAgents(string filename) {
    vector<Agent> agentList;

    ifstream myfile(filename);
    int start, end, waypoint;

    if (myfile.is_open())
    {
        while (myfile >> start >> waypoint >> end)
        {
            Agent newAgent = Agent(start, waypoint, end);
            agentList.push_back(newAgent);
        }
        myfile.close();
    }
    else
    {
        cout << "ImportAgents: Unable to open file. Sorry" << endl;
    }

    return agentList;
}

vector<Agent> importAgentsFromPythonOutput(string filename) {
    vector<Agent> agentList;

    ifstream myfile(filename);
    int start, end, waypoint;
    vector<vector<int>> data;


    if (myfile.is_open())
    {
        for (int i = 0; i < 2; i++)
        {
            if (myfile >> start >> end)
            {
                vector<int> vec;
                vec.push_back(start);
                vec.push_back(end);
                data.push_back(vec);
            }
        }
        for (int i = 0; i < 2; i++)
        {
            if (myfile >> waypoint)
            {
                Agent agent = Agent(data[i][0], waypoint, data[i][1]);
                agentList.push_back(agent);
            }
        }
        myfile.close();
    }
    else
    {
        cout << "ImportAgents: Unable to open file. Sorry" << endl;
    }

    return agentList;
}
