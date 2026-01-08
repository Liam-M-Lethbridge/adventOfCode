#include <string>
#include <iostream>
#include <fstream>
std::ifstream input("theFile.txt");

using namespace std;

int findBiggestNumber(string line, int &index, int minus){
    /**
     * Finds the largest number by checking for 9 then 8 then 7 and so on. 
     * 
     * @param line - the grid.
     * @param index - the index we are checking.
     * @param minus - how far away from the edge of the string we are allowed to look.
     * 
     * @returns the largest number given the restrictions.
     */

    for(int largest=9; largest >0; largest--){
        // we make sure we start after the previous largest number was found but also make sure we don't choose a value too close to the right end of the number
        for(int i = index; i < line.length()-minus; i++){
            
            if(stoi(line.substr(i,1)) == largest){
                index = i+1;
                return largest;
            }
        }
    }
    return 0;
}


int main(){
    long long total = 0;
    int index1 = 0;
    string x, c;
    // reading line by line from a file this time because it was cleaner than copying and pasting
    for(string line; getline(input, line); )
    {
        
        for(int i = 0; i < 12; i++){
            c.append(to_string(findBiggestNumber(line, index1, 11-i)));
        }
        total += stoll(c);
   
        // rest c and index
        c = "";
        index1 = 0;
    }
    
    cout << total;
    return 0;
}