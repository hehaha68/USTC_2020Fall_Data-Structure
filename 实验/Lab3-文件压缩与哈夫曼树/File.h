#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "Huffman.h"

/*********函数声明*********/
char* Create_Filename(char* source_filename, char* obj_filename, char re, char* new_filename);//创建文件
int Initial_Compress_Files(char* source_filename, FILE** inp, char* obj_filename, FILE** outp, char re, char* new_filename);//初始化压缩文件
int Initial_Decompress_Files(char* source_filename, FILE** inp, char* obj_filename, FILE** outp, char re, char* new_filename); //重新变成原始文件
int Compress(char* source_filename, char* obj_filename, char re, char* new_filename);//压缩文件
int Decompress(char* source_filename, char* obj_filename, char re, char* new_filename);//解压缩文件
int Write_Compress_File(FILE* in, FILE* out, HTnp ht, HFCode hc[], char* source_filename, long source_filesize);//写入压缩文件
int Write_Decompress_File(FILE* in, FILE* out, short mini_ht[][2], long bits_pos, long obj_filesize);//写入解压文件
unsigned char Chars_To_Bits(const unsigned char chars[8]);//char to bit表示
/*************************/




char* Create_Filename(char* source_filename, char* obj_filename, char re,char* new_filename) { //创建文件
    char* temp;
    char s[100000] = "D:\\USTC\\Homework\\DS\\huffman\\" ;//固定路径
    if ((temp = strrchr(source_filename, '.')) == NULL) { //把原始文件 赋给 临时文件夹 为空
        strcpy(obj_filename, source_filename);
        strcat(obj_filename, ".huff");
    }
    else {
        strncpy(obj_filename, source_filename, temp - source_filename);
        obj_filename[temp - source_filename] = '\0';
        strcat(obj_filename, ".huff");
    }

    if (re == 'r') {//若重命名
        strcat(s, new_filename);
        strcpy(obj_filename,s);
    }
    return obj_filename;
}

int Initial_Compress_Files(char* source_filename, FILE** inp, char* obj_filename, FILE** outp, char re, char* new_filename) { //初始化文件
    if (fopen(source_filename, "rb+") == NULL)   return -1;
    if (obj_filename == NULL) { //压缩文件为空
        if ((obj_filename = (char*)malloc(256 * sizeof(char))) == NULL) //分配空间
            return -1;
        Create_Filename(source_filename, obj_filename,re,new_filename);//创建文件
    }
    if (strcmp(source_filename, obj_filename) == 0)//如果待压缩文件和压缩文件比较后相同
        return -1;  
    if ((*outp = fopen(obj_filename, "wb")) == NULL)
        return -1;
    if ((*inp = fopen(source_filename, "rb+")) == NULL)
        return -1;
    printf("\nSource File:%s\nObject File:%s\n",
        source_filename, obj_filename);//待压缩文件source_filename,压缩文件obj_filename
    free(obj_filename); //关闭
    return 0;
}

int Compress(char* source_filename, char* obj_filename, char re, char* new_filename) { //压缩文件
    FILE* in, * out;
    int flag, i, j;
    HFCode hc[256];
    HTNode  ht[256 * 2 - 1];
    long frequency[256], source_filesize;

    long obj_filesize = 0;
    float compress_rate;

    flag = Initial_Compress_Files(source_filename, &in, obj_filename, &out,re, new_filename);//初始化文件
    if (flag != 0) {
        puts("\nFailed to read flie!\n");
        return flag;
    }
    source_filesize = Frequency(in, frequency);//频率数据
    flag = Create_HFTree(frequency, 256, ht);//建树
    if (flag != 0) {
        puts("\nFailed to build Huffman Tree！\n");
        return flag;
    }
    flag = Encode_HFTree(ht, 256, hc); //建编码
    if (flag != 0) {
        puts("\nFailed to build Huffman code！\n");
        return flag;
    }

    for (i = 0; i < 256; i++)
        obj_filesize += frequency[i] * hc[i].len;
    obj_filesize = obj_filesize % 8 == 0 ? obj_filesize / 8 : obj_filesize / 8 + 1;
    for (i = 0; i < 256 - 1; i++)
        obj_filesize += 2 * sizeof(short);
    obj_filesize += strlen(source_filename) + 1;
    obj_filesize += sizeof(long);
    obj_filesize += sizeof(unsigned int);
    compress_rate = (float)obj_filesize / source_filesize;
    printf("The size of the compressed file is:%ld字节\nThe Compression rate is :%.2lf%%\n", obj_filesize, compress_rate * 100);

    flag = Write_Compress_File(in, out, ht, hc, source_filename, source_filesize);//写入压缩文件
    if (flag != 0) {
        puts("\nFailed to write flie!\n");
        return flag;
    }
    puts("\nCompress Finished!\n");//完成
    fclose(in);
    fclose(out);//关闭文件
    for (i = 0; i < 256; i++)   
        free(hc[i].codestr);
    return 0;
}

unsigned char Chars_To_Bits(const unsigned char chars[8]) { //把char to bit表示
    int i;
    unsigned char bits = 0;
    bits |= chars[0];//与首位或运算
    for (i = 1; i < 8; ++i) {
        bits <<= 1;//二进制左移一位
        bits |= chars[i];//继续或运算下一位
    }
    return bits;
}

int Write_Compress_File(FILE* in, FILE* out, HTnp ht, HFCode hc[], char* source_filename, long source_filesize) { //写入压缩文件
    unsigned int    i, read_counter, write_counter, huff_head = 0xFFFFFFFF;
    unsigned char   write_char_counter, code_char_counter, copy_char_counter,
                    read_buf[256], write_buf[256], write_chars[8], filename_size = strlen(source_filename);
    HFCode* cur_HFCode;

    fseek(in, 0L, SEEK_SET);//文件指针定位到开头
    fseek(out, 0L, SEEK_SET);
    fwrite(&huff_head, sizeof(unsigned int), 1, out);
    fwrite(&filename_size, sizeof(unsigned char), 1, out);
    fwrite(source_filename, sizeof(char), filename_size, out);
    fwrite(&source_filesize, sizeof(long), 1, out);
    for (i = 256; i < 256 * 2 - 1; i++) {
        fwrite(&(ht[i].l), sizeof(ht[i].l), 1, out);
        fwrite(&(ht[i].r), sizeof(ht[i].r), 1, out);
    }
    write_counter = write_char_counter = 0;
    read_counter = 256;
    while (read_counter == 256) {
        read_counter = fread(read_buf, 1, 256, in);
        for (i = 0; i < read_counter; i++) {
            cur_HFCode = &hc[read_buf[i]];
            code_char_counter = 0;
            while (code_char_counter != cur_HFCode->len) {
                copy_char_counter = (8 - write_char_counter > cur_HFCode->len - code_char_counter ?
                    cur_HFCode->len - code_char_counter : 8 - write_char_counter);
                memcpy(write_chars + write_char_counter, 
                        cur_HFCode->codestr + code_char_counter, copy_char_counter);//复制内存
                write_char_counter += copy_char_counter;
                code_char_counter += copy_char_counter;
                if (write_char_counter == 8) {
                    write_char_counter = 0;
                    write_buf[write_counter++] = Chars_To_Bits(write_chars);
                    if (write_counter == 256) {
                        fwrite(write_buf, 1, 256, out);
                        write_counter = 0;
                    }
                }
            }
        }
    }
    fwrite(write_buf, 1, write_counter, out);
    if (write_char_counter != 0) {
        write_char_counter = Chars_To_Bits(write_chars);//位函数的调用，实现压缩功能
        fwrite(&write_char_counter, 1, 1, out);
    }
    return 0;
}

int Write_Decompress_File(FILE* in, FILE* out, short mini_ht[][2], long bits_pos, long obj_filesize) { //写入解压缩
    long    cur_size;
    unsigned char   read_buf[256], write_buf[256], convert_bit;
    unsigned int    read_counter, write_counter, cur_pos;
    fseek(in, bits_pos, SEEK_SET);
    fseek(out, 0L, SEEK_SET);
    read_counter = 256 - 1;
    cur_size = write_counter = 0;
    cur_pos = 256 * 2 - 2;
    while (cur_size != obj_filesize) {
        if (++read_counter == 256) {
            fread(read_buf, 1, 256, in);
            read_counter = 0;
        }
        for (convert_bit = 128; convert_bit != 0; convert_bit >>= 1) {
            cur_pos = ((read_buf[read_counter] & convert_bit) == 0 ? mini_ht[cur_pos][0] : mini_ht[cur_pos][1]);
            if (cur_pos < 256) {
                write_buf[write_counter] = (unsigned char)cur_pos;
                if (++write_counter == 256) {
                    fwrite(write_buf, 1, 256, out);
                    write_counter = 0;
                }
                cur_pos = 256 * 2 - 2;
                if (++cur_size == obj_filesize)
                    break;
            }
        }
    }
    fwrite(write_buf, 1, write_counter, out);
    return 0;
}

int Initial_Decompress_Files(char* source_filename, FILE** inp, char* obj_filename, FILE** outp,char re, char* new_filename) { //重新变成原始文件
    unsigned int    huff_head; //压缩文件的头文件
    unsigned char   filename_size;    //文件大小 
    char s[100000] = "D:\\USTC\\Homework\\DS\\huffman\\";
    if ((*inp = fopen(source_filename, "rb")) == NULL)
        return -1;
    printf("\nSource Flie:%s,", source_filename);
    fread(&huff_head, sizeof(unsigned int), 1, *inp);
    if (huff_head == NULL)
        return -1;
    if (obj_filename == NULL) {
        if ((obj_filename = (char*)malloc(256 * sizeof(char))) == NULL)//分配
            return -1;
        fread(&filename_size, sizeof(unsigned char), 1, *inp);
        fread(obj_filename, sizeof(char), filename_size, *inp);
        obj_filename[filename_size] = '\0';
    }
    else {
        fread(&filename_size, sizeof(unsigned char), 1, *inp);
        fseek(*inp, filename_size, SEEK_CUR);
    }
    if (re == 'r') {//若重命名
        strcat(s, new_filename);
        strcpy(obj_filename, s);
    }
    if ((*outp = fopen(obj_filename, "wb")) == NULL)
        return -1;
    printf("\nObject Flie:%s\n", obj_filename);
    free(obj_filename);
    return 0;
}

int Decompress(char* source_filename, char* obj_filename, char re, char* new_filename) { //解压缩文件
    int flag;
    FILE* in, * out;
    short   mini_ht[256 * 2 - 1][2];
    long    obj_filesize;
    flag = Initial_Decompress_Files(source_filename, &in, obj_filename, &out, re, new_filename); //调用重新变为原始文件的函数
    if (flag != 0) {
        puts("\nFailed to read flie!\n");
        return flag;
    }
    fread(&obj_filesize, sizeof(long), 1, in);
    Mini_HFTree(in, mini_ht);//建树
    flag = Write_Decompress_File(in, out, mini_ht, ftell(in), obj_filesize);//写原文件
    if (flag != 0) {
        puts("\nFailed to decompress！\n");
        return flag;
    }
    puts("\nDecompress Finished!\n");
    fclose(in);
    fclose(out);
    return 0;
}
