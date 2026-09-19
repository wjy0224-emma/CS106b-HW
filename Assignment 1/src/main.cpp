#include <iostream>
#include "console.h"
#include "SimpleTest.h"
#include "perfect.h"
#include "soundex.h"
using namespace std;


/* 运行选定测试，并启动完美数或 Soundex 作业程序。 */
int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    findPerfects(40000);
    // Comment out the above line and uncomment below line 
    // to switch between running perfect.cpp and soundex.cpp
//    soundexSearch("res/surnames.txt");

    cout << "Back in main(): FINISHED!" << endl;
    return 0;
}


/* 通过编译期调用确认各作业函数符合评分程序要求的函数原型。 */
void confirmFunctionPrototypes() {
    long n = 0;
    bool b;
    string s;

    n = divisorSum(n);
    b = isPerfect(n);
    if (b)
        ;
    findPerfects(n);

    n = smarterSum(n);
    b = isPerfectSmarter(n);
    findPerfectsSmarter(n);

    n = findNthPerfectEuclid(n);

    s = lettersOnly(s);
    s = soundex(s);
    soundexSearch(s);
}
