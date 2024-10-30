

#ifndef AES_ALGORITHM_SET_AES128_H
#define AES_ALGORITHM_SET_AES128_H
#include <stdio.h>
#include "Conversion.h"
#include "Common_Calculate.h"
#include <stdlib.h>
int num1 = 1;

char aesMain(unsigned char * state,
              unsigned char * expandedKey)
{
    unsigned char roundKey[BLOCKSIZE];
    createRoundKey(expandedKey, roundKey);
    addRoundKey(state, roundKey);
    for(int i = 1; i<ROUNDNUM; i++)
    {
        createRoundKey(expandedKey + BLOCKSIZE * i , roundKey);
        aes_round(state, roundKey);
    }
    createRoundKey(expandedKey + BLOCKSIZE * ROUNDNUM, roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}
void aesEncrypt(unsigned char * input,
                 unsigned char * output,
                 unsigned char * IV,
                 unsigned char * key)
{

    int plainTextLength = strlen(input);
    int block_number = (plainTextLength%16 == 0?plainTextLength/BLOCKSIZE : plainTextLength/BLOCKSIZE + 1);
    unsigned char block[10][BLOCKSIZE];
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char SaveIV[BLOCKSIZE];
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
    unsigned char roundkey[16];
    for(int i = 0;i<block_number;i++)
    {
        for(int i = 0;i<4;i++)
        {
            for(int j = 0;j<4;j++)
            {
                SaveIV[i + (j * 4)] = IV[(i * 4) + j];
            }
        }
        aesMain(SaveIV, expandedKey);
        XORBlock(block[i], SaveIV, BLOCKSIZE);
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


#endif //AES_ALGORITHM_SET_AES128_H
