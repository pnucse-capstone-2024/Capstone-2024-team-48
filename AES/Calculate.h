

#include <time.h>
#include "Substitution.h"
#include "DataTypes.h"
#ifndef AES_CALCULATE_H
#define AES_CALCULATE_H

void expandKey(unsigned char *expandedKey, unsigned char *key, enum keySize, size_t expandedKeySize);

void addRoundKey(unsigned char *state, unsigned char *roundKey);
unsigned char galois_multiplication(unsigned char a, unsigned char b);
void createRoundKey(unsigned char *expandedKey, unsigned char *roundKey);

void XORBlock(unsigned char *block1, unsigned char *block2, int blockSize)
{
    for(int i =0;i<blockSize;i++)
    {
        block1[i] = block1[i] ^ block2[i];
    }
}

void rotate(unsigned char *word)
{//배열을 왼쪽으로 옮겨준다.
    unsigned char c;
    int i;
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
    int i;
    //32비트를 왼쪽으로 8비트씩 이동시킨다.
    rotate(word);
    for(int i = 0;i < 4;i++)
    {
        word[i] = getSBoxValue(word[i]);
    }
    word[0] = word[0] ^ getRconValue(iteration);
    //여기서 첫번째 바이트만 RconValue에 대한 XOR 연산을 수행해서 다음에 생기는 라운드키에도 영향이 생기게 한다.
}
void expandKey(unsigned char *expandedKey,
               unsigned char *key,
               enum keySize size,
               size_t expandedKeySize)
{
    // expandedKey = 확장된 키를 저장할 배열
    // key = 초기 암호화 키
    // size = 초기 키의 크기
    //expandedKeySize = 확장된 키의 전체 크기
    int currentSize = 0;
    int rconIteration = 1;
    unsigned  char t[4] = {0};

    //가장 처음의 키값을 확장된 키에 복사해준다.
    for(int i = 0;i<size;i++)
    {
        expandedKey[i] = key[i];
    }
    currentSize += size;

    //확장된 전체 크기의 키에 도달할때까지 키를 넣어준다.
    while(currentSize < expandedKeySize)
    {
        //마지막 4바이트를 임시 배열 t에 복사한다
        for(int i = 0 ;i<4;i++)
        {
            t[i] = expandedKey[(currentSize - 4) + i];
        }

        if(currentSize % size == 0)
        {
            core(t, rconIteration++);
        }
        if(size == SIZE_32 && ((currentSize % size) == 16))
        {
            for(int i =0;i<4;i++)
            {
                t[i] = getSBoxValue(t[i]);
            }
        }
        for(int i = 0 ;i<4;i++)
        {
            expandedKey[currentSize] = expandedKey[(currentSize - size)]^ t[i];
            currentSize++;
        }
    }
}

//가로로 저장된 값을 세로로 저장해준다.
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
void addRoundKey(unsigned char *state, unsigned char *roundKey)
{
    for(int i =0;i<16;i++)
    {
        state[i] = state[i] ^roundKey[i];
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
void addPKCS7Padding(unsigned char *block, size_t blockSize, size_t dataSize){
    size_t paddingSize = blockSize - (dataSize % blockSize);
    unsigned char paddingValue = (unsigned char)paddingSize;
    for(size_t i =blockSize-paddingSize ;i < blockSize;i++)
    {
        block[i] = paddingValue;
    }
}
size_t removePKCS7Padding(unsigned char * block, size_t blockSize){
    unsigned char paddingValue = block[blockSize - 1];
    size_t paddingSize = (size_t)paddingValue;
    if(paddingSize > 0 && paddingSize <= blockSize){
        return paddingSize;
    }
    return blockSize;
}
void CounterPlus(char * Counter)
{
    int i = 15;
    while(Counter[i] != 0xFF)
    {
        i--;
    }
    Counter[i] += 0x01;
}
void Printing(char * sentence,char * Print_Text,int size)
{
    printf("\n%s\n",sentence);
    for (int i = 0; i < size; i++)
    {
        printf("%2.2x%c", (unsigned char)Print_Text[i], ((i + 1) % 16) ? ' ' : '\n');
    }
}
void generate_V(unsigned char *reset_v, size_t length) {
    // 난수 생성기 초기화
    srand((unsigned)time(NULL));

    // IV를 랜덤 바이트로 채움
    for (size_t i = 0; i < length; i++) {
        reset_v[i] = rand() % 256; // 0에서 255 사이의 난수 생성
    }
}
void generate_V_CTR(unsigned char * reset_v, size_t length)
{
    srand((unsigned)time(NULL));

    for(size_t i = 0;i<4;i++)
    {
        reset_v[i] = 0;
    }
    for(size_t i = 4;i<length;i++)
    {
        reset_v[i] = rand() % 256; // 0에서 255 사이의 난수 생성
    }
}
void increment_counter(unsigned char *counter, size_t length) {
    // 카운터를 증가시키는 로직, LSB에서 시작하여 MSB로 올라감
    for (int i = 0; i < 16; i++) {
        if (++counter[i])  // 현재 바이트를 증가시키고 오버플로우 검사
            break;  // 오버플로우 없으면 반복 중지
    }
}
void decrement_counter(unsigned char *counter, size_t length) {
    // 가장 낮은 바이트부터 시작하여 언더플로우 처리
    for (int i = length - 1; i >= 0; i--) {
        if (counter[i]-- != 0)  // 현재 바이트를 감소시키고 언더플로우 검사
            break;  // 언더플로우 없으면 반복 중지
    }
}


#endif //AES_CALCULATE_H
