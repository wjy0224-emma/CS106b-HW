#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/** 使用二叉最小堆实现的 DataPoint 优先队列。 */
class PQHeap {
public:
    /** 创建一个空优先队列。 */
    PQHeap();

    /** 释放优先队列分配的动态内存。 */
    ~PQHeap();

    /**
     * 将元素加入队列，时间复杂度为 O(log n)。
     * @param element 要加入的数据点。
     */
    void enqueue(DataPoint element);

    /**
     * 删除并返回优先级最小的队首元素；空队列时调用 error()。
     * 时间复杂度为 O(log n)。
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

    /** 输出内部堆数组和容量信息，仅用于调试。 */
    void printDebugInfo();

private:

    DataPoint* minHeap;
    int usedItem;
    int allocatedItem;

    /** 检查内部数组的容量范围和最小堆性质。 */
    void validateInternalState();

    /** 计算数组表示中父节点、左孩子和右孩子的下标。 */
    int getParentIndex(int curIndex);
    int getLeftChildIndex(int curIndex);
    int getRightChildIndex(int curIndex);

    /** 扩大底层动态数组容量。 */
    void expand();
    /** 将末尾元素上浮以恢复最小堆性质。 */
    void bubbleUp();
    /** 将堆顶元素下沉以恢复最小堆性质。 */
    void bubbleDown();

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQHeap);
};
