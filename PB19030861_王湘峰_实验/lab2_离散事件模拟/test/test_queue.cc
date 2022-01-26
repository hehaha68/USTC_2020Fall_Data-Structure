#include "../queue.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

std::random_device rd;
std::mt19937 generator(rd());

TEST_CASE("Simple queue", "[queue]") {
    Queue<int> q;
    Queue<int>::init(q, 3);
    int e;
    
    REQUIRE(Queue<int>::length(q) == 0);
    REQUIRE(Queue<int>::empty(q));
    REQUIRE(!Queue<int>::full(q));
    
    REQUIRE(Queue<int>::enqueue(q, 5));
    REQUIRE(Queue<int>::enqueue(q, 1));
    REQUIRE(Queue<int>::dequeue(q, e));
    REQUIRE(e == 5);
    REQUIRE(Queue<int>::top(q, e));
    REQUIRE(e == 1);
    REQUIRE(Queue<int>::length(q) == 1);

    REQUIRE(Queue<int>::enqueue(q, 3));
    REQUIRE(Queue<int>::enqueue(q, 2));
    REQUIRE(Queue<int>::full(q));
    REQUIRE(!Queue<int>::enqueue(q, 4));
    REQUIRE(Queue<int>::full(q));
    REQUIRE(Queue<int>::top(q, e));
    REQUIRE(e == 1);
    REQUIRE(!Queue<int>::empty(q));

    REQUIRE(Queue<int>::dequeue(q, e));
    REQUIRE(e == 1);
    REQUIRE(Queue<int>::length(q) == 2);
    REQUIRE(Queue<int>::dequeue(q, e));
    REQUIRE(e == 3);
    REQUIRE(Queue<int>::dequeue(q, e));
    REQUIRE(e == 2);
    REQUIRE(!Queue<int>::dequeue(q, e));
    REQUIRE(e == 2);
    
    Queue<int>::destroy(q);
}

TEST_CASE("Complex queue", "[queue]") 
{
    int in = 0;
    int out = 0;
    std::bernoulli_distribution d(0.5);

    Queue<int> q;
    Queue<int>::init(q, 5);
    int e;

    for(int i=0; i<100; i++) 
    {
        if(d(generator))
        {
            REQUIRE(Queue<int>::enqueue(q, in) ^ in - out >= 5);
            if(in - out < 5)
            {
                in++;
            } 
            else
            {
                REQUIRE(Queue<int>::full(q));
            }
            
        }
        else
        {
            REQUIRE(Queue<int>::dequeue(q, e) ^ in == out);
            if(in > out) {
                REQUIRE(e == out);
                out++;
            }
            else
            {
                REQUIRE(Queue<int>::empty(q));
            }
            
        }

        REQUIRE(Queue<int>::length(q) == in - out);
    }

    Queue<int>::destroy(q);
}