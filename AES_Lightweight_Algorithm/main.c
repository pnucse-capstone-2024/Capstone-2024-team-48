#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "AES128.h"
#include "AES128_beforehand.h"
//#include "AES128_beforehand_light.h"
//#include "AES128_RemoveShift.h"
//#include "AES128_Mixcolumns_Light.h"

#define PLAINTEXT "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710f5d3d58503b9699de785895a96fdbaafe03a030d0f3fd6f3fe60734f6a03fdee0ba4a5f76d6cd0d5e64b225d26e119b77a85e5ed2073033d06cf6c6e4a59e5c1a5e46997257fe23365b94e40bdbdb14f4686cfbc7c7559d2f80d3a60c4c9d0ac"
#define KEY "2b7e151628aed2a6abf7158809cf4f3c"
#define I_VECTOR "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
#define CIPHERTEXT "874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff5ae4df3edbd5d35e5b4f09020db03eab1e031dda2fbe03d1792170a0f3009cee45de927d1733f89317edb9cd06b61fadb8a3475742ded706edaaa2d7178a14f5307d098e24b2fb2c83228e6565196bf5071f2b6d67b050341395d4c6badc72701278d0b16591aaa4f0167855970e47743cc4a0caf1ccbc3ae4b8f22eb3841d81"
void Printing(char * sentence,char * Print_Text,int size)
{
    printf("\n%s\n",sentence);
    for (int i = 0; i < size; i++)
    {
        printf("%2.2x%c", (unsigned char)Print_Text[i], ((i + 1) % 16) ? ' ' : '\n');
    }
}
void ChangeHexa(unsigned char * input, unsigned char * output, int size)
{

    for(int i = 0;i<size;i++)
    {
        char byte[3] = {output[2*i], output[2*i+1], '\0'};
        input[i] = (char)strtol(byte, NULL, 16);
    }
}
int CompareText(unsigned char * comp1, unsigned char * comp2,int size)
{
    for(int i = 0;i<size;i++)
    {
        if(comp1[i] != comp2[i])
        {
            return 0;
        }
    }
    return 1;
}

int main() {
    unsigned char * Pre_table1[4][256];
    unsigned char * Pre_table2[4][256];
    unsigned char * Pre_table3[4][256];
    unsigned char * Pre_table4[4][256];
    int plaintext_len = strlen(PLAINTEXT)/2;
    int key_len = strlen(KEY)/2;
    int IV_len = strlen(I_VECTOR)/2;
    unsigned char * plaintext = malloc(plaintext_len);
    unsigned char * ciphertext = malloc(plaintext_len);
    unsigned char * key = malloc(key_len);
    unsigned char * IV = malloc(IV_len);
    unsigned char * ciphertext_result = malloc(plaintext_len);
    if(plaintext == NULL)
    {
        printf("메모리 할당에 실패했습니다.\n");
        return 1;
    }
    ChangeHexa(plaintext, PLAINTEXT, plaintext_len);
    ChangeHexa(key, KEY, key_len);
    ChangeHexa(IV, I_VECTOR, IV_len);
    ChangeHexa(ciphertext_result,CIPHERTEXT,plaintext_len);

    Printing("평문을 출력합니다.",plaintext,plaintext_len);
    Printing("키를 출력합니다.", key, key_len);
    Printing("카운터를 출력합니다", IV, IV_len);

    //aesEncrypt(plaintext,ciphertext,IV,key);
    aesEncryptBefore(plaintext,IV,0,Pre_table1,Pre_table2,Pre_table3,Pre_table4, key,ciphertext);
    //aesEncryptBeforeLight(plaintext,ciphertext,IV,key);
    //aesEncryptRemoveShift(plaintext,ciphertext,key,IV);
    //aesEncryptMixColumnsLight(plaintext,ciphertext,IV, key);
    Printing("암호문을 출력합니다", ciphertext, plaintext_len);

    if(CompareText(ciphertext,ciphertext_result,plaintext_len))
    {
        printf("암호문이 맞습니다.");
    }
    else
    {
        printf("암호문이 다릅니다.");
    }
    return 0;
}
