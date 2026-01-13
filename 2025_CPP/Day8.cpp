#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
using namespace std;


ifstream input("theFile.txt");

class Network{
    public:
        int nNodes;
        Network(){
            nNodes = 0;
        }
};

class Node{
    public:
        shared_ptr<Network> network;
        bool connected; 
        Node(){
            connected = false;
        }

        void connectToNetwork(shared_ptr<Network> net){
            net->nNodes += 1;
            connected = true;
            network = net;
        }
        void moveToNetwork(shared_ptr<Network> newNet){
            network = newNet;
        }

};



float calcNorm(vector<int>  a, vector<int>b){
    float total = 0;
    for(int i = 0; i<3; i++){
        total += pow((a[i] - b[i]),2);
    }
    return sqrt(total);
}   

vector<int> findMin(vector<vector<float> > matrixOfDistances){
    float minValue = numeric_limits<float>::max();
    int x, y;
    for(int i = 0; i< matrixOfDistances.size(); i++){
        for(int j = i+1; j< matrixOfDistances.size(); j++){
            if(matrixOfDistances[i][j] < minValue){
                minValue = matrixOfDistances[i][j];
                x = i;
                y = j;
            }
        }
    }
    vector<int> coords;
    coords.push_back(x);
    coords.push_back(y);
    return coords;
}

int findMax(vector<shared_ptr<Network> > networks){
    float maxValue = -1;
    int x;
    for(int i = 0; i< networks.size(); i++){
        if((networks[i] -> nNodes) > maxValue){
            maxValue = networks[i] -> nNodes;
            x = i;
        }
    }
    

    return x;
}

bool checkRow(vector<vector<int> > connectionsMatrix, int rowIndex){
    for(int i = 0; i < connectionsMatrix.size(); i++){
        if(connectionsMatrix[rowIndex][i] == 1){
            return true;
        }
    }
    return false;
}


vector<int> connectPoints(vector<vector<float> > matrixOfDistances,vector<Node> &nodes, vector<shared_ptr<Network> > &networks){
vector<shared_ptr<Network> > nodeNetworks;
for(int i = 0; i < nodes.size(); i++){
    nodeNetworks.push_back(0);
}

vector<int> coords;
int nConnections = matrixOfDistances.size();
    // for part one, replace while loop with for loop
    for(int l = 0; l < 1000; l++){
    // for part two, use while loop
    // while(true){
        coords = findMin(matrixOfDistances);
        int x = coords[0];
        int y = coords[1];
        matrixOfDistances[x][y] = 10000000;

        if(nodes[x].connected == false and nodes[y].connected == false){
            auto net = std::make_shared<Network>();
            nodes[x].connectToNetwork(net);
            nodes[y].connectToNetwork(net);
            networks.push_back(net);

        }
        else{
            if(nodes[x].connected and nodes[y].connected){
                if(nodes[x].network != nodes[y].network){
                    shared_ptr<Network> np1 = nodes[x].network;
                    shared_ptr<Network> np2 = nodes[y].network;

                    // replace all pointers in the vector with the new pointer
                    for (int i = 0; i < nodes.size(); i++) {
                        if (nodes[i].network == np1) {
                            nodes[i].moveToNetwork(np2);
                            nodeNetworks[i] = np2;
                        }
                    }
                    np2->nNodes += np1->nNodes;
                    np1->nNodes = 0;         

                } 
                continue;
                    
        }

            if (nodes[x].connected){
                nodes[y].connectToNetwork(nodes[x].network);
            }else if(nodes[y].connected){
                nodes[x].connectToNetwork(nodes[y].network);
            }
        }

        bool allConnected = true;
        for(int i = 0; i < nodes.size(); i++){
            if(nodes[i].connected == false){
                allConnected = false;
            }
        }
        if(allConnected){
            break;
        }

    }

    cout << "all network sizes: \n";
    for(int i = 0; i < networks.size(); i++){
        if(networks[i]->nNodes >0){
            cout << networks[i]->nNodes;
            cout << "\n";
        }
    }
    long mult = 1;
    for(int i = 0; i< 3; i++){
        int x = findMax(networks);
        mult *= networks[x]->nNodes;
        networks[x]->nNodes = 0;
    }
    cout << " \npart one output: \n";
    cout << mult;
    cout << "\n";
    return coords;
}


int main(){
    vector <vector <int> > points;
    vector <Node> nodes;
    for(string line; getline(input, line);){
        vector <int> row;
        string workingString = "";
        nodes.push_back(Node());
        for(int i = 0; i < line.length(); i++){
            if(line[i] == ',' or i == line.length()-1){
                if(i == line.length()-1){
                    workingString.append(string() + line[i]);

                }
                row.push_back(stoi(workingString));
                workingString = "";
            }
            else{
                workingString.append(string() + line[i]);
            }
        }
        points.push_back(row);
        
    }
    vector<vector<float> > matrixOfDistances;
    


    for(int i = 0; i < points.size();i++){
        vector<float> distances;
        for(int j = 0; j < points.size(); j++){
            if(i == j){
                distances.push_back(numeric_limits<float>::max());
            }
            else{
                distances.push_back(calcNorm(points[i],points[j]));
            }
        }
        matrixOfDistances.push_back(distances);
    }
    vector<shared_ptr<Network> > networks;
    vector<int> coords = connectPoints(matrixOfDistances, nodes, networks);
    int x = coords[0];
    int y = coords[1];

    cout << "\npart two output: \n";
    cout << long(points[x][0])*long(points[y][0]);

}