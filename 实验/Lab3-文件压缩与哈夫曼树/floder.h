#include<stdio.h>
#include<windows.h>

int Read_Floder(char name[6][100],int flag){//读取文件夹下文件名列表
    char add1[10000]= "D:\\USTC\\Homework\\DS\\huffman\\test\\*";//待压缩
    char add2[10000] = "D:\\USTC\\Homework\\DS\\huffman\\new.huff\\*";//待解压
    int i = 1;
    WIN32_FIND_DATA p;
    HANDLE h;
    if(flag == 1)    h = FindFirstFile(strcat(add1,".txt"), &p);
    else if (flag == 0) h = FindFirstFile(strcat(add1, ".huff"), &p); 
    else if(flag == 2) h = FindFirstFile(strcat(add2, ".huff"), &p);
    puts(p.cFileName);
    strcpy(name[0], p.cFileName);
    while (FindNextFile(h, &p)) {
        puts(p.cFileName);
        strcpy(name[i], p.cFileName);//形成完整路径
        i++;
    }
    return 0;
}
