#pragma once

#include "datapoint.h"
#include "vector.h"
#include <istream>


/** 将所有数据点放入优先队列再依次取出，使向量按优先级升序排列。 */
void pqSort(Vector<DataPoint>& v);


/**
 * 从输入流中选出优先级最高的至多 k 个数据点，并按优先级降序返回。
 * 当 k 大于数据总量 n 时返回全部 n 个元素；相同优先级可采用任意顺序。
 * 目标时间复杂度为 O(n log k)，额外空间复杂度为 O(k)。
 *
 * @param stream 包含 DataPoint 的输入流。
 * @param k 需要保留的最大元素数量。
 * @return min(n, k) 个最高优先级数据点组成的降序向量。
 */
Vector<DataPoint> topK(std::istream& stream, int k);
