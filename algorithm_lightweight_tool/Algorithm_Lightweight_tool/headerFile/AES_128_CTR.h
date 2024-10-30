//
// Created by 임준식 on 9/23/24.
//

#ifndef ALGORITHM_LIGHTWEIGHT_TOOL_AES_128_CTR_H
#define ALGORITHM_LIGHTWEIGHT_TOOL_AES_128_CTR_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Conversion.h"



#define KEYSIZE 16
#define BLOCKSIZE 16
#define EXPANDEDKEYSIZE 176
#define ROUNDNUM 10
void incrementCounter(uint8_t *counter) {
    for (int i = 16 - 1; i >= 0; i--) {
        if (++counter[i] != 0) {
            break;  // 오버플로가 발생하지 않으면 증가 종료
        }
    }
}

void createRoundKey(unsigned char *expandedKey, unsigned char *roundKey)
{
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            roundKey[(i + (j*4))] = expandedKey[(i * 4) + j];
        }
    }
}
void aes_encrypt(unsigned char * plainText,
                 unsigned char * cryptedText,
                 unsigned char * key,
                 unsigned char * counterVec);
void convertHexStringToByteArray(const char *hexString, unsigned char *byteArray) {

    for (int i = 0; i < BLOCKSIZE * 2; i += 2) {
        unsigned int byte;
        sscanf(hexString + i, "%2x", &byte);
        byteArray[i / 2] = (unsigned char)byte;
    }
}
void increment_counter(unsigned char *counter, size_t length) {
    // 카운터를 증가시키는 로직, LSB에서 시작하여 MSB로 올라감
    for (int i = 15; i > 0; i--) {
        if (++counter[i])  // 현재 바이트를 증가시키고 오버플로우 검사
            break;  // 오버플로우 없으면 반복 중지
    }
}
void addRoundKey(unsigned char *state, unsigned char *roundKey)
{
    for(int i = 0;i<16;i++)
    {
        state[i] = state[i] ^ roundKey[i];
    }
}
void rotate(unsigned char *word)
{//배열을 왼쪽으로 옮겨준다.
    unsigned char c;
    c = word[0];
    //값을 왼쪽으로 다 이동
    for(int i = 0 ;i<3;i++)
    {
        word[i] = word[i+1];
    }
    //처음 자리에 있던걸 맨뒤쪽으로 보내준다.
    word[3] = c;
}
void core(unsigned char *word, int iteration)
{
    //32비트를 왼쪽으로 8비트씩 이동시킨다.
    rotate(word);
    for(int i = 0;i < 4;i++)
    {
        word[i] = getSBoxValue(word[i]);
    }
    word[0] = word[0] ^ getRconValue(iteration);
    //여기서 첫번째 바이트만 RconValue에 대한 XOR 연산을 수행해서 다음에 생기는 라운드키에도 영향이 생기게 한다.
}
void XORBlock(unsigned char *block1, unsigned char *block2, int blockSize)
{
    for(int i =0;i<blockSize;i++)
    {
        block1[i] = block1[i] ^ block2[i];
    }
}
unsigned char galois_multiplication(unsigned char a, unsigned char b)
{
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for(counter = 0; counter <8; counter++)
    {
        if((b&1) == 1)
        {
            p^=a;
        }
        hi_bit_set = (a&0X80);
        a<<=1;
        if(hi_bit_set == 0X80)
        {
            a^= 0X1b;
        }
        b>>=1;
    }
    return p;
}
void expandKey(unsigned char *expandedKey,
               unsigned char *key)
{
    // expandedKey = 확장된 키를 저장할 배열
    // key = 초기 암호화 키
    // size = 초기 키의 크기
    //expandedKeySize = 확장된 키의 전체 크기
    int currentSize = 0;
    int rconIteration = 1;
    unsigned  char t[4] = {0};

    //가장 처음의 키값을 확장된 키에 복사해준다.
    for(int i = 0;i<BLOCKSIZE;i++)
    {
        expandedKey[i] = key[i];
    }
    currentSize += BLOCKSIZE;

    //확장된 전체 크기의 키에 도달할때까지 키를 넣어준다.
    while(currentSize < EXPANDEDKEYSIZE)
    {
        //마지막 4바이트를 임시 배열 t에 복사한다
        for(int i = 0 ;i<4;i++)
        {
            t[i] = expandedKey[(currentSize - 4) + i];
        }

        if(currentSize % BLOCKSIZE == 0)
        {
            core(t, rconIteration++);
        }
        for(int i = 0 ;i<4;i++)
        {
            expandedKey[currentSize] = expandedKey[(currentSize - BLOCKSIZE)]^ t[i];
            currentSize++;
        }
    }
}
void subBytes(unsigned char *state)
{
    for(int i = 0;i<16;i++)
    {
        state[i] = getSBoxValue(state[i]);
    }
}
void shiftRow(unsigned char *state, unsigned char nbr)
{
    unsigned char tmp;
    for(int i = 0 ;i<nbr;i++)
    {
        tmp = state[0];
        for(int j = 0;j < 3;j++)
        {
            state[j] = state[j+1];
        }
        state[3] = tmp;
    }
}
void shiftRows(unsigned char *state)
{
    for(int i = 0;i<4;i++)
    {
        shiftRow(state + i * 4,i);
    }
}
void mixColumn(unsigned char *column)
{
    unsigned char cpy[4];
    int i;
    for (i = 0; i < 4; i++)
    {
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0], 2) ^
                galois_multiplication(cpy[3], 1) ^
                galois_multiplication(cpy[2], 1) ^
                galois_multiplication(cpy[1], 3);

    column[1] = galois_multiplication(cpy[1], 2) ^
                galois_multiplication(cpy[0], 1) ^
                galois_multiplication(cpy[3], 1) ^
                galois_multiplication(cpy[2], 3);

    column[2] = galois_multiplication(cpy[2], 2) ^
                galois_multiplication(cpy[1], 1) ^
                galois_multiplication(cpy[0], 1) ^
                galois_multiplication(cpy[3], 3);

    column[3] = galois_multiplication(cpy[3], 2) ^
                galois_multiplication(cpy[2], 1) ^
                galois_multiplication(cpy[1], 1) ^
                galois_multiplication(cpy[0], 3);
}
void mixColumns(unsigned char *state)
{
    unsigned char column[4];
    for(int i =0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            column[j] = state[(j*4)+i];
        }
        mixColumn(column);
        for(int j = 0;j<4;j++)
        {
            state[(j*4 + i)] = column[j];
        }

    }
}

void aes_round(unsigned char *state, unsigned char *roundKey)
{
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey);
}
char aes_main_ctr(unsigned char * state,
              unsigned char * expandedKey,
              unsigned char * RoundSave,
              int SaveIndex)
{
    unsigned char roundKey[BLOCKSIZE];
    createRoundKey(expandedKey , roundKey);
    addRoundKey(state, roundKey);
    for(int i = 1;i<ROUNDNUM;i++)
    {
        createRoundKey(expandedKey + BLOCKSIZE * i , roundKey);
        aes_round(state, roundKey);
        for(int q = 0;q<4;q++)
        {
            for(int k = 0;k<4;k++)
            {
                RoundSave[(SaveIndex*160)+((i-1) * 16) + (q * 4) + k] = state[(q * 4) + k];
            }
        }
    }
    createRoundKey(expandedKey + BLOCKSIZE * ROUNDNUM, roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state,roundKey);
    for(int q = 0;q<4;q++)
    {
        for(int k = 0;k<4;k++)
        {
            RoundSave[(SaveIndex*160)+((10-1) * 16) + (k * 4) + q] = state[(q * 4) + k];
        }
    }

}
char aes_main(unsigned char * state,
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
void aes_encrypt_CTR(unsigned char * input,
                     unsigned char * output,
                     unsigned char * key,
                     unsigned char * counter,
                     unsigned char * RoundSave)
{
    int SaveIndex = 0;
    int plainTextLength = BLOCKSIZE;
    int block_number = (plainTextLength%16 == 0?strlen(input)/BLOCKSIZE :   strlen(input)/BLOCKSIZE + 1);
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
                save_reset[(k + (j *4))] = counter[(k * 4) + j];
            }
        }
        aes_main_ctr(save_reset, expandedKey,RoundSave,SaveIndex);
        XORBlock(block[i], save_reset, BLOCKSIZE);
        increment_counter(counter,16);
        SaveIndex++;
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
void aes_encrypt_CTR_Testing(unsigned char * input,
                             unsigned char * output,
                             unsigned char * key)
{
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char block[BLOCKSIZE];
    unsigned char save_reset[BLOCKSIZE];
    expandKey(expandedKey, key);
    for(int j = 0;j<4;j++)
    {
        for(int a = 0;a<4;a++)
        {
            block[(j + (a * 4))] = input[(j * 4) + a];
        }
    }
    aes_main(block, expandedKey);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            output[(i * 4) + j] = block[(i + (j * 4))];
        }
    }
}
void invSubBytes(unsigned char * state)
{
    int i;
    for(int i =0;i<16;i++)
    {
        state[i] = getSBoxInvert(state[i]);
    }
}

void invShiftRow(unsigned char *state, unsigned char nbr)
{
    unsigned char tmp;
    for(int i = 0; i<nbr;i++)
    {
        tmp = state[3];
        for(int j = 3; j > 0;j--)
        {
            state[j] = state[j - 1];
        }
        state[0] = tmp;
    }
}
void invShiftRows(unsigned char *state)
{
    for(int i =0;i<4;i++)
    {
        invShiftRow(state + i * 4, i);
    }
}

void invMixColumn(unsigned char * column)
{
    unsigned char cpy[4];
    for(int i =0;i<4;i++)
    {
        cpy[i] = column[i];
    }
    column[0] = galois_multiplication(cpy[0], 14) ^
                galois_multiplication(cpy[3], 9) ^
                galois_multiplication(cpy[2], 13) ^
                galois_multiplication(cpy[1], 11);
    column[1] = galois_multiplication(cpy[1], 14) ^
                galois_multiplication(cpy[0], 9) ^
                galois_multiplication(cpy[3], 13) ^
                galois_multiplication(cpy[2], 11);
    column[2] = galois_multiplication(cpy[2], 14) ^
                galois_multiplication(cpy[1], 9) ^
                galois_multiplication(cpy[0], 13) ^
                galois_multiplication(cpy[3], 11);
    column[3] = galois_multiplication(cpy[3], 14) ^
                galois_multiplication(cpy[2], 9) ^
                galois_multiplication(cpy[1], 13) ^
                galois_multiplication(cpy[0], 11);
}
void invMixColumns(unsigned char *state)
{
    unsigned char column[4];

    for(int i =0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            column[j] = state[(j*4) + i];
        }
        invMixColumn(column);
        for(int j= 0;j<4;j++)
        {
            state[(j*4) + i] = column[j];
        }
    }
}

void  aes_invRound(unsigned char * state, unsigned char *roundKey)
{
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state,roundKey);
    invMixColumns(state);
}
void aes_invMain(unsigned char *state, unsigned char * expandedKey)
{
    unsigned char roundKey[16];
    createRoundKey(expandedKey + 16 *ROUNDNUM, roundKey);
    addRoundKey(state, roundKey);
    for(int i = ROUNDNUM - 1;i>0;i--)
    {
        createRoundKey(expandedKey + 16 *i, roundKey);
        aes_invRound(state, roundKey);
    }
    createRoundKey(expandedKey, roundKey);
    invSubBytes(state);
    invShiftRows(state);
    addRoundKey(state,roundKey);
}

char aes_decrypt(unsigned char * input,
                 unsigned char * output,
                 unsigned char * key,
                 unsigned char * counter,
                 unsigned char * RoundSave
                 ) {
    int inputLength = BLOCKSIZE;
    int block_number = (inputLength % 16 == 0 ? inputLength / BLOCKSIZE : inputLength / BLOCKSIZE + 1);
    unsigned char block[10][BLOCKSIZE];
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char save_reset[BLOCKSIZE];
    for (int i = 0; i < block_number; i++) {
        for (int j = 0; j < 4; j++) {
            for (int a = 0; a < 4; a++) {
                block[i][(j + (a * 4))] = input[(i * BLOCKSIZE) + (j * 4) + a];
            }
        }
    }
    expandKey(expandedKey, key);
    for (int i = 0; i < block_number; i++) {
        for (int k = 0; k < 4; k++) {
            for (int j = 0; j < 4; j++) {
                save_reset[(k + (j * 4))] = counter[(k * 4) + j];
            }
        }
        aes_main_ctr(   save_reset, expandedKey,RoundSave,0);
        XORBlock(block[i], save_reset, BLOCKSIZE);
    }
    for (int a = 0; a < block_number; a++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                output[(a * BLOCKSIZE) + (i * 4) + j] = block[a][(i + (j * 4))];
            }
        }
    }
}
#endif //ALGORITHM_LIGHTWEIGHT_TOOL_AES_128_CTR_H
