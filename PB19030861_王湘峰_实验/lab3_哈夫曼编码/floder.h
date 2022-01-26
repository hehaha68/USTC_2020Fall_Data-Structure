#include<stdio.h>
#include<windows.h>

int Read_Floder(char name[6][100],int flag){
    char add[10000]= "D:\\USTC\\Homework\\DS\\huffman\\test\\*";
    int i = 1;
    WIN32_FIND_DATA p;
    //HANDLE h = FindFirstFile("D:\\USTC\\Homework\\DS\\huffman\\test\\*.txt", &p);
    HANDLE h;
    if(flag)    h = FindFirstFile(strcat(add,".txt"), &p);
    else h = FindFirstFile(strcat(add, ".huff"), &p);
    puts(p.cFileName);
    strcpy(name[0], p.cFileName);
    while (FindNextFile(h, &p)) {
        puts(p.cFileName);
        strcpy(name[i], p.cFileName);
        i++;
    }
    //for (i = 0; i < 6; i++)
    //    puts(name[i]);
    return 0;
}
