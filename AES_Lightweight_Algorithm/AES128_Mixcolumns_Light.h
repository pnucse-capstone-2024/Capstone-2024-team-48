//
// Created by 임준식 on 10/16/24.
//
#include "Common_Calculate.h"

#ifndef AES_ALGORITHM_SET_AES128_MIXCOLUMNS_LIGHT_H
#define AES_ALGORITHM_SET_AES128_MIXCOLUMNS_LIGHT_H






//초기의 열에 필요한 값들을 설정한다.
unsigned char createMixColumnsArr( unsigned char mixColumnsArr1[16][3],
                                   unsigned char mixColumnsArr2[16][3],
                                   unsigned char * IV,
                                   unsigned char * expandedKey)
{
    unsigned char temp[16];
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            temp[i + (4*j)] = IV[(i*4) + j];
        }
    }
    unsigned char roundKey[BLOCKSIZE];
    createRoundKey(expandedKey, roundKey);
    addRoundKey(temp, roundKey);
    subBytes(temp);
    for(int i = 0;i<16;i++)
    {
        for(int j = 0;j<3;j++)
        {
           mixColumnsArr1[i][j] = galois_multiplication(temp[i],j+1);
        }
    }
    shiftRows(temp);
    mixColumns(temp);
    createRoundKey(expandedKey + 16, roundKey);
    addRoundKey(temp, roundKey);
    subBytes(temp);
    for(int i = 0;i<16;i++)
    {
        for(int j = 0;j<3;j++)
        {
            mixColumnsArr2[i][j] = galois_multiplication(temp[i],j+1);
        }
    }
}
unsigned char setMixColumnsArr(unsigned char mixColumnsArr1[16][3],
                               unsigned char mixColumnsArr2[16][3],
                               unsigned char * IV,
                               unsigned char * expandedKey,
                               int * result)
{
    unsigned char seq[32] = {
            0,5,10,15,0,4,8,12,
            1,6,11,12,1,5,9,13,
            2,7,8,13,2,6,10,14,
            3,4,9,14,3,7,11,15
    };
    int num[4];
    for(int i =0;i<4;i++)
    {
        num[i] = result[i];
    }
    unsigned char IV_save[16];
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            IV_save[i+(4*j)] = IV[(i*4) + j];
        }
    }
    unsigned char roundKey[16];
    unsigned char temp[4];
    createRoundKey(expandedKey, roundKey);
    if(!num[0])
    {

        temp[0] = IV_save[0];
        temp[1] = IV_save[5];
        temp[2] = IV_save[10];
        temp[3] = IV_save[15];

        Byte4_Round(temp,expandedKey,roundKey,seq);
        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[0][j] = galois_multiplication(temp[0],j+1);
        }
        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[4][j] = galois_multiplication(temp[1],j+1);
        }
        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[8][j] = galois_multiplication(temp[2],j+1);
        }
        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[12][j] = galois_multiplication(temp[3],j+1);
        }
    }
    if(!num[1])
    {

        temp[0] = IV_save[1];
        temp[1] = IV_save[6];
        temp[2] = IV_save[11];
        temp[3] = IV_save[12];
        Byte4_Round(temp,expandedKey,roundKey,seq+8);

        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[1][j] = galois_multiplication(temp[0],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[5][j] = galois_multiplication(temp[1],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[9][j] = galois_multiplication(temp[2],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[13][j] = galois_multiplication(temp[3],j+1);
        }
    }
    if(!num[2])
    {

        temp[0] = IV_save[2];
        temp[1] = IV_save[7];
        temp[2] = IV_save[8];
        temp[3] = IV_save[13];
        Byte4_Round(temp,expandedKey,roundKey,seq+16);
        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[2][j] = galois_multiplication(temp[0],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[6][j] = galois_multiplication(temp[1],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[10][j] = galois_multiplication(temp[2],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[14][j] = galois_multiplication(temp[3],j+1);
        }
    }
    if(!num[3])
    {
        temp[0] = IV_save[3];
        temp[1] = IV_save[4];
        temp[2] = IV_save[9];
        temp[3] = IV_save[14];
        Byte4_Round(temp,expandedKey,roundKey,seq+32);
        for (int j = 0; j < 3; j++) {
            mixColumnsArr2[3][j] = galois_multiplication(temp[0],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[7][j] = galois_multiplication(temp[1],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[11][j] = galois_multiplication(temp[2],j+1);
        }  for (int j = 0; j < 3; j++) {
            mixColumnsArr2[15][j] = galois_multiplication(temp[3],j+1);
        }
    }

}
void mixColumnsSave(unsigned char mixColumnsArr[16][3] , unsigned char * IV)
{
    IV[0] = mixColumnsArr[0][1] ^ mixColumnsArr[5][2] ^ mixColumnsArr[10][0] ^ mixColumnsArr[15][0];
    IV[4] = mixColumnsArr[0][0] ^ mixColumnsArr[5][1] ^ mixColumnsArr[10][2] ^ mixColumnsArr[15][0];
    IV[8] = mixColumnsArr[0][0] ^ mixColumnsArr[5][0] ^ mixColumnsArr[10][1] ^ mixColumnsArr[15][2];
    IV[12] = mixColumnsArr[0][2] ^ mixColumnsArr[5][0] ^ mixColumnsArr[10][0] ^ mixColumnsArr[15][1];

    IV[1] = mixColumnsArr[1][1] ^ mixColumnsArr[6][2] ^ mixColumnsArr[11][0] ^ mixColumnsArr[12][0];
    IV[5] = mixColumnsArr[1][0] ^ mixColumnsArr[6][1] ^ mixColumnsArr[11][2] ^ mixColumnsArr[12][0];
    IV[9] = mixColumnsArr[1][0] ^ mixColumnsArr[6][0] ^ mixColumnsArr[11][1] ^ mixColumnsArr[12][2];
    IV[13] = mixColumnsArr[1][2] ^ mixColumnsArr[6][0] ^ mixColumnsArr[11][0] ^ mixColumnsArr[12][1];

    IV[2] = mixColumnsArr[2][1] ^ mixColumnsArr[7][2] ^ mixColumnsArr[8][0] ^ mixColumnsArr[13][0];
    IV[6] = mixColumnsArr[2][0] ^ mixColumnsArr[7][1] ^ mixColumnsArr[8][2] ^ mixColumnsArr[13][0];
    IV[10] = mixColumnsArr[2][0] ^ mixColumnsArr[7][0] ^ mixColumnsArr[8][1] ^ mixColumnsArr[13][2];
    IV[14] = mixColumnsArr[2][2] ^ mixColumnsArr[7][0] ^ mixColumnsArr[8][0] ^ mixColumnsArr[13][1];

    IV[3] = mixColumnsArr[3][1] ^ mixColumnsArr[4][2] ^ mixColumnsArr[9][0] ^ mixColumnsArr[14][0];
    IV[7] = mixColumnsArr[3][0] ^ mixColumnsArr[4][1] ^ mixColumnsArr[9][2] ^ mixColumnsArr[14][0];
    IV[11] = mixColumnsArr[3][0] ^ mixColumnsArr[4][0] ^ mixColumnsArr[9][1] ^ mixColumnsArr[14][2];
    IV[15] = mixColumnsArr[3][2] ^ mixColumnsArr[4][0] ^ mixColumnsArr[9][0] ^ mixColumnsArr[14][1];
}

char aesMainMixColumnsLight(unsigned char * state,
             unsigned char * expandedKey)
{
    unsigned char roundKey[BLOCKSIZE];
    createRoundKey(expandedKey + 32, roundKey);
    addRoundKey(state, roundKey);
    for(int i = 3; i<ROUNDNUM; i++)
    {
        createRoundKey(expandedKey + BLOCKSIZE * i , roundKey);
        aes_round(state, roundKey);
    }
    createRoundKey(expandedKey + BLOCKSIZE * ROUNDNUM, roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}
void aesEncryptMixColumnsLight(unsigned char * input,
                unsigned char * output,
                unsigned char * IV,
                unsigned char * key)
{
    unsigned char mixColumnsArr1[16][3];
    unsigned char mixColumnsArr2[16][3];
    int plainTextLength = strlen(input);
    int block_number = (plainTextLength%16 == 0?plainTextLength/BLOCKSIZE : plainTextLength/BLOCKSIZE + 1);
    unsigned char block[10][BLOCKSIZE];
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char SaveIV[BLOCKSIZE];
    unsigned char SaveIV_before[BLOCKSIZE];
    int result[4];
    for(int i = 0;i<block_number;i++)
    {
        //0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 =>
        //0  | 1 | 2 | 3
        //4  | 5 | 6 | 7
        //8  | 9 | 10| 11
        //12 | 13| 14| 15
        for(int j = 0;j<4;j++)
        {
            for(int a = 0;a<4;a++)
            {
                block[i][(j + (a * 4))] = input[(i*BLOCKSIZE) + (j * 4) + a];
            }
        }
    }
    expandKey(expandedKey,key);
    createMixColumnsArr(mixColumnsArr1, mixColumnsArr2,IV, expandedKey);
    for(int j = 0;j<4;j++)
    {
        for(int k = 0;k <4;k++)
        {
            SaveIV_before[(j*4) + k] = IV[(j*4) + k];
        }
    }
    for(int i = 0;i<block_number;i++)
    {
        Compare_IV(SaveIV_before, IV, result);
        setMixColumnsArr(mixColumnsArr1,mixColumnsArr2,IV,expandedKey,result);
        mixColumnsSave(mixColumnsArr2,SaveIV);
        printf("-------\n");
        Print_ex2(SaveIV);
        aesMainMixColumnsLight(SaveIV, expandedKey);
        XORBlock(block[i], SaveIV, BLOCKSIZE);
        for(int j = 0;j<4;j++)
        {
            for(int k = 0;k <4;k++)
            {
                SaveIV_before[(j*4) + k] = IV[(j*4) + k];
            }
        }
        increment_counter(IV,BLOCKSIZE);
    }
    for(int a = 0;a<block_number;a++)
    {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                output[(a*BLOCKSIZE)+(i * 4) + j] = block[a][(i + (j * 4))];
            }
        }
    }
}

#endif //AES_ALGORITHM_SET_AES128_MIXCOLUMNS_LIGHT_H
