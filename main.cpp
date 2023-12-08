#include <iostream>
#include <string>
#include <thread>
#include "./calculator/Calculator.h"

using namespace std; 

int main() {

    auto loop = [](){
        Calculator calc = Calculator();
        cout << "Enter a math expression that can be made out of \"any number, any operator (/,*,+,-), or parentheses.\"" << endl;
        cout << "Enter \"q\" of \"quit\" to exit the program." << endl;
        while(true){
            string str;
            getline(cin, str);
            if(str[0] == 'q') break;
            calc.run(str);
            calc.clear_all();
        }
    };

    thread loop_thread{loop};
    loop_thread.join();

    return 0;
}