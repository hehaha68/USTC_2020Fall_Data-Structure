#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "priority_queue.h"

typedef struct Node {//Huffman结构
    long    w;//w为权值
    short   p, l, r,add; //p为parent，l为左孩子，r为右孩子
}HTNode, * HTnp;//动态分配数组存储哈夫曼树

typedef struct Huffman_Code {//Huffman编码表结构
    unsigned char len;
    unsigned char* codestr;
}HFCode;

typedef char** HuffmanCode;//动态分配数组存储哈夫曼编码表

int compare(const Node&a, const Node&b) {
    if (a.w > b.w) return 1;
    else if (a.w < b.w) return -1;
    else  return 0;
}

long Frequency(FILE* in, long frequency[]) { //频率数据
    int     i, read_len;
    unsigned char   buf[256];
    long    filesize;

    for (i = 0; i < 256; i++)   frequency[i] = 0;
    fseek(in, 0L, SEEK_SET);
    read_len = 256;

    while (read_len == 256) {
        read_len = fread(buf, 1, 256, in);
        for (i = 0; i < read_len; i++)  frequency[*(buf + i)]++;
    }
    for (i = 0, filesize = 0; i < 256; i++) filesize += frequency[i];

    return filesize;
}

//int Select(HTnp ht, int n, int* s1, int* s2) { //选取结点权值最小的树
//    int i, x;
//    long minValue = 1000000, min = 0;
//    for (x = 0; x < n; x++)
//        if (ht[x].p == -1)  break;
//
//    for (i = 0; i < n; i++) {
//        if (ht[i].p == -1 && ht[i].w < minValue) {
//            minValue = ht[i].w;
//            min = i;
//        }
//    }
//    *s1 = min;
//
//    minValue = 1000000, min = 0;
//    for (i = 0; i < n; i++) {
//        if (ht[i].p == -1 && ht[i].w < minValue && i != *s1) {
//            minValue = ht[i].w;
//            min = i;
//        }
//    }
//
//    *s2 = min;
//    return 1;
//}

int Create_HFTree(long w[], int n, HTNode ht[]) { //创建哈夫曼树
    int m, i;
    int s1, s2;
    HTNode S1, S2;
    Priority_Queue<Node> Q;

    if (n < 1)    return -1;
    m = 2 * n - 1; 

    Priority_Queue<Node>::init(Q, n, compare);

    if (ht == NULL)   return -1;
    for (i = 0; i < n; i++) {
        ht[i].w = w[i];
        ht[i].p = ht[i].l = ht[i].r = -1;
        ht[i].add = i;
        Priority_Queue<Node>::enqueue(Q, ht[i]);
    }
    for (; i < m; i++)
        ht[i].w = ht[i].p = ht[i].l = ht[i].r = -1;

    for (i = n; i < m; i++) {
        //Select(ht, i, &s1, &s2);
        //ht[s1].p = ht[s2].p = i;
        //ht[i].l = s1;
        //ht[i].r = s2;
        //ht[i].w = ht[s1].w + ht[s2].w;
        Priority_Queue<Node>::dequeue(Q, S1);
        Priority_Queue<Node>::dequeue(Q, S2);
        s1 = S1.add;
        s2 = S2.add;
        ht[s1].p = ht[s2].p = i;
        ht[i].l = s1;
        ht[i].r = s2;
        ht[i].w = ht[s1].w + ht[s2].w;
        ht[i].add = i;
        Priority_Queue<Node>::enqueue(Q, ht[i]);
    }
    Priority_Queue<Node>::destroy(Q);
    return 0;
}

int Encode_HFTree(HTnp htp, int n, HFCode hc[]) { //编码哈夫曼树
    int i, j, p, codelen;
    unsigned char* code = (unsigned char*)malloc(n * sizeof(unsigned char));

    if (code == NULL) return -1;
    for (i = 0; i < n; i++) {
        for (p = i, codelen = 0; p != 2 * n - 2; p = htp[p].p, codelen++)
            code[codelen] = (htp[htp[p].p].l == p ? 0 : 1);
        if ((hc[i].codestr = (unsigned char*)malloc((codelen) * sizeof(unsigned char))) == NULL)
            return -1;
        hc[i].len = codelen;
        for (j = 0; j < codelen; j++)
            hc[i].codestr[j] = code[codelen - j - 1];
    }
    free(code);
    return 0;
}

void Mini_HFTree(FILE* in, short mini_ht[][2]) {
    int i;
    for (i = 0; i < 256; i++)
        mini_ht[i][0] = mini_ht[i][1] = -1;
    fread(mini_ht[i], sizeof(short), 2 * (256 - 1), in);
}
