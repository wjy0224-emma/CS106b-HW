#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/* 初始化空的二叉最小堆，并分配初始容量的动态数组。 */
PQHeap::PQHeap() {
    usedItem=0;
    allocatedItem=INITIAL_CAPACITY;
    minHeap=new DataPoint[allocatedItem];
}

/* 释放最小堆持有的动态数组，避免内存泄漏。 */
PQHeap::~PQHeap() {
    delete [] minHeap;
}

/* 将新元素追加到堆尾并上浮，恢复最小堆性质；容量不足时先扩容。 */
void PQHeap::enqueue(DataPoint elem) {
    if(usedItem==allocatedItem){
        expand();
    }
    // 新元素先放到完全二叉树的最后
    minHeap[usedItem]=elem;
    usedItem++;
    // 再恢复 min-heap 性质
    bubbleUp();
}

/* 返回堆顶的最小优先级元素但不删除；空堆时报告错误。 */
DataPoint PQHeap::peek() const {
    if(isEmpty()){
        error("cannot look at empty heap");
    }
    return minHeap[0];
}

/* 删除并返回堆顶元素，用末尾元素补位后下沉以恢复最小堆性质。 */
DataPoint PQHeap::dequeue() {
    if(isEmpty()){
        error("cannot dequeue an empty heap");
    }
    DataPoint result=minHeap[0];

    minHeap[0]=minHeap[usedItem-1];
    usedItem--;

    if(!isEmpty()){
        bubbleDown();
    }
    return result;
}

/* 判断堆中是否没有元素。 */
bool PQHeap::isEmpty() const {
    return usedItem==0;
}

/* 返回堆中当前保存的元素数量。 */
int PQHeap::size() const {
    return usedItem;
}

/* 将逻辑元素数量清零；保留已分配数组供后续复用。 */
void PQHeap::clear() {
    usedItem=0;
}

/* 输出已使用数量、已分配容量和各数组槽位，辅助检查堆结构。 */
void PQHeap::printDebugInfo() {
    cout << "usedItem = " << usedItem << endl;
    cout << "allocatedItem = " << allocatedItem << endl;

    for (int i = 0; i < usedItem; i++) {
        cout << i << ": "
             << minHeap[i]
             << endl;
    }
}

/* 检查元素数量范围，并验证每个父节点的优先级不大于其子节点。 */
void PQHeap::validateInternalState() {
     if (usedItem < 0 || usedItem > allocatedItem) {
        error("Invalid heap size.");
    }

    for (int i = 0; i < usedItem; i++) {

        int leftIndex = getLeftChildIndex(i);
        int rightIndex = getRightChildIndex(i);

        if (leftIndex < usedItem &&
            minHeap[i].priority > minHeap[leftIndex].priority) {

            error("Heap property violated.");
        }

        if (rightIndex < usedItem &&
            minHeap[i].priority > minHeap[rightIndex].priority) {

            error("Heap property violated.");
        }
    }
}

/* 根据数组下标返回当前节点的父节点下标。 */
int PQHeap::getParentIndex(int curIndex) {
    return (curIndex-1)/2;
}

/* 根据数组下标返回当前节点的左孩子下标。 */
int PQHeap::getLeftChildIndex(int curIndex) {
    return 2*curIndex+1;
}

/* 根据数组下标返回当前节点的右孩子下标。 */
int PQHeap::getRightChildIndex(int curIndex) {
    return 2*curIndex+2;
}

/* 将底层动态数组容量扩大为原来的两倍，并复制现有堆元素。 */
void PQHeap::expand() {
    int newAlocatedItem=2*allocatedItem;

    DataPoint* newHeap=new DataPoint[newAlocatedItem];
    for(int i=0;i<usedItem;i++){
        newHeap[i]=minHeap[i];
    }
    delete [] minHeap;

    minHeap=newHeap;
    allocatedItem=newAlocatedItem;

}

/* 让新加入的末尾元素沿父节点方向上浮，直到满足最小堆性质。 */
void PQHeap::bubbleUp() {
    int curIndex=usedItem-1;

    while(curIndex>0){
        int parentIndex=getParentIndex(curIndex);

        if(minHeap[curIndex].priority<minHeap[parentIndex].priority){
            DataPoint temp=minHeap[curIndex];
            minHeap[curIndex]=minHeap[parentIndex];
            minHeap[parentIndex]=temp;
            curIndex=parentIndex;
        }
        else{
            break;
        }
    }
}

/* 让堆顶元素与较小的孩子交换并持续下沉，直到满足最小堆性质。 */
void PQHeap::bubbleDown() {
    int curIndex=0;

    while(getLeftChildIndex(curIndex)<usedItem){
        int leftIndex=getLeftChildIndex(curIndex);
        int rightIndex=getRightChildIndex(curIndex);

        int smallerChildrenIndex=leftIndex;

        if(rightIndex<usedItem && minHeap[rightIndex].priority<minHeap[leftIndex].priority){
            smallerChildrenIndex=rightIndex;
        }

        if(minHeap[smallerChildrenIndex].priority<minHeap[curIndex].priority){
            DataPoint temp = minHeap[curIndex];
            minHeap[curIndex] = minHeap[smallerChildIndex];
            minHeap[smallerChildIndex] = temp;

            curIndex = smallerChildIndex;
        }
        else{
            break;
        }
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */




/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Newly-created heap is empty.") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

PROVIDED_TEST("Provided Test: Enqueue / dequeue single element (two cycles)") {
    PQHeap pq;
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
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Dequeue / peek on empty priority queue throws error") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Dequeue / peek on recently-cleared priority queue throws error") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Enqueue elements in sorted order.") {
    PQHeap pq;
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
    PQHeap pq;
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
    PQHeap pq;
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

    PQHeap pq;
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
    PQHeap pq;
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
    PQHeap pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
}

PROVIDED_TEST("Test enqueue/dequeue of longer random sequence") {
    PQHeap pq;

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
    PQHeap pq;
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
    PQHeap pq;
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

/* 用随机元素填充堆后全部取出，验证边界值、顺序和最终大小。 */
static void fillAndEmpty(int n) {
    PQHeap pq;
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

PROVIDED_TEST("Provided Test: Stress Test. Time the amount of time it takes to cycle many elements in and out. Should take at most about 5-10 seconds.") {
    TIME_OPERATION(20000, fillAndEmpty(20000));
}
