//
// Created by 임준식 on 10/15/24.
//

#ifndef AES_ALGORITHM_SET_AES128_BEFOREHAND_LIGHT_H
#define AES_ALGORITHM_SET_AES128_BEFOREHAND_LIGHT_H
#include <stdio.h>
#include "Conversion.h"
#include "Common_Calculate.h"
#include <stdlib.h>

#define BLOCKSIZE 16
#define EXPANDEDKEYSIZE 176
#define ROUNDNUM 10





void createColumns(unsigned char * preColumns1, unsigned char * preColumns2,
                   unsigned char * preColumns3, unsigned char * preColumns4,
                   unsigned char * state,  int * result, unsigned char * IV,
                   unsigned char * expandedKey)
{
    unsigned char seq[32] = {
            0,5,10,15,0,4,8,12,
            1,6,11,12,1,5,9,13,
            2,7,8,13,2,6,10,14,
            3,4,9,14,3,7,11,15
    };
    unsigned char temp[4];
    unsigned char IV_temp[16];
    unsigned char * expandedKeySave = expandedKey;
    unsigned char roundKey[BLOCKSIZE];
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            IV_temp[i + (j*4)] = IV[(i*4)+j];
        }
    }
    if(!result[0])
    {
        temp[0] = IV_temp[0];
        temp[1] = IV_temp[5];
        temp[2] = IV_temp[10];
        temp[3] = IV_temp[15];
        Byte4_Round(temp, expandedKeySave, roundKey,seq);
        for(int i = 0;i<4;i++)
        {
            preColumns1[i] = temp[i];
        }
    }
    if(!result[1])
    {
        temp[0] = IV_temp[1];
        temp[1] = IV_temp[6];
        temp[2] = IV_temp[11];
        temp[3] = IV_temp[12];
        Byte4_Round(temp, expandedKeySave,roundKey,seq+8);
        for(int i = 0;i<4;i++)
        {
            preColumns2[i] = temp[i];
        }
    }
    if(!result[2])
    {
        temp[0] = IV_temp[2];
        temp[1] = IV_temp[7];
        temp[2] = IV_temp[8];
        temp[3] = IV_temp[13];
        Byte4_Round(temp, expandedKeySave,roundKey,seq+16);
        for(int i = 0;i<4;i++)
        {
            preColumns3[i] = temp[i];
        }
    }
    if(!result[3])
    {
        temp[0] = IV_temp[3];
        temp[1] = IV_temp[4];
        temp[2] = IV_temp[9];
        temp[3] = IV_temp[14];
        Byte4_Round(temp, expandedKeySave,roundKey,seq+24);
        for(int i = 0;i<4;i++)
        {
            preColumns4[i] = temp[i];
        }
    }
    state[0] = preColumns1[0];
    state[1] = preColumns2[0];
    state[2] = preColumns3[0];
    state[3] = preColumns4[0];
    state[4] = preColumns1[1];
    state[5] = preColumns2[1];
    state[6] = preColumns3[1];
    state[7] = preColumns4[1];
    state[8] = preColumns1[2];
    state[9] = preColumns2[2];
    state[10] = preColumns3[2];
    state[11] = preColumns4[2];
    state[12] = preColumns1[3];
    state[13] = preColumns2[3];
    state[14] = preColumns3[3];
    state[15] = preColumns4[3];

}

char aesMainEncryptBeforeLight(unsigned char * state,
              unsigned char * expandedKey)
{
    unsigned char roundKey[BLOCKSIZE];
    createRoundKey(expandedKey + (BLOCKSIZE*2), roundKey);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey);
    for(int i = 3; i<ROUNDNUM; i++)
    {
        createRoundKey(expandedKey + (BLOCKSIZE * i) , roundKey);
        aes_round(state, roundKey);
    }
    createRoundKey(expandedKey + (BLOCKSIZE * ROUNDNUM), roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}
void aesEncryptBeforeLight(unsigned char * input,
                 unsigned char * output,
                 unsigned char * IV,
                 unsigned char * key)
{

    int plainTextLength = strlen(input);
    int block_number = (plainTextLength%16 == 0?plainTextLength/BLOCKSIZE : plainTextLength/BLOCKSIZE + 1);
    unsigned char block[10][BLOCKSIZE];
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char SaveIV[BLOCKSIZE];
    unsigned char SaveIV_before[BLOCKSIZE];
    unsigned char roundKey[BLOCKSIZE];

    int result[4] = {0,0,0,0};//어떤 부분이 바꼈는지 보기 위해서 설정한다. 0 = 15, 1 = 14, 2 = 13, 3 = 12
    unsigned char preColumns1[4]; //1열 저장
    unsigned char preColumns2[4]; //2열 저장
    unsigned char preColumns3[4]; //3열 저장
    unsigned char preColumns4[4]; //4열 저장
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
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            SaveIV[i + (j * 4)] = IV[(i * 4) + j];
            SaveIV_before[(i * 4) + j] = IV[(i * 4) + j];
        }
    }
    createRoundKey(expandedKey, roundKey);
    addRoundKey(SaveIV, roundKey);
    subBytes(SaveIV);
    shiftRows(SaveIV);
    mixColumns(SaveIV);
    createRoundKey(expandedKey+16, roundKey);
    addRoundKey(SaveIV, roundKey);
    subBytes(SaveIV);
    preColumns1[0] = SaveIV[0];
    preColumns1[1] = SaveIV[4];
    preColumns1[2] = SaveIV[8];
    preColumns1[3] = SaveIV[12];

    preColumns2[0] = SaveIV[1];
    preColumns2[1] = SaveIV[5];
    preColumns2[2] = SaveIV[9];
    preColumns2[3] = SaveIV[13];

    preColumns3[0] = SaveIV[2];
    preColumns3[1] = SaveIV[6];
    preColumns3[2] = SaveIV[10];
    preColumns3[3] = SaveIV[14];

    preColumns4[0] = SaveIV[3];
    preColumns4[1] = SaveIV[7];
    preColumns4[2] = SaveIV[11];
    preColumns4[3] = SaveIV[15];
    Print_ex2(SaveIV);
    printf("--------\n");
    //2라운드 SubBytes까지 저장함
    aesMainEncryptBeforeLight(SaveIV,expandedKey);
    XORBlock(block[0], SaveIV,BLOCKSIZE);
    //첫번째 블록은 끝남.
    for(int i = 1;i<block_number;i++)
    {
        increment_counter(IV,BLOCKSIZE);
        Compare_IV(SaveIV_before, IV, result);
        createColumns(preColumns1,preColumns2,preColumns3,preColumns4, SaveIV, result,IV,expandedKey);
        Print_ex2(SaveIV);
        printf("--------\n");
            for(int j = 0;j<16;j++) {
                SaveIV_before[j] = IV[j];
            }
        aesMainEncryptBeforeLight(SaveIV, expandedKey);
        XORBlock(block[i], SaveIV, BLOCKSIZE);
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

#endif //AES_ALGORITHM_SET_AES128_BEFOREHAND_LIGHT_H
