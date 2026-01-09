#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
std::ifstream input("theFile.txt");

int tachyonSplitter(vector<string> grid){
    int count = 0;
    string line = grid[0];
    for(int i = 1; i < grid.size(); i++){
        string prevLine = line;
        line = grid[i];
        for(int j = 0; j < line.length(); j++){
            if(prevLine[j] == 'S' || prevLine[j] == '|'){

                if (line[j] == '.'){
                    line[j] = '|';
                }else if(line[j] == '^'){
                    line[j+1] = '|';
                    line[j-1] = '|';
                    count+=1;
                }
            }
        }
        cout << line;
        cout << "\n";
    }
    return count;
}

class TachyonSplitter{
    public:
        vector<TachyonSplitter> descendents;
        vector<string> grid;
        int rowIndex;
        int colIndex;
        TachyonSplitter(vector<string> g, int y, int x){
            grid = g;
            rowIndex = y;
            colIndex = x;
        }

        void findDecendents(){
            // find left descendent
            for(int i = rowIndex+1;i < grid.size(); i++){
                if(grid[i][colIndex-1] == '^'){
                    descendents.push_back(TachyonSplitter(grid,i, colIndex-1));
                    break;
                }
            }
            // find right descendent
            for(int i = rowIndex+1;i < grid.size(); i++){
                if(grid[i][colIndex+1] == '^'){
                    descendents.push_back(TachyonSplitter(grid,i, colIndex+1));
                    break;
                }
            }
            for(int i = 0; i < descendents.size();i++){
                descendents[i].findDecendents();
            }
        }

        long calculateValue(){
            long value = 0;
            for(int i = 0; i < descendents.size();i++){
                value += descendents[i].calculateValue();
            }
            value = value + 2 - descendents.size();
            cout << "[";
            cout << rowIndex;
            cout << ",";
            cout << colIndex;
            cout << "]. Value: ";
            cout << value;
            cout << ". n.o. descendents: ";
            cout << descendents.size();
            cout << "\n ";

            return value;
        }
        
};


void findFirstSplitter(vector<string> grid){
    /**
     * This function finds the first tachyon splitter.
     * Recursively, this tachyon splitter finds the splitters it interacts with next and so on.
     * @param grid - the grid.
     * 
     */
    for(int i = 0; i <grid.size(); i++){
        for(int j = 0; j < grid[i].length(); j++){
            if(grid[i][j] == '^'){
                // confirm that two above is the S
                if (grid[i-2][j] == 'S'){
                    TachyonSplitter t = TachyonSplitter(grid,i,j);
                    t.findDecendents();
                    cout << t.calculateValue();
                    return;
                }
            }
        }
    }
}

long long lookDown(vector<vector<string> > grid, int rowIndex, int columnIndex){
    /**
     * This function looks down until it finds a value or the bottom of the grid. 
     * It returns the value if it finds it. Otherwise it returns 1.
     * @param grid - the grid.
     * @param rowIndex - the row index to start looking down from.
     * @param grid - the column we are looking down.
     * 
     * @returns the value.
     */
        for(int i = rowIndex+1; i < grid.size(); i++){
            if (grid[i][columnIndex] != "."){
                return stoll(grid[i][columnIndex]);
        }
    }
    return 1;
}



long long countingAlgorithm(vector<vector<string> > grid){
    /**
     * This function uses the same algorithm which I wanted to implement with the TachyonSplitter class. 
     * However that class was a flashy approach but far too inefficient.
     * We count from the bottom up the number of possible paths. 
     * Adding a 1 if we look down and reach the bottom or the value of the splitter we reach.
     * 
     * @param grid - the grid.
     * 
     * @returns the total number of possible paths.
     */

    for(int i = grid.size()-1; i >0; i--){
        for(int j = 0; j < grid[i].size(); j++){
            if(grid[i][j] == "^"){
                // we look left and right and add the values to determine the value of this splitter.

                long long left = lookDown(grid, i, j-1);
                long long right = lookDown(grid, i, j+1);
                grid[i][j] = to_string(left+right);
            }

        }
    }

    // look in row 2 to find the total number
    for(int i = 0; i < grid[2].size(); i++){
        if(grid[2][i] != "."){
            return stoll(grid[2][i]);
        }
    }
    return -1;
}



int main(){
    // same as before, generate a vector of strings
    vector <string> grid;


    for(string line; getline(input, line);){
        grid.push_back(line);
    }
    int size = grid.size();

    cout << tachyonSplitter(grid);
    cout << "\n";


    // This did work but was reaaaally bad on memory and crashed on large inputs.
    // findFirstSplitter(grid);

    // we need individual strings not chars

    vector<vector<string> > grid2;
    for(int i = 0; i < size; i++){
        vector<string> row;
        for(int j = 0; j < grid[i].length(); j++){
            row.push_back(string() + grid[i][j]);
        }
        grid2.push_back(row);
    }
    cout << countingAlgorithm(grid2);
}