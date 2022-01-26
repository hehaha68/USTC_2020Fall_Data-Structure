#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "File.h"
#include<direct.h>
#include "floder.h"

int main(int argc, char** argv){
    int s;
    char m, File_Add[100000] = "C:\\Users\\wonderful\\Desktop\\Ñ§Ï°&¹¤×÷\\lab3_¹þ·òÂü±àÂë\\hyy\\";
    s = atoi(argv[1]);
    m = argv[1][1];
    char floder_name[6][100];

    if (!strrchr(argv[2], '.')) {
        switch (m) {
        case 'z': {
            printf("\nFiles in %s:\n", strcat(File_Add, argv[2]));
            Read_Floder(floder_name,1);
            for (int i = 0; i < 6; i++) {
                char Floder_Add[100000] = "C:\\Users\wonderful\\Desktop\\Ñ§Ï°&¹¤×÷\\lab3_¹þ·òÂü±àÂë\\hyy\\";
                Compress(strcat(Floder_Add, floder_name[i]), NULL, NULL, NULL);
            }
            char A[100000] = "C:\\Users\\wonderful\\Desktop\\Ñ§Ï°&¹¤×÷\\lab3_¹þ·òÂü±àÂë\\hyy\\";
            mkdir(strcat(A, "new"));
            break;
        }
        case 'u':
            break;
        }
    }
    else {
        switch (m) {
        case 'z':
            Compress(strcat(File_Add, argv[2]), NULL, argv[1][2], argv[3]);
            break;
        case 'u':
            Decompress(strcat(File_Add, argv[2]), NULL, argv[1][2], argv[3]);
            break;
        default:
            printf("INPUT ERROR;\n");
        }
    }
    return 0;
}
