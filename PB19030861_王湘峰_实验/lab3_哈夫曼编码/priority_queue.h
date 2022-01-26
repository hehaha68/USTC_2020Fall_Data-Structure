#ifndef _MY_PRIORITY_QUEUE_H_
#define _MY_PRIORITY_QUEUE_H_

#include <stdlib.h>
#include <assert.h>

template <typename T>
struct Priority_Queue
{
private:
    T* data;
    size_t rear;
    size_t capacity;
    int (*cmp)(const T&, const T&);

public:
    static void init(struct Priority_Queue& self, size_t capacity, int (*cmp)(const T&, const T&));
    static void destroy(struct Priority_Queue& self);
    static bool enqueue(struct Priority_Queue& self, const T& e);
    static bool dequeue(struct Priority_Queue& self, T& e);
    static bool top(const struct Priority_Queue& self, T& e);
    static size_t length(const struct Priority_Queue& self);
    static bool empty(const struct Priority_Queue& self);
    static bool full(const struct Priority_Queue& self);
};

template <typename T>
void Priority_Queue<T>::init(struct Priority_Queue<T>& self, size_t capacity, int (*cmp)(const T& a, const T& b))
{
    self.capacity = capacity;
    self.data = (T*)malloc((capacity + 1) * sizeof(T));
    self.rear = 0;
    self.cmp = cmp;
    assert(self.data != NULL);
}

template <typename T>
void Priority_Queue<T>::destroy(struct Priority_Queue<T>& self)
{
    free(self.data);
}

template <typename T>
bool Priority_Queue<T>::enqueue(struct Priority_Queue<T>& self, const T& e)
{
    if (full(self))
        return false;

    size_t i = ++self.rear;
    for (; i > 1 && self.cmp(self.data[i / 2], e) > 0; i /= 2)
    {
        self.data[i] = self.data[i / 2];
    }
    self.data[i] = e;
    return true;
}

template <typename T>
bool Priority_Queue<T>::dequeue(struct Priority_Queue<T>& self, T& e)
{
    if (empty(self))
        return false;

    e = self.data[1];
    self.rear--;

    int i = 2;
    for (; i <= self.rear; i *= 2)
    {
        if (i < self.rear && self.cmp(self.data[i], self.data[i + 1]) > 0)
            i++;

        if (i <= self.rear && self.cmp(self.data[self.rear + 1], self.data[i]) > 0)
            self.data[i / 2] = self.data[i];
        else
            break;
    }
    self.data[i / 2] = self.data[self.rear + 1];
    return true;
}

template <typename T>
bool Priority_Queue<T>::top(const struct Priority_Queue<T>& self, T& e)
{
    if (empty(self))
        return false;

    e = self.data[1];
    return true;
}

template <typename T>
size_t Priority_Queue<T>::length(const struct Priority_Queue<T>& self)
{
    return self.rear;
}

template <typename T>
bool Priority_Queue<T>::empty(const struct Priority_Queue<T>& self)
{
    return length(self) == 0;
}

template <typename T>
bool Priority_Queue<T>::full(const struct Priority_Queue<T>& self)
{
    return length(self) == self.capacity;
}

#endif