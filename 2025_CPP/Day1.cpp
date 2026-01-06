#include <iostream>
#include <string>
using namespace std;

class Dial {
    public:
        int value;
        int restCounter;
        int clickCounter;
        bool ignore;
        Dial(){
            value = 50;
            restCounter = 0;
            clickCounter = 0;
            ignore = false;
        }
        void rotate(string input){
            /**
             * Rotates the dial according to the instruction. 
             * 
             * @param input - the instruction input.
             */
                    
            // We know that the input is of the form "ANN" where the first value is a either L or R and everything following is a number
            if (input.length() < 2) return;
            if (input.substr(0,1) != "R" && input.substr(0,1) != "L") return;

            int rotNum = stoi(input.substr(1));
            if (input.substr(0,1) == "R"){
                value += rotNum;
                // we know that the value before operation was between 0 and 99 so for every 100 we know the dial went past 0 that many times
                clickCounter += value/100;
            }else{
                value -= rotNum;
                // we know that the value before operation was between 0 and 99 
                // if the value becomes negative then we have gone past 0 1+ (-value/100) times
                if(value <= 0){
                    if (ignore){
                        clickCounter += (-value)/100;
                    }else{
                        clickCounter += 1 +(-value)/100;
                    }
                }
            }
            
            value = value%100;

            if (value == 0){ 
                restCounter = restCounter +1;
                ignore = true;
            }else{
                ignore = false;
            }
            if (value < 0){
                value = 100+value;
            }
        }
};

int main() {
    
    Dial dial;
    string x;
    while(true){
        cin >> x;
        dial.rotate(x);

        if (x == "Q"){
            cout << dial.restCounter;
            cout << "\n";
            cout << dial.clickCounter;
            cout << "\n";

            return 0;
        }
    }
    return 0;
}