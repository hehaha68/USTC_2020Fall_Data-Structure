#include<stdio.h>
int main(){
	char a[9]={"11010110"};
	//unsigned char Chars_To_Bits(const unsigned char chars[8]) { //把字符串用二进制数字表示
    int i;
    unsigned char bits = 0;
    bits |= a[0];
    for (i = 1; i < 8; ++i) {
        bits <<= 1;
        bits |= a[i];
    }
    //return bits;
    printf("%d",bits);
}

