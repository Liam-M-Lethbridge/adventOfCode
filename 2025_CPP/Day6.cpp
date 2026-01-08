#include <string>
#include <vector>
#include <iostream>
#include <fstream>
std::ifstream input("theFile.txt");
using namespace std;

long long compute(vector<string>* arr, vector<string> operators, int length){
    /**
     * Combines numbers according to the corresponding operator and returns the sum of the results.
     * 
     * @param arr - the array of numbers.
     * @param operators - the vector of operators.
     * @param length - the number of rows.
     * 
     * @returns the sum of the results.
     */
    long long total = 0;

    for(int i = 0; i < operators.size(); i++){
        string op = operators[i];
        long long currentOp = stoll(arr[0][i]);

        for(int j = 1; j < length; j++){

            if(op == "+"){
                currentOp = currentOp + stoll(arr[j][i]);
            }
            else{
                currentOp = currentOp * stoll(arr[j][i]);
            }
        }
        total += currentOp;
    }
    return total;
}


bool checkColumnEmpty(vector<string> lines, int line){
    /**
     * checks if the column contains only whitespace.
     * 
     * @param lines - the vector of rows.
     * @param line - the index of the column.
     * 
     * @returns true if only whitespace. False otherwise.
     */
    for(int i = 0; i < lines.size()-1;i++){
        if(lines[i][line] != ' '){
            return false;
        }
    }

    return true;
}


int main(){
    // storing the lengths
    vector<string> lines;

    
    for (string line; getline(input, line);) {
        lines.push_back(line);
    }


    int rows = lines.size();
    int length = lines[rows-1].length();

    vector<string> operators;
    string character;
    string line = lines[rows-1];

    for(int i = 0; i < length; i++){
        character = line[i];

        if(character != " "){
            operators.push_back(character);
        }
    }
    int columns = operators.size();

    // build arrays
    string workingString = "";
    vector<string>* arrOfPtrs = new vector<string>[columns];
    for(int i = 0; i < rows-1; i++){
        string line = lines[i];

        vector<string> row;
        for(int j = 0; j < line.length(); j++){
            character = line[j];

            // if we have an empty column or we have reached the end:
            if(checkColumnEmpty(lines, j)){
                row.push_back(workingString);
                workingString = "";
                
            }
            else{
                workingString.append(character);
                if(j == line.length()-1){
                    row.push_back(workingString);
                    workingString = "";
                }
            }
            
        }
        arrOfPtrs[i] = row;
    }

    //part one
    cout << compute(arrOfPtrs, operators, rows-1);
    cout << "\n";

    //part two
    long long total = 0;
    for(int i = 0; i < operators.size(); i++){
        vector<string> row;
        int count = 0;
        string op = operators[i];
        long long currentOp;
        if(op == "+"){
            currentOp = 0;
        }else{
            currentOp = 1;
        }
        while(true){
            workingString = "";
            for(int j=0; j < rows-1; j++){
                string word = arrOfPtrs[j][i];
                if(count < word.length()){
                    character = word[count];
                    workingString.append(character);
                }
            }
            if( workingString.length() == 0){
                break;
            }
            count +=1;
            if(op == "+"){
                currentOp = currentOp+stoll(workingString);
            }else{
                currentOp = currentOp*stoll(workingString);
            }
        }
        total+=currentOp;
    }
    
    cout << total;

    delete[] arrOfPtrs;
}