//
// Created by 임준식 on 10/16/24.
//
#include <stdlib.h>
#include "Conversion.h"

#define BLOCKSIZE 16
#define EXPANDEDKEYSIZE 176
#define ROUNDNUM 10
#ifndef AES_ALGORITHM_SET_COMMON_CALCULATE_H
#define AES_ALGORITHM_SET_COMMON_CALCULATE_H
void createRoundKey(unsigned char * expandedKey, unsigned char * roundKey);
void addRoundKey(unsigned char * state, unsigned char * roundKey);
void rotate(unsigned char * word);
void core(unsigned char * word, int iteration);
unsigned char galois_multiplication(unsigned char a, unsigned char b);
void expandKey(unsigned char *expandedKey, unsigned char *key);
void subBytes(unsigned char * state);
void shiftRow(unsigned char * state, unsigned char nbr);
void shiftRows(unsigned char * state);
void mixColumn(unsigned char * column);
void mixColumns(unsigned char *state);
int increment_counter_(unsigned char *counter, size_t length)
{

    for (int i = 15; i > 0; i--) {
        if (++counter[i])  // 현재 바이트를 증가시키고 오버플로우 검사
            return i;  // 오버플로우 없으면 반복 중지
    }
}
void increment_counter(unsigned char *counter, size_t length) {
    // 카운터를 증가시키는 로직, LSB에서 시작하여 MSB로 올라감
    for (int i = 15; i > 0; i--) {
        if (++counter[i])  // 현재 바이트를 증가시키고 오버플로우 검사
            break;  // 오버플로우 없으면 반복 중지
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
void XORBlock(unsigned char *block1, unsigned char *block2, int blockSize)
{
    for(int i =0;i<blockSize;i++)
    {
        block1[i] = block1[i] ^ block2[i];
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
unsigned char galois_multiplication(unsigned char a, unsigned char b)
{
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for(; b<<1; )
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
            expandedKey[currentSize] = expandedKey[(currentSize - BLOCKSIZE)] ^ t[i];
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
void Compare_IV(unsigned char * comp1, unsigned char * comp2, int * save)
{
    for(int i = 15;i>11;i--)
    {
        if(comp1[i] != comp2[i])
        {
            save[15-i] = 0;
        }
        else
        {
            save[15-i] = 1;
        }
    }
}
void addRoundKey_4byte(unsigned char * state, unsigned char * roundKey,unsigned char * seq)
{
    for(int i = 0;i<4;i++)
    {
        state[i] = state[i] ^ roundKey[seq[i]];
    }
}
void subBytes_4Byte(unsigned char * state)
{
    for(int i = 0;i<4;i++)
    {
        state[i] = getSBoxValue(state[i]);
    }
}
void Print_ex2(unsigned char * state)
{
    for(int i =0;i<16;i++)
    {
        printf("%2.2x%c", (unsigned char)state[i], ((i + 1) % 4) ? ' ' : '\n');

    }
}
void Byte4_Round(unsigned char * state, unsigned char * expandedKey, unsigned char  * roundKey, unsigned char * seq)
{
    createRoundKey(expandedKey, roundKey);
    addRoundKey_4byte(state,roundKey, seq);
    subBytes_4Byte(state);
    mixColumn(state);
    createRoundKey(expandedKey + 16, roundKey);
    addRoundKey_4byte(state, roundKey, seq+4);
    subBytes_4Byte(state);
}
#endif //AES_ALGORITHM_SET_COMMON_CALCULATE_H
