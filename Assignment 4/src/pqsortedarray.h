#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/** 使用有序动态数组实现的 DataPoint 优先队列。 */
class PQSortedArray {
public:
    /** 创建一个空优先队列。 */
    PQSortedArray();

    /** 释放优先队列分配的动态内存。 */
    ~PQSortedArray();

    /**
     * 将元素插入有序数组，时间复杂度为 O(n)。
     * @param element 要加入的数据点。
     */
    void enqueue(DataPoint element);

    /**
     * 删除并返回优先级最小的队首元素；空队列时调用 error()。
     * 时间复杂度为 O(1)。
     */
    DataPoint dequeue();

    /** 返回但不删除最小优先级元素；空队列时调用 error()，复杂度为 O(1)。 */
    DataPoint peek() const;

    /** 判断队列是否为空，时间复杂度为 O(1)。 */
    bool isEmpty() const;

    /** 返回队列中的元素数量，时间复杂度为 O(1)。 */
    int size() const;

    /** 清空队列，时间复杂度为 O(1)。 */
    void clear();

    /** 输出内部数组状态，仅用于调试。 */
    void printDebugInfo();

private:
    DataPoint* elements; // array of elements
    int allocatedCapacity;  // number of slots allocated in array
    int numItems;           // number of slots used in array

    /** 扩大底层动态数组容量。 */
    void expand();
    /** 查找元素在降序数组中的插入位置。 */
    int findInsertPlace(DataPoint elem) const;

    /** 验证容量范围及数组按优先级降序排列的不变量。 */
    void validateInternalState();

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQSortedArray);
};
