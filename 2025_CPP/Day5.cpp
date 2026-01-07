#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
std::ifstream input("thefile.txt");
using namespace std;

bool checkInRange(string range, long long number){
    int index = (int)(strchr(range.c_str(), '-')- range.c_str());

    long long smallerNumber = stoll(range.substr(0,index));
    long long largerNumber = stoll(range.substr(index+1));

    if (smallerNumber>number || largerNumber<number){
        return false;
    }
    return true;
}


vector<string> combineRanges(vector<string> ranges, vector<string> ranges2){
    for(int i =0; i< ranges.size(); i++){
        bool found = false;
        for(int j = 0; j < ranges2.size(); j++){
            int index = (int)(strchr(ranges[i].c_str(), '-')- ranges[i].c_str());

            long long smallerNumber = stoll(ranges[i].substr(0,index));
            long long largerNumber = stoll(ranges[i].substr(index+1));

            bool sInRange = checkInRange(ranges2[j], smallerNumber);
            bool lInRange = checkInRange(ranges2[j], largerNumber);

            index = (int)(strchr(ranges2[j].c_str(), '-')- ranges2[j].c_str());

            string finalSmallerNumber = (ranges2[j].substr(0,index));
            string finalLargerNumber = (ranges2[j].substr(index+1));

            if (sInRange && lInRange){
                cout << "both ";
                cout << smallerNumber;
                cout << " and ";
                cout << largerNumber;
                cout << " are in the range ";
                cout << ranges2[j];
                cout << "\n";
                found = true;
            }
            else if (sInRange){
                found = true;

                cout << smallerNumber;
    
                cout << " is in the range ";
                cout << ranges2[j];
                cout << "\n";
                ranges2[j] = finalSmallerNumber.append("-").append(to_string(largerNumber));
            }
            else if (lInRange){
                cout << largerNumber;
    
                cout << " is in the range ";
                cout << ranges2[j];
                cout << "\n";

                found = true;
                ranges2[j] = to_string(smallerNumber).append("-").append(finalLargerNumber);
            }

            else if (smallerNumber < stoll(finalSmallerNumber) && largerNumber >stoll(finalLargerNumber)){
                ranges2[j] = to_string(smallerNumber).append("-").append(to_string(largerNumber));
            }
        }
        if(found == false){
            ranges2.push_back(ranges[i]);
        }
    }
    return ranges2;
}
long long countEm(vector<string> finalRanges){
    long long count = 0;
    for(int i =0; i< finalRanges.size(); i++){
        int index = (int)(strchr(finalRanges[i].c_str(), '-')- finalRanges[i].c_str());
        long long smallerNumber = stoll(finalRanges[i].substr(0,index));
        long long largerNumber = stoll(finalRanges[i].substr(index+1));
        count = count + (largerNumber-smallerNumber)+1;
    }
    return count;
}

void printRanges(vector<string> arr){
    for(int i = 0; i< arr.size();i++){
        cout << arr[i];
        cout << "\n";
    }

}

int main(){
    vector<string> ranges;
    vector<long long> numbers;
    // save ranges
    for( std::string line; getline(input, line);){
        if(line.length() == 0){
            break;
        }
        ranges.push_back(line);
    }
    // save numbers
    for( std::string line; getline(input, line);){
        numbers.push_back(stoll(line));
    }

    // for part one

    int n = 0;
    for(int i = 0; i < numbers.size(); i++){
        for(int j = 0; j< ranges.size(); j++){
            if (checkInRange(ranges[j], numbers[i])){

                n++;
                break;
            }
        }
    }
    cout << n;
    
    // for part two
    vector<string> finalRanges;
    while(true){
        finalRanges = combineRanges(ranges, finalRanges);
        if (finalRanges.size() == ranges.size()){
            break;
        }
        // cout << countEm(finalRanges);
        // printRanges(finalRanges);
        cout << "\n";
        cout << ranges.size();
        cout << "\n";
        ranges = finalRanges;
        finalRanges.clear();

    }

    // now we count
    printRanges(finalRanges);
    cout << "\n";
    cout << countEm(finalRanges);
}