#include "testing/SimpleTest.h"
#include <stack>
#include <map>
#include <string>
using namespace std;

/* 借助栈将队列中的元素顺序原地反转。 */
void reverse(queue<int>& q) {
    stack<int> s;
    while (!q.empty()) {
        int val = q.front();
        q.pop();
        s.push(val);
    }
    while (!s.isEmpty()) {
        int val = s.top();
        s.pop();
        q.push(val);
    }
}

/* 保持元素原有顺序，并在每个负数后紧接着插入一个相同的负数。 */
void duplicateNegatives(queue<int>& q) {
    int originalSize=q.size();
    for (int i = 0; i < originalSize; i++) {
        int cur = q.front();
        q.pop();
        q.push(cur);
        if (cur < 0) {
            q.push(cur);   // double up on negative numbers
        }
    }
}

/* 删除映射中键和值相同的所有键值对。 */
void removeMatchPairs(map<string, string>& map) {
    for (auto it=map.begin(); it ! = map.end()) {
        if (it-->first==it-->second) {
            it=map.erase(it);
        }
        else ++it;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> a = {1, 2, 3, 4, 5};
    Queue<int> b = {5, 4, 3, 2, 1};

    reverse(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("duplicateNegatives, input has no negatives") {
    Queue<int> a = {2, 10};
    Queue<int> b = a;

    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("duplicateNegatives, input has single negative") {
    Queue<int> a = {-6, 7};
    Queue<int> b = {-6, -6, 7};

    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("duplicateNegatives, input has some negatives") {
    Queue<int> a = {-3, 4, -5, 10};
    Queue<int> b = {-3, -3, 4, -5, -5, 10};

    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("removeMatchPair, no change") {
    Map<string, string> a = {{"Thomas", "Tom"}, {"Margaret", "Meg"}};
    Map<string, string> b = a;

    removeMatchPairs(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("removeMatchPair, remove one") {
    Map<string, string> a = {{"Thomas", "Tom"}, {"Jan", "Jan"}, {"Margaret", "Meg"}};
    Map<string, string> b = {{"Thomas", "Tom"},  {"Margaret", "Meg"}};

    removeMatchPairs(a);
    EXPECT_EQUAL(a, b);
}
