#include <string>
#include <vector>
#include <iostream>
#include <fstream>
std::ifstream input("thefile.txt");

using namespace std;

bool checkSurroundings(char** arr, int length, int width, int x, int y){
    /**
     * Given an x and y coordinate of a '@', check if it is valid to be removed. 
     * 
     * @param arr - the grid.
     * @param length - the length of the grid.
     * @param width - the width of the grid.
     * @param x - the x coordinate.
     * @param y - the y coordinate.
     * 
     * @returns true if valid '@' else false.
     */
    
    // check all surrounding points in the array
    int count = 0;
    if(arr[x][y] == '.'){
        return 0;
    }
    for(int i = x-1; i<= x+1; i++){
        // if the x coordinate is out of bounds, skip
        if(i <0 || i>=length){
            continue;
        }
        else{
            // if the y coordinate is out of bounds, skip
            for(int j = y-1; j<= y+1; j++){
                if(j <0 || j>=width){
                    continue;
                }
                else{
                    if(arr[i][j] == '@' or arr[i][j] == 'X'){
                        count++;
                    }
                }
            }
        }
    }
    // including itself, we want no more than 4
    return count <=4;
}

int gridSearch(int length, int width, char** arr){
    /**
     * Search the grid for possible '@' to remove. 
     * Replace all valid '@' with X and increment the score.
     * 
     * @param length - the length of the grid.
     * @param width - the width of the grid.
     * @param arr - the grid.
     * 
     * @returns the number of '@' removals.
     */

    int total = 0;
    for(int i = 0; i <length; i++){
        for(int j = 0; j< width; j++){
            if(checkSurroundings(arr, length, width, i, j)){
                arr[i][j] = 'X';
                total++;
            }
        }
        cout << arr[i];
        cout << "\n";
    }
    return total;
}

void resetArr(char** arr, int length, int width){
     /**
     * Replaces all 'X' with '.' in the grid 
     * 
     * @param arr - the grid.
     * @param length - the length of the grid.
     * @param width - the width of the grid.
     */

    // resets the array to remove all 'X's.
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(arr[i][j] == 'X'){
                arr[i][j] = '.';
            }
        }
    }
    return;
}

int main(){
    // first we need to know the dimensions of the array
    vector<string> lines;
    string line;
    while (getline(input, line)) {
        lines.push_back(line);
    }
    int length = lines.size();
    int width = lines.empty() ? 0 : lines[0].size();

    // now we make the array
    // because we are using dynamic arrays we need an array of pointers
    char** arrOfPtrs = new char*[length];
    for(int count = 0; count < length; count++){
        arrOfPtrs[count] = new char[width];
        
        for(int i = 0; i< width;i++){
            // I know this is kinda goofy. I just wanted to force myself to use a dynamic array. It's pretty cool.
            arrOfPtrs[count][i] = lines[count][i];
            
        }

    }

    // now we can check the array
    int bigTotal = 0;
    int littleTotal;
    while(true){
        littleTotal = gridSearch(length, width, arrOfPtrs);
        // if no more '@'s are removed then we are done
        if (littleTotal == 0){
            break;
        }
        // otherwise we need to set all of the 'X's to '.'
        resetArr(arrOfPtrs, length, width);
        bigTotal += littleTotal;
    }

    cout << bigTotal;

    // deallocate memory
    for(int i = 0; i< length; i++){
        delete[] arrOfPtrs[i];
    }
    delete[] arrOfPtrs;

    return 0;
}


