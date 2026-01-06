#include <iostream>
#include <string>
using namespace std;

long long checkRangePart2(string num1, string num2){
    /**
     * sums all of the invalid IDs in the range according to the part 2 spec. 
     * 
     * @param num1 - the start of the range.
     * @param num2 - the end of the range.
     * 
     * @returns the sum of invalid IDs in the range.
     */

    long long total = 0;
    int length;
    string word;
    string portion;
    bool weAreChecking = true;
    // for each value in the range we check.
    for(long long i = stoll(num1); i <= stoll(num2); i++){
        word = to_string(i);
        length = word.length();
        // we also check each valid number we can split the string into to make equal parts.
        for(int div = 2; div <= length;div++){

            // checking validity
            if (length%div == 0){
                // store for comparisons
                portion = word.substr(0,length/div);
                weAreChecking = true;

                // for each subset, we check if the subset equals the variable 'portion'. If not then this is not a repeating pattern and we can skip this division number.
                for(int nextOne = 1; nextOne < div; nextOne++){
                    if(word.substr(nextOne*(length/div),length/div)!=portion){
                        weAreChecking = false;
                        break;
                    }
                }
                // if we reach this point and find that 'weAreChecking' has not been set to false then we have checked that each portion is equal and can add to the total.
                if(weAreChecking==true){
                    total += i;

                    // important to break here so that we skip the rest of the checks. We don't want to end up adding the same number twice 
                    // e.g. 1111 can be split into two equal part 11 and 11 or four equal parts but we only want to add it to the total once.
                    break;
                }
                else{
                    weAreChecking=true;
                }
            }
        }
    }
    return total;
}

long long checkRange(string num1, string num2){
       /**
     * sums all of the invalid IDs in the range according to the part 1 spec. 
     * 
     * @param num1 - the start of the range.
     * @param num2 - the end of the range.
     * 
     * @returns the sum of invalid IDs in the range.
     */
    
    long long total = 0;
    int length;
    string word;
    // for each value in the range we check.
    for(long long i = stoll(num1); i <= stoll(num2); i++){
        word = to_string(i);
        length = word.length();
        // if the length is odd then we can skip it as it cannot make two equal parts.
        if (length%2 == 0){
            // if the two parts are equal we add it to the total.
            if(word.substr(0,length/2) == word.substr(length/2)){
                total += i;
            }
        }
    }
    return total;
}

int main(){
    string x;
    cin >> x;
    string workingString = "";
    long long total = 0;
    int p;
    // one at a time we check each range
    for(int i = 0; i < x.length(); i++){
 
        // construct each string
        if(x.substr(i,1) == ","){
            // find where the '-' is and split the string in two around it.
            p = (int)(strchr(workingString.c_str(), '-') - workingString.c_str());
     
            total += checkRangePart2(workingString.substr(0, p), workingString.substr(p+1));
            workingString = "";
        }else{
            workingString.append(x.substr(i,1));
        }
    }
    cout << total;
}