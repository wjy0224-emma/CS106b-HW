#include "pqsortedarray.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/* 初始化空优先队列，并分配初始容量的动态数组。 */
PQSortedArray::PQSortedArray() {
    allocatedCapacity=INITIAL_CAPACITY;
    numItems=0;
    elements=new DataPoint[allocatedCapacity];
}

/* 释放优先队列持有的动态数组，避免内存泄漏。 */
PQSortedArray::~PQSortedArray() {
    delete [] elements;
}

/* 返回元素在降序数组中的插入位置，使最小优先级始终位于数组末尾。 */
int PQSortedArray:: findInsertPlace(DataPoint elem) const{
   for (int i=0;i<numItems;i++){
        if(elem.priority>=elements[i].priority){
            return i;
            //这里找到第一个位置之后立刻停下来
        }
    }
    return numItems;
}
/* 将元素插入有序数组；必要时扩容，并移动后续元素保持优先级降序。 */
void PQSortedArray::enqueue(DataPoint elem) {

    if(numItems==allocatedCapacity){
        expand();
    }

    int insertPlace=findInsertPlace(elem);

    for(int i=numItems;i>insertPlace;i--){
        elements[i]=elements[i-1];
    }

    elements[insertPlace]=elem;
    numItems++;
}

/* 返回当前队列中的元素数量。 */
int PQSortedArray::size() const {
    return numItems;
}

/* 返回数组末尾的最小优先级元素但不删除；空队列时报告错误。 */
DataPoint PQSortedArray::peek() const {
    if(isEmpty()){
        error('cannot peek at an empty priority queue.');
    }
    return elements[numItems-1];
}

/* 删除并返回数组末尾的最小优先级元素；空队列时报告错误。 */
DataPoint PQSortedArray::dequeue() {
    if(isEmpty()){
        error("cannot dequeue from an empty priority queue.");
    }

    DataPoint result=elements[numItems-1];
    numItems--;
    return result;
}

/* 判断队列是否不含任何元素。 */
bool PQSortedArray::isEmpty() const {
    return numItems==0;
}

/* 将逻辑元素数量清零；保留已分配数组供后续复用。 */
void PQSortedArray::clear() {
    numItems=0;
}

/* 将动态数组容量扩大为原来的两倍，并复制现有元素。 */
void PQSortedArray::expand() {
    int newCapacity=allocatedCapacity*2;
    DataPoint* new_elements=new DataPoint[newCapacity];
    for (int i=0;i<numItems;i++){
        new_elements[i]=elements[i];
    }
    delete [] elements;
    elements=new_elements;
    allocatedCapacity=newCapacity;
}


/* 输出内部数组状态，辅助调试；当前实现不产生输出。 */
void PQSortedArray::printDebugInfo() {
}

/* 检查元素数量与容量是否合法，并验证数组按优先级降序排列。 */
void PQSortedArray::validateInternalState() {
     if (numItems > allocatedCapacity) {
        error("Number of items exceeds allocated capacity.");
    }

    for (int i = 0; i < numItems - 1; i++) {
        if (elements[i].priority < elements[i + 1].priority) {
            error("Elements are not in decreasing priority order.");
        }
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */




/* * * * * Provided Tests Below This Point * * * * */

//PROVIDED_TEST("Provided Test: Newly-created heap is empty.") {
//    PQSortedArray pq;

//    EXPECT(pq.isEmpty());
//    EXPECT(pq.size() == 0);
//}

PROVIDED_TEST("Provided Test: Enqueue / dequeue single element (two cycles)") {
    PQSortedArray pq;
    DataPoint point = { "Programming Abstractions", 106 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Test clear operation works with single element."){
    PQSortedArray pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Dequeue / peek on empty priority queue throws error") {
    PQSortedArray pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Dequeue / peek on recently-cleared priority queue throws error") {
    PQSortedArray pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Enqueue elements in sorted order.") {
    PQSortedArray pq;
    for (int i = 0; i < 25; i++) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 25);
    for (int i = 0; i < 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Enqueue elements in reverse-sorted order.") {
    PQSortedArray pq;
    for (int i = 25; i >= 0; i--) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 26);
    for (int i = 0; i <= 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert ascending and descending sequences.") {
    PQSortedArray pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert random sequence of elements.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    PQSortedArray pq;
    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            charToString('A' + i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}


PROVIDED_TEST("Provided Test: Insert duplicate elements.") {
    PQSortedArray pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        DataPoint one = pq.dequeue();
        DataPoint two = pq.dequeue();

        EXPECT_EQUAL(one.priority, i);
        EXPECT_EQUAL(two.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Handles data points with empty string name.") {
    PQSortedArray pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
}

PROVIDED_TEST("Test enqueue/dequeue of longer random sequence") {
    PQSortedArray pq;

    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(0, 100);
        DataPoint pt = {"elem" + integerToString(randomValue), randomValue};
        pq.enqueue(pt);
    }
    EXPECT_EQUAL(pq.size(), 100);
    DataPoint last = {"", -1};
    for (int i = 0; i < 100; i++) {
        DataPoint cur = pq.dequeue();
        EXPECT(cur.priority >= 0 && cur.priority <= 100 && cur.priority >= last.priority);
        last = cur;
    }
    EXPECT_EQUAL(pq.size(), 0);
}


PROVIDED_TEST("Provided Test: Handles data points with negative weights.") {
    PQSortedArray pq;
    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
}

PROVIDED_TEST("Provided Test: Interleave enqueues and dequeues.") {
    PQSortedArray pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

/* 用随机元素填充队列后全部取出，验证边界值、顺序和最终大小。 */
static void fillAndEmpty(int n) {
    PQSortedArray pq;
    DataPoint max = {"max", 106106106};
    DataPoint min = {"min", -106106106};

    pq.enqueue(min);
    pq.enqueue(max);
    for (int i = 0; i < n; i++) {
        int randomPriority = randomInteger(-10000, 10000);
        pq.enqueue({ "", randomPriority });
    }
    EXPECT_EQUAL(pq.size(), n + 2);

    EXPECT_EQUAL(pq.dequeue(), min);
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.dequeue(), max);
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Provided Test: Stress Test. Time the amount of time it takes to cycle many elements in and out. Should take at most a couple seconds.") {
    TIME_OPERATION(10000, fillAndEmpty(10000));
}

/* 向队列加入 n 个按优先级递增的数据点，用于性能测试。 */
static void fillQueue(PQSortedArray& pq, int n){
    for (int i = 0; i < n; i++){
        pq.enqueue({ "", i });
    }
}

/* 从队列中连续移除 n 个元素，用于性能测试。 */
static void emptyQueue(PQSortedArray& pq, int n){
    for (int i = 0; i < n; i++){
        pq.dequeue();
    }
}

PROVIDED_TEST("Provided Test: Introductory test for timing analysis. Uses both fillQueue and emptyQueue functions."){
    PQSortedArray pq;
    TIME_OPERATION(10000, fillQueue(pq, 10000));
    TIME_OPERATION(10000, emptyQueue(pq, 10000));
}

// TODO: add your own STUDENT_TEST cases here.
