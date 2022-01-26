#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "Huffman.h"
int Write_Compress_File(FILE* in, FILE* out, HTnp ht, HFCode hc[], char* source_filename, long source_filesize);

char* Create_Filename(char* source_filename, char* obj_filename, char re,char* new_filename) { //�����ļ�
    char* temp;
    char s[100000] = "D:\\USTC\\Homework\\DS\\huffman\\" ;
    if ((temp = strrchr(source_filename, '.')) == NULL) { //��ԭʼ�ļ� ���� ��ʱ�ļ��� Ϊ��
        strcpy(obj_filename, source_filename);
        strcat(obj_filename, ".huff");
    }
    else {
        strncpy(obj_filename, source_filename, temp - source_filename);
        obj_filename[temp - source_filename] = '\0';
        strcat(obj_filename, ".huff");
    }

    if (re == 'r') {
        strcat(s, new_filename);
        strcpy(obj_filename,s);
    }
    return obj_filename;
}
int Initial_Compress_Files(char* source_filename, FILE** inp, char* obj_filename, FILE** outp, char re, char* new_filename) { //��ʼ���ļ�
    if (fopen(source_filename, "rb") == NULL)   return -1;
    if (obj_filename == NULL) { //ѹ���ļ�Ϊ��
        if ((obj_filename = (char*)malloc(256 * sizeof(char))) == NULL) //����ռ�
            return -1;
        Create_Filename(source_filename, obj_filename,re,new_filename);
    }
    if (strcmp(source_filename, obj_filename) == 0)//�����ѹ���ļ���ѹ���ļ��ȽϺ���ͬ
        return -1;  
    if ((*outp = fopen(obj_filename, "wb")) == NULL)
        return -1;
    if ((*inp = fopen(source_filename, "rb")) == NULL)
        return -1;
    printf("\nSource File:%s\nObject File:%s\n",
        source_filename, obj_filename);//��ѹ���ļ�source_filename,ѹ���ļ�obj_filename
    free(obj_filename); //�ر�
    return 0;
}

int Compress(char* source_filename, char* obj_filename, char re, char* new_filename) { //ѹ���ļ�
    FILE* in, * out;
    int flag, i, j;
    HFCode hc[256];
    HTNode  ht[256 * 2 - 1];
    long frequency[256], source_filesize;

    flag = Initial_Compress_Files(source_filename, &in, obj_filename, &out,re, new_filename);//���ó�ʼ���ļ��ĺ���
    if (flag != 0) {
        puts("\nFailed to read flie!\n");
        return flag;
    }
    source_filesize = Frequency(in, frequency);   //����Ƶ�����ݸ���Դ�ļ���С
    flag = Create_HFTree(frequency, 256, ht);
    if (flag != 0) {
        puts("\nFailed to build Huffman Tree��\n");
        return flag;
    }
    flag = Encode_HFTree(ht, 256, hc); //���ù��������뺯��
    if (flag != 0) {
        puts("\nFailed to build Huffman code��\n");
        return flag;
    }

    flag = Write_Compress_File(in, out, ht, hc, source_filename, source_filesize);
    if (flag != 0) {
        puts("\nFailed to write flie!\n");
        return flag;
    }
    puts("\nCompress Finished!\n");
    fclose(in);
    fclose(out);
    for (i = 0; i < 256; i++)   free(hc[i].codestr);
    return 0;
}

unsigned char Chars_To_Bits(const unsigned char chars[8]) { //���ַ����ö��������ֱ�ʾ
    int i;
    unsigned char bits = 0;
    bits |= chars[0];
    for (i = 1; i < 8; ++i) {
        bits <<= 1;
        bits |= chars[i];
    }
    return bits;
}

int Write_Compress_File(FILE* in, FILE* out, HTnp ht, HFCode hc[], char* source_filename, long source_filesize) { //д��ѹ���ļ�
    unsigned int    i, read_counter, write_counter, huff_head = 0xFFFFFFFF;
    unsigned char   write_char_counter, code_char_counter, copy_char_counter,
        read_buf[256], write_buf[256], write_chars[8], filename_size = strlen(source_filename);
    HFCode* cur_HFCode;

    fseek(in, 0L, SEEK_SET);//�ļ�ָ�붨λ����ͷ
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
                memcpy(write_chars + write_char_counter, cur_HFCode->codestr + code_char_counter, copy_char_counter);
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
        write_char_counter = Chars_To_Bits(write_chars);//λ�����ĵ��ã�ʵ��ѹ������
        fwrite(&write_char_counter, 1, 1, out);
    }
    return 0;
}

int Write_Decompress_File(FILE* in, FILE* out, short mini_ht[][2], long bits_pos, long obj_filesize) { //д���ѹ��
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

int Initial_Decompress_Files(char* source_filename, FILE** inp, char* obj_filename, FILE** outp,char re, char* new_filename) { //���±��ԭʼ�ļ�
    unsigned int    huff_head; //ѹ���ļ���ͷ�ļ�
    unsigned char   filename_size;    //�ļ���С 
    char s[100000] = "D:\\USTC\\Homework\\DS\\huffman\\";
    if ((*inp = fopen(source_filename, "rb")) == NULL)
        return -1;
    printf("\nSource Flie:%s,", source_filename);
    fread(&huff_head, sizeof(unsigned int), 1, *inp);
    if (huff_head == NULL)
        return -1;
    if (obj_filename == NULL) {
        if ((obj_filename = (char*)malloc(256 * sizeof(char))) == NULL)//����
            return -1;
        fread(&filename_size, sizeof(unsigned char), 1, *inp);
        fread(obj_filename, sizeof(char), filename_size, *inp);
        obj_filename[filename_size] = '\0';
    }
    else {
        fread(&filename_size, sizeof(unsigned char), 1, *inp);
        fseek(*inp, filename_size, SEEK_CUR);
    }
    if (re == 'r') {
        strcat(s, new_filename);
        strcpy(obj_filename, s);
    }

    if ((*outp = fopen(obj_filename, "wb")) == NULL)
        return -1;
    printf("\nObject Flie:%s\n", obj_filename);
    
    free(obj_filename);
    return 0;
}

int Decompress(char* source_filename, char* obj_filename, char re, char* new_filename) { //��ѹ���ļ�
    int flag;
    FILE* in, * out;
    short   mini_ht[256 * 2 - 1][2];
    long    obj_filesize;
    flag = Initial_Decompress_Files(source_filename, &in, obj_filename, &out, re, new_filename); //�������±�Ϊԭʼ�ļ��ĺ���
    if (flag != 0) {
        puts("\nFailed to read flie!\n");
        return flag;
    }
    fread(&obj_filesize, sizeof(long), 1, in);
    Mini_HFTree(in, mini_ht);
    flag = Write_Decompress_File(in, out, mini_ht, ftell(in), obj_filesize);
    if (flag != 0) {
        puts("\nFailed to decompress��\n");
        return flag;
    }
    puts("\nDecompress Finished!\n");
    fclose(in);
    fclose(out);
    return 0;
}