/*
 * 完美数相关算法：计算真因数之和、判断与搜索完美数，
 * 并使用欧几里得－欧拉公式生成完美数。
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
#include "perfect.h"
using namespace std;

/* 返回 n 的所有真因数（不含 n 本身）之和。 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* 判断 n 是否为正完美数，即其真因数之和是否等于自身。 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* 穷举并输出区间 [1, stop) 内的所有完美数。 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* 按因数对搜索，以较低的时间复杂度计算 n 的真因数之和。 */
long smarterSum(long n) {
    /* TODO: Fill in this function. */
    long result=1;
    if (n<=1) return 0;

    for (long divisor=2;divisor<=sqrt(n);divisor++){
        if(n%divisor==0 && divisor!=sqrt(n)){
            result+=divisor;
            long paireddivisor=n/divisor;
            if(divisor!=paireddivisor){
                result+=paireddivisor;
            }
        }
        //直接写一个paireddivisor会比每次都要计算开方速度更快，而且浮点数和整数比较不够稳妥
        //else if (n%divisor && divisor==sqrt(n)) result+=divisor;
    }
    return result;
}

/* 使用 smarterSum 判断 n 是否为正完美数。 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return (n!=0) && (n==smarterSum(n));
}

/* 使用优化后的因数求和方法，输出区间 [1, stop) 内的完美数。 */
void findPerfectsSmarter(long stop) {
     for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* 使用欧几里得－欧拉公式返回第 n 个偶完美数；n 非正时返回 0。 */
long findNthPerfectEuclid(long n) {
    if(n<=0){
        return 0;
    }
    long k=1;
    long count=0;//已经找到几个完美数
    long perfectNumber=0;
    while(count<n){
        long mersenneNumber=pow(2,k)-1;

        if(isPrime(mersenneNumber)){
            perfectNumber=pow(2,k-1)*mersenneNumber;
            count++;
        }
        k++;
    }
    return perfectNumber;
}

/* 判断 n 是否为质数。 */
bool isPrime(long n){
    if(n<2){
        return false;
    }
    for (int i=2;i<=sqrt(n);i++){
        if(n%i==0) return false;
    }
    return true;
}


/* * * * * * Test Cases * * * * * */

// TODO: add your STUDENT_TEST test cases here!

/*
 * Here is sample test demonstrating how to use a loop to set the input sizes
 * for a sequence of time trials.
 */
//STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {
//    int smallest = 1000, largest = 8000;
//    for (int size = smallest; size <= largest; size *= 2) {
//        TIME_OPERATION(size, findPerfects(size));
//    }
//}


/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}
