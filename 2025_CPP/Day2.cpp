#include <iostream>
#include <string>
using namespace std;

long long checkRangePart2(string num1, string num2){
    long long total = 0;
    int length;
    string word;
    string portion;
    bool weAreChecking = true;
    bool numberChecked = false;
    // otherwise, we check each number from num1 to num2
    for(long long i = stoll(num1); i <= stoll(num2); i++){
        numberChecked = false;
        word = to_string(i);
        length = word.length();
        for(int div = 2; div <= length;div++){


            // if (numberChecked == true){
            //     numberChecked = false;
            //     break;
            // }
            if (length%div == 0){
                portion = word.substr(0,length/div);

                cout << portion;
                weAreChecking = true;
                for(int nextOne = 1; nextOne < div; nextOne++){
                    cout << " and ";
                    cout << word.substr(nextOne*(length/div),length/div);

                    if(word.substr(nextOne*(length/div),length/div)!=portion){
                        cout << " Nope \n";
                        weAreChecking = false;
                        break;
                    }
                }
                if(weAreChecking==true){
                    cout << "\n";
                    cout << i;
                    cout << "\n";
                    total += i;
                    break;
                }
                else{
                    weAreChecking=true;
                    // break;
                }
            }
        }
    }
    return total;
}

long long checkRange(string num1, string num2){
    long long total = 0;
    int length;
    string word;
    // otherwise, we check each number from num1 to num2
    for(long long i = stoll(num1); i <= stoll(num2); i++){
        word = to_string(i);
        length = word.length();
        if (length%2 == 0){
            if(word.substr(0,length/2) == word.substr(length/2)){
                total += i;
                cout << i;
                cout << "\n";
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
            cout << "\n";
            cout << workingString.substr(0, p);
            cout << "\n";
            cout << workingString.substr(p+1);
            cout << "\n\n";
            total += checkRangePart2(workingString.substr(0, p), workingString.substr(p+1));
            workingString = "";
        }else{
            workingString.append(x.substr(i,1));
        }
    }
    cout << total;
}