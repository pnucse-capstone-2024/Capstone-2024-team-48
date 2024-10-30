#include <stdio.h>
#include "FileIO.h"
#include "AES_decryption.h"
#include "AES_encryption.h"
#include "Calculate.h"
#include "DataTypes.h"

char FolderPath[] = "/Users/imjunsik/Desktop/example";
char cryp_FileName[] = "/Users/imjunsik/Desktop/example/cryptogram.txt";
char plain_FileName[] = "/Users/imjunsik/Desktop/example/plaintext.txt";
char key_FileName[] = "/Users/imjunsik/Desktop/example/key.txt";
char resetvector_FileName[] = "/Users/imjunsik/Desktop/example/resetvector.txt";

int Text_Size(char * text)
{
    int size = 0;
    while(text[size] != '\0')
    {
        size++;
    }
    return size;
}
int main(void)
{
    //초기에 생성할 파일위치 지정
    Create_Folder(FolderPath);
    //암호문 키사이즈
    enum keySize keysize;
    int RoundNum = 0;
    int input = 0;
    MODE opMode;

    unsigned char key[BUFFER_SIZE];
    //{'k', 'k', 'k', 'k', 'e', 'e', 'e', 'e', 'y', 'y', 'y', 'y', '.', '.', '.', '.'};
    int key_size = Reading_File(key_FileName, key);
    unsigned char plaintext[BUFFER_SIZE];
    unsigned char Pre_table1[4][256];
    unsigned char Pre_table2[4][256];
    unsigned char Pre_table3[4][256];
    unsigned char Pre_table4[4][256];
    //{'a', 'b', 'c', 'd', 'e', 'f', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    int plaintext_size = Reading_File(plain_FileName, plaintext);
    int resetV_size = 0;
    switch (key_size) {
        case 16:
            keysize = SIZE_16;
            RoundNum = 10;
            break;
        case 24:
            keysize = SIZE_24;
            RoundNum = 12;
            break;
        case 32:
            keysize = SIZE_32;
            RoundNum = 14;
            break;
    }
    int expandedKeySize = (keysize * RoundNum) + keysize;
    unsigned char expandedKey[expandedKeySize];
    unsigned char ciphertext[BUFFER_SIZE];
    unsigned char decryptedtext[BUFFER_SIZE];
    unsigned char reset_v[BLOCKSIZE];
    unsigned char reset_v2[BLOCKSIZE];
    unsigned char operation_s[10][16];
    int Mode_Select;
    bool isPadding = false;
    printf("적용할 모드를 선택해주세요(1 = ECB, 2 = CBC, 3 = CTR, 4 = OFB, 5 = CFB, 6 = GCM)");
    scanf("%d", &Mode_Select);
    opMode = (MODE)Mode_Select;
    Printing("가장 처음의 키값을 출력합니다.",key,key_size);
    if(opMode == CTR)
    {
        generate_V_CTR(reset_v, BLOCKSIZE);
        for(int i = 0;i<BLOCKSIZE;i++)
        {
            reset_v2[i] = reset_v[i];
        }
    }
    else
    {
        generate_V(reset_v,BLOCKSIZE);
        for(int i = 0;i<16;i++)
        {
            reset_v[i] = 0x00;
        }
        resetV_size = Reading_File(resetvector_FileName, reset_v);
    }
    expandKey(expandedKey, key, keysize, expandedKeySize);
    //확장된 키값 출력
    Printing("확장된 키값을 출력합니다.",expandedKey,expandedKeySize);
    //평문 출력
    Printing("평문을 출력합니다",plaintext,plaintext_size);
    //암호화
    ECB_aes_encrypt(plaintext, ciphertext, key, reset_v, operation_s,Pre_table1,Pre_table2,Pre_table3,Pre_table4, keysize,plaintext_size, &isPadding, opMode);
    //복호화
    if(opMode == CTR)
    {
        ECB_aes_decrypt(ciphertext, decryptedtext, key, reset_v2,operation_s,Pre_table1,Pre_table2,Pre_table3,Pre_table4,keysize,plaintext_size,&isPadding,opMode);
    }
    else
    {
        ECB_aes_decrypt(ciphertext, decryptedtext, key, reset_v,operation_s,Pre_table1,Pre_table2,Pre_table3,Pre_table4,keysize,plaintext_size,&isPadding,opMode);
    }

    Create_Write_File(cryp_FileName,ciphertext,"w+",plaintext_size);
}
//들어있는 값을 왼쪽으로 이동시키는 함수



