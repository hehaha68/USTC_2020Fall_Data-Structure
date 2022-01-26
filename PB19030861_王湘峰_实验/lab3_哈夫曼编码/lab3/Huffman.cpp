#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include<direct.h>
#include"priority_queue.h"
typedef struct Node {//Huffman�ṹ
    long    w;//wΪȨֵ
    short   p, l, r,add; //pΪparent��lΪ���ӣ�rΪ�Һ���
}HTNode, * HTnp;//��̬��������洢��������

typedef struct Huffman_Code {//Huffman�����ṹ
    unsigned char len;
    unsigned char* codestr;
}HFCode;

typedef char** HuffmanCode;//��̬��������洢�����������

int compare(const Node&a, const Node&b) {
    if (a.w > b.w) return 1;
    else if (a.w < b.w) return -1;
    else  return 0;
}
/*int Read_Floder(char name[6][100],int flag){
    char add[10000]= "C:\\Users\\wonderful\\Desktop\\lab3\\";
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
}*/


char* Create_Filename(char* source_filename, char* obj_filename, char re,char* new_filename) { //�����ļ�
    char* temp;
    char s[100000] = "C:\\Users\\wonderful\\Desktop\\lab3\\" ;
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
long Frequency(FILE* in, long frequency[]) { //Ƶ������
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
int Create_HFTree(long w[], int n, HTNode ht[]) { //������������
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
int Encode_HFTree(HTnp htp, int n, HFCode hc[]) { //�����������
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
    char s[100000] = "C:\\Users\\wonderful\\Desktop\\lab3\\";
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
void Mini_HFTree(FILE* in, short mini_ht[][2]) {
    int i;
    for (i = 0; i < 256; i++)
        mini_ht[i][0] = mini_ht[i][1] = -1;
    fread(mini_ht[i], sizeof(short), 2 * (256 - 1), in);
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

//int Select(HTnp ht, int n, int* s1, int* s2) { //ѡȡ���Ȩֵ��С����
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

int main(int argc, char** argv){
    int s;
    char m, File_Add[100000] = "C:\\Users\\wonderful\\Desktop\\lab3\\";
    s = atoi(argv[1]);
    m = argv[1][1];
    char floder_name[6][100];

    
    if(strlen(argv[1])==2) {
        switch (m) {
        case 'z':
            Compress(strcat(File_Add, argv[2]), NULL, argv[1][2], argv[3]);
            break;
        case 'u':
            Decompress(strcat(File_Add, argv[2]), NULL, argv[1][2], argv[3]);
            break;
        default:
            printf("INPUT ERROR\n");
        }
    }
    else printf("INPUT ERROR\n");
    return 0;
}
