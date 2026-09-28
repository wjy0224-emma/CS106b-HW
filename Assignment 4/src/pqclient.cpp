#include "pqclient.h"
#include "pqsortedarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;

/* 将所有数据点加入最小优先队列，再依次取出，以优先级升序原地排序。 */
void pqSort(Vector<DataPoint>& v) {
    PQHeap pq;

    for (DataPoint elem:v){
        qp.enqueue(elem);
    }
    for(int i=0;i<v.size();i++){
        v[i]=pq.dequeue();
    }
}

/*
 * 返回输入流中优先级最高的至多 k 个数据点，并按优先级降序排列。
 * 扫描过程中只保留 k 个候选项，因此时间复杂度为 O(n log k)，空间复杂度为 O(k)。
 */
Vector<DataPoint> topK(istream& stream, int k) {
    if(k<=0){
        return {};
    }

    PQHeap pq;
    DataPoint elem;

    while(stream>>elem){
        pq.enqueue(elem);

        if(pq.size()>k){
            pq.dequeue();
        }
    }

    Vector<DataPoint> result(pq.size());

    for(int i=result.size()-1;i>=0;i--){
        result[i]=pq.dequeue();
    }

    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* 将一组数据点序列化为输入流，供测试 topK 使用。 */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* 将闭区间 [start, stop] 中的整数包装成数据点并写入测试输入流。 */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", i };
        result << " " << pt << " ";
    }
    return result;
}

/* TODO: Add your own custom tests here! */












/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Test pqSort") {
    Vector<DataPoint> input;
    Vector<int> expectedOutput;
    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(1, 10);
        input.add({ "elem" + integerToString(randomValue), randomValue});
        expectedOutput.add(randomValue);
    }
    expectedOutput.sort();
    pqSort(input);
    for (int i = 0; i < 100; i++) {
        EXPECT_EQUAL(input[i].priority, expectedOutput[i]);
    }
}

PROVIDED_TEST("Time operation pqSort") {
    int startSize = 10000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("Provided Test: Stream 0 elements, ask for top 0") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

PROVIDED_TEST("Provided Test: Stream 0 elements, ask for top 1") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("Provided Test: Stream 0 elements, ask for top many") {
    stringstream stream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, kMany), expected);
}

PROVIDED_TEST("Provided Test: Stream 1 element, ask for top 0") {
    stringstream stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

PROVIDED_TEST("Provided Test: Stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = { { "A" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("Provided Test: Stream 1 element, ask for top many") {
    stringstream stream = asStream({ { "A" , 1 } });
    Vector<DataPoint> expected = { { "A" , 1 } };
    EXPECT_EQUAL(topK(stream, kMany), expected);
}

PROVIDED_TEST("Provided Test: Works in a simple case.") {
    /* Build a stream with some simple elements in it. */
    Vector<DataPoint> input = {
        { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 }
    };
    stringstream stream = asStream(input);

    /* What we should see. */
    Vector<DataPoint> expected = {
        { "D", 4 }, { "C", 3 }
    };

    EXPECT(topK(stream, 2) == expected);


    stream = asStream(input);
    expected = {
        { "D", 4 }, { "C", 3 }, { "B", 2 }
    };

    EXPECT(topK(stream, 3) == expected);

    stream = asStream(input);
    expected = {
        { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 }
    };

    EXPECT(topK(stream, 4) == expected);

    stream = asStream(input);
    EXPECT(topK(stream, 6) == expected);
}

PROVIDED_TEST("Provided Test: Stream contains duplicate elements") {
    Vector<DataPoint> vec = {
        { "" , 1 },
        { "" , 3 },
        { "" , 2 },
        { "" , 1 },
        { "" , 5 },
        { "" , 4 },
        { "" , 2 },
        { "" , 3 },
        { "" , 4 },
        { "" , 5 },
    };
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { vec[4] };
    EXPECT_EQUAL(topK(stream, 1), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4] };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5] };
    EXPECT_EQUAL(topK(stream, 3), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5], vec[5] };
    EXPECT_EQUAL(topK(stream, 4), expected);

    stream = asStream(vec);
    expected = { vec[4], vec[4], vec[5], vec[5], vec[1] };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("Provided Test: Stream contains negative elements") {
    Vector<DataPoint> vec = {
        { "" , 1 },
        { "" , 3 },
        { "" , 2 },
        { "" , -1 },
        { "" , -5 },
        { "" , 4 },
        { "" , -2 },
        { "" , 3 },
        { "" , -4 },
        { "" , 5 },
    };
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { vec[9] };
    EXPECT_EQUAL(topK(stream, 1), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5] };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1] };
    EXPECT_EQUAL(topK(stream, 3), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1], vec[1] };
    EXPECT_EQUAL(topK(stream, 4), expected);

    stream = asStream(vec);
    expected = { vec[9], vec[5], vec[1], vec[1], vec[2] };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("Provided Test: Stream many elements, ask for top 0") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(stream, 0), expected);
}

PROVIDED_TEST("Provided Test: Stream many elements, ask for top 1") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = { { "" , kMany - 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("Provided Test: Stream many elements, ask for top 5") {
    Vector<DataPoint> vec;
    for (int i = 0; i < kMany; i++) vec.add({ "", i });
    stringstream stream = asStream(vec);
    Vector<DataPoint> expected = {
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
        { "" , kMany - 5 }
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("Provided Test: Stress Test – many elements with random values") {
    Vector<int> sorted;
    Vector<DataPoint> points;
    for (int i = 0; i < 10000; i++) {
        int weight = randomInteger(0, 100000);
        sorted.add(weight);
        points.add({ "" , weight});
    }

    stringstream stream = asStream(points);
    sort(sorted.begin(), sorted.end(), greater<int>());
    Vector<DataPoint> result = topK(stream, 10);

    EXPECT_EQUAL(result.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(result[i].priority, sorted[i]);
    }
}

//PROVIDED_TEST("Provided Test: Stress Test – Stream many elements, ask for top half. May take >5 minutes on some computers.") {
//    stringstream stream = asStream(1, kMany);
//    Vector<DataPoint> result = topK(stream, kMany/2);
//    EXPECT_EQUAL(result.size(), kMany/2);
//    EXPECT_EQUAL(result[0].priority, kMany);
//    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
//}

PROVIDED_TEST("Provided Test: Time operation top-k on fixed K") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

PROVIDED_TEST("Student Test: Time operation top-k on fixed N") {
    int size = 200000;
    int startK = 100;
    for (int k = startK; k <= 100*startK; k *= 10) {
        Vector<DataPoint> input;
        for (int i = 0; i < size; i++) {
            input.add({ "", randomInteger(1, size) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(k, topK(stream, k));
    }
}
