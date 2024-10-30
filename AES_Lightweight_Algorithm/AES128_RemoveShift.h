//
// Created by 임준식 on 10/16/24.
//

#ifndef AES_ALGORITHM_SET_AES128_REMOVESHIFT_H
#define AES_ALGORITHM_SET_AES128_REMOVESHIFT_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Conversion.h"
#include "Common_Calculate.h"




int addRoundArr[4][16] = {
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
        {0,1,2,3,7,4,5,6,10,11,8,9,13,14,15,12},
        {0,1,2,3,6,7,4,5,8,9,10,11,14,15,12,13},
        {0,1,2,3,5,6,7,4,10,11,8,9,15,12,13,14},
};

void shiftMixColumns(unsigned char *state, unsigned char * roundKey, int * roundNum)
{
    unsigned char column[4];
    int save_num[4];
    int num1 = 0;
    int round = *roundNum%4; // 라운드 번호에 따라서 방식이 정해진다.
    for(int i =0;i<4;i++)
    {
        num1 = i;
        for(int j = 0;j<4;j++)
        {
            column[j] = state[num1]; // 가장 처음 저장되는 값 0, 1, 2, 3으로 고정된다.
            save_num[j] = num1;
            if(round == 1) // 첫번째 라운드일때
            {
                num1 = num1+5 - ((3-i == j)?4 : 0 );
            }
            else if(round == 2) // 두번째 라운드일때
            {
                if(i < 2)
                {
                    num1 = num1 + ((j%2 == 0)? 6 : 2);
                }
                else
                {
                    num1 = num1 + ((j%2 == 0)? 2 : 6);
                }
            }
            else if(round == 3) // 3번째 라운드일때
            {
                num1 = num1 + 3 + ((i == j)? 4 : 0);
            }
            else // 4번째 라운드일때
            {
                num1 = num1 + 4;
            }
        }
        mixColumn(column);
        for(int j = 0;j<4;j++)
        {
            state[save_num[j]] = column[j] ^ roundKey[addRoundArr[round][save_num[j]]];
        }
    }
}

void aesRoundEncryptRemoveShift(unsigned char *state, unsigned char *roundKey, int * roundNum)
{
    subBytes(state);
    shiftMixColumns(state, roundKey, roundNum);
}
char aesMainEncryptRemoveShift(unsigned char * state,
              unsigned char * expandedKey)
{
    unsigned char roundKey[BLOCKSIZE];
    createRoundKey(expandedKey , roundKey); // 라운드키를 이용해서 확장키 생성
    addRoundKey(state, roundKey); // 평문에 라운드키 추가
    for(int i = 1;i<ROUNDNUM;i++)
    {
        createRoundKey(expandedKey + BLOCKSIZE * i , roundKey);
        aesRoundEncryptRemoveShift(state, roundKey,&i);
    }
    createRoundKey(expandedKey + BLOCKSIZE * ROUNDNUM , roundKey);
    subBytes(state);
    shiftRows(state);
    shiftRows(state);
    addRoundKey(state,roundKey);
}
void aesEncryptRemoveShift(unsigned char * input,
                 unsigned char * output,
                 unsigned char * key,
                 unsigned char * counter)
{

    int plainTextLength = strlen(input);
    int block_number = (plainTextLength%16 == 0?plainTextLength/BLOCKSIZE : plainTextLength/BLOCKSIZE + 1);
    unsigned char block[10][BLOCKSIZE];
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char save_reset[BLOCKSIZE];
    for(int i = 0;i<block_number;i++)
    {
        for(int j = 0;j<4;j++)
        {
            for(int a = 0;a<4;a++)
            {
                block[i][(j + (a * 4))] = input[(i*BLOCKSIZE) + (j * 4) + a];
            }
        }
    }
    expandKey(expandedKey,key);
    for(int i = 0;i<block_number;i++)
    {
        for(int k = 0; k < 4;k++)
        {
            for(int j = 0;j<4;j++){
                save_reset[(k + (j *4))] = counter[(k*4) + j];
            }
        }
        aesMainEncryptRemoveShift(save_reset, expandedKey);
        XORBlock(block[i], save_reset, BLOCKSIZE);
        increment_counter(counter, BLOCKSIZE);
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

#endif //AES_ALGORITHM_SET_AES128_REMOVESHIFT_H
