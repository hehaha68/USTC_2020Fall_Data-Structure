
/********************************************************************************
** School:  School Of Data Science, USTC
** auth:    PB19010450 和泳毅
** date:    2020/12/24 19:38:28
** desc:    利用Huffman树进行对文件/文件夹的压缩/解压功能
** Ver.:    V3.0.0
*********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include<direct.h>
#include "File.h"
#include "floder.h"

int main(int argc, char* argv[]){
   int i;
   char File_Add[100000] = "D:\\USTC\\Homework\\DS\\huffman\\";
    char floder_name[6][100], floder_name_2[6][100];
    if (argc <= 1)   return -1;
    if (argv[1][2] == 'f') {//////对文件夹压缩
        switch (argv[1][1]) {
        case 'z': {//////压缩
            printf("\nFiles in %s:\n", strcat(File_Add, argv[2]));
            Read_Floder(floder_name,1);//读取文件夹文件列表
            for (i = 0; i < 6; i++) {
                char Floder_Add[100000] = "D:\\USTC\\Homework\\DS\\huffman\\test\\";//文件夹路径(也可以写成自己输入的)
                Compress(strcat(Floder_Add, floder_name[i]), NULL, NULL, NULL);//压缩每一个文件
            }
            
            char A[100000] = "D:\\USTC\\Homework\\DS\\huffman\\";
            mkdir(strcat(A, "new.huff"));//建压缩进的文件夹
            Read_Floder(floder_name_2, 0);//读取原文件夹下.huff文件
            for (i = 0; i < 6; i++) {
                char B[100000] = "D:\\USTC\\Homework\\DS\\huffman\\test\\";
                char C[100000] = "D:\\USTC\\Homework\\DS\\huffman\\new.huff\\";
                MoveFile(strcat(B, floder_name_2[i]), strcat(C, floder_name_2[i]));//移动文件到压缩文件夹下
            }
            break;
        }
        case 'u': {//////解压
            printf("\nFiles in %s:\n", strcat(File_Add, argv[2]));
            Read_Floder(floder_name, 2);//读取压缩文件夹下.huff文件
            char A[100000] = "D:\\USTC\\Homework\\DS\\huffman\\";
            mkdir(strcat(A, "test"));//恢复原文件夹，也可以在压缩时，
                                     //将文件夹名写入一个固定文件名的文本文件进行读取赋名
            for (i = 0; i < 6; i++) {
                char Floder_Add[100000] = "D:\\USTC\\Homework\\DS\\huffman\\new.huff\\";
                Decompress(strcat(Floder_Add, floder_name[i]), NULL, NULL, NULL);//解压文件
                DeleteFile(Floder_Add);//删除.huff文件
            }
            rmdir("D:\\USTC\\Homework\\DS\\huffman\\new.huff");//删除压缩文件夹
            break;
        }
        default:
            printf("INPUT ERROR;\n");
        }
    }
    else {//////对文件压缩
        switch (argv[1][1]) {
        case 'z'://////压缩
            Compress(strcat(File_Add, argv[2]), NULL, argv[1][2], argv[3]);
            break;
        case 'u'://////解压
            Decompress(strcat(File_Add, argv[2]), NULL, argv[1][2], argv[3]);
            remove(File_Add);//删除压缩文件
            break;
        default:
            printf("INPUT ERROR;\n");
        }
    }
    return 0;
}
