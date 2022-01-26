#include "../priority_queue.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

std::random_device rd;
std::mt19937 generator(rd());

void reshuffule(int *arr, int len)
{
    std::uniform_int_distribution<> d(0, len-1);
    for(int i=0; i<len; i++)
    {
        int j = d(generator);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int int_compare(const int &a, const int &b) {
    if (a > b) {
        return 1;
    }
    else if (a < b) {
        return -1;
    }
    else {
        return 0;
    }
}

TEST_CASE("Simple priority queue", "[priority queue]") {

    Priority_Queue<int> q;
    Priority_Queue<int>::init(q, 3, int_compare);
    int e;

    REQUIRE(Priority_Queue<int>::length(q) == 0);
    REQUIRE(Priority_Queue<int>::empty(q));
    REQUIRE(!Priority_Queue<int>::full(q));

    REQUIRE(Priority_Queue<int>::enqueue(q, 5));
    REQUIRE(Priority_Queue<int>::top(q, e));
    REQUIRE(e == 5);
    REQUIRE(Priority_Queue<int>::enqueue(q, 1));
    
    REQUIRE(Priority_Queue<int>::top(q, e));
    REQUIRE(e == 1);
    REQUIRE(Priority_Queue<int>::enqueue(q, 3));
    REQUIRE(Priority_Queue<int>::top(q, e));
    REQUIRE(e == 1);

    REQUIRE(Priority_Queue<int>::length(q) == 3);
    REQUIRE(!Priority_Queue<int>::empty(q));
    REQUIRE(!Priority_Queue<int>::enqueue(q, 4));
    REQUIRE(Priority_Queue<int>::top(q, e));
    REQUIRE(Priority_Queue<int>::full(q));
    
    REQUIRE(Priority_Queue<int>::dequeue(q, e));
    REQUIRE(e == 1);
    REQUIRE(Priority_Queue<int>::enqueue(q, 2));
    REQUIRE(Priority_Queue<int>::dequeue(q, e));
    REQUIRE(e == 2);

    REQUIRE(Priority_Queue<int>::dequeue(q, e));
    REQUIRE(e == 3);
    REQUIRE(Priority_Queue<int>::length(q) == 1);
    REQUIRE(Priority_Queue<int>::dequeue(q, e));
    REQUIRE(e == 5);
    REQUIRE(!Priority_Queue<int>::dequeue(q, e));
    REQUIRE(e == 5);

    Priority_Queue<int>::destroy(q);
}

TEST_CASE("Complex priority queue", "[queue]") 
{
    int arr[100];
    for(int i=0; i<100; ++i)
    {
        arr[i] = i;
    }
    reshuffule(arr, 25);
    reshuffule(arr + 25, 50);
    reshuffule(arr + 75, 25);

    Priority_Queue<int> q;
    Priority_Queue<int>::init(q, 100, int_compare);
    int e;

    for(int i=25; i<75; i++)
    {
        REQUIRE(Priority_Queue<int>::enqueue(q, arr[i]));
    }

    for(int i=25; i<50; i++)
    {
        REQUIRE(Priority_Queue<int>::dequeue(q, e));
        REQUIRE(e == i);
    }

    for(int i=0; i<25; i++)
    {
        REQUIRE(Priority_Queue<int>::enqueue(q, arr[i]));
        REQUIRE(Priority_Queue<int>::enqueue(q, arr[99-i]));
    }

    for(int i=0; i<25; i++)
    {
        REQUIRE(Priority_Queue<int>::dequeue(q, e));
        REQUIRE(e == i);
    }

    for(int i=50; i<100; i++)
    {
        REQUIRE(Priority_Queue<int>::dequeue(q, e));
        REQUIRE(e == i);
    }

    Priority_Queue<int>::destroy(q);
}