#include <stdlib.h>

template <typename T>
struct Priority_Queue
{
private:
    T* data;
    size_t rear;
    size_t capacity;
    int (*cmp)(const T& a, const T& b);

public:
    static void init(struct Priority_Queue& q, size_t capacity, int (*cmp)(const T& a, const T& b));
    static void destroy(struct Priority_Queue& q);
    static bool enqueue(struct Priority_Queue& q, const T& e);
    static bool dequeue(struct Priority_Queue& q, T& e);
    static bool top(const struct Priority_Queue& q, T& e);
    static size_t length(const struct Priority_Queue& q);
    static bool empty(const struct Priority_Queue& q);
    static bool full(const struct Priority_Queue& q);
};

template <typename T>
void Priority_Queue<T>::init(struct Priority_Queue<T>& q, size_t capacity, int (*cmp)(const T& a, const T& b))
{
    q.data = (T*)malloc((capacity + 1) * sizeof(T));
    if (!q.data)    exit(-2);
    q.rear = 0;
    q.capacity = capacity;
    q.cmp = cmp;
}

template <typename T>
void Priority_Queue<T>::destroy(struct Priority_Queue<T>& q)
{
    if (q.data)
        free(q.data);
    q.data = NULL;
    q.rear = 0;
    q.capacity = 0;
}

template <typename T>
bool Priority_Queue<T>::enqueue(struct Priority_Queue<T>& q, const T& e)
{
    size_t i, j;
    if (!full(q))//非满
    {
        q.data[++q.rear] = e;
        for (i = q.rear; i / 2 > 0 && q.cmp(q.data[i / 2], e) >= 0; i /= 2)
            q.data[i] = q.data[i / 2];
        q.data[i] = e;
        return true;
    }
    return false;
}

template <typename T>
bool Priority_Queue<T>::dequeue(struct Priority_Queue<T>& q, T& e)
{
    size_t i, j;
    T temp;
    if (!empty(q))//非空
    {
        e = q.data[1];
        q.data[1] = q.data[q.rear];
        q.rear--;
        temp = q.data[1];
        for (i = 1; i * 2 <= q.rear; i = j)
        {
            j = i * 2;
            if (j != q.rear && q.cmp(q.data[j + 1], q.data[j]) <= 0)
                j++;
            if (q.cmp(temp, q.data[j]) >= 0)   q.data[i] = q.data[j];
            else    break;
        }
        q.data[i] = temp;
        return true;
    }
    return false;
}

template <typename T>
bool Priority_Queue<T>::top(const struct Priority_Queue<T>& q, T& e)
{
    if (empty(q))
        return false;
    e = q.data[1];
    return true;
}

template <typename T>
size_t Priority_Queue<T>::length(const struct Priority_Queue<T>& q)
{
    size_t length;
    if (!empty(q))
    {
        length = q.rear;
        return length;
    }
    return 0;
}

template <typename T>
bool Priority_Queue<T>::empty(const struct Priority_Queue<T>& q)
{
    if (q.rear != 0)
        return false;
    return true;
}

template <typename T>
bool Priority_Queue<T>::full(const struct Priority_Queue<T>& q)
{
    if (q.rear == q.capacity)//满则真
        return true;
    return false;
}