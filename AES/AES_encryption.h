//
// Created by 임준식 on 8/11/24.
//
#include <stdlib.h>
#include <stdbool.h>
#include "Substitution.h"
#include "Calculate.h"
#ifndef AES_AES_ENCRYPTION_H
#define AES_AES_ENCRYPTION_H

#define BLOCKSIZE 16

void subBytes(unsigned char *state);
void shiftRows(unsigned char *state);
void shiftRow(unsigned char *state, unsigned char nbr);
void mixColumns(unsigned char *state);
void mixColumn(unsigned char *column);

void aes_main(unsigned char * state, unsigned char * expandedKey,int nbrRounds);
void aes_round(unsigned char *state, unsigned char *roundKey);
char ECB_aes_encrypt(unsigned char *input,
                     unsigned char *output,
                     unsigned char *key,
                     unsigned char *reset_V,
                     unsigned char  operation_s[10][16],
                     unsigned char ** Pre_table1,
                     unsigned char ** Pre_table2,
                     unsigned char ** Pre_table3,
                     unsigned char ** Pre_table4,
                     enum keySize size,int textsize, bool * isPadding, MODE opMode);

void subBytes(unsigned char *state)
{
    for(int i = 0;i<16;i++)
    {
        state[i] = getSBoxValue(state[i]);
    }
}
void shiftRows(unsigned char *state)
{
    for(int i = 0;i<4;i++)
    {
        shiftRow(state + i * 4,i);
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
void aes_round(unsigned char *state, unsigned char *roundKey)
{
    subBytes(state);
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey);
}

//state : block에 저장된 값
//expandedKey : 확장된 키값
//nbrRounds : 수행하는 nbrRounds 128비트의 키 길이를 가지기 때문에
void aes_main(unsigned char * state, unsigned char * expandedKey,int nbrRounds)
{
    unsigned char roundKey[16];
    createRoundKey(expandedKey , roundKey);
    addRoundKey(state, roundKey);
    //라운드키를 이용해서 평문을 바꿔준다.
    for(int i = 1; i<nbrRounds;i++)
    {
        createRoundKey(expandedKey + 16 * i, roundKey);
        aes_round(state, roundKey);
    }
    //마지막 한번의 round를 남겨놓고 값을 만들어준 이후에 마지막으로 남은 라운드를 수행해준다.
    //마지막은 mixcolumns를 수행하지 않는다.
    createRoundKey(expandedKey + 16 * nbrRounds, roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}
void CTR_aes_main(unsigned char * state,
                  unsigned char * expandedKey,
                  unsigned char Pre_table1[4][256],
                  unsigned char Pre_table2[4][256],
                  unsigned char Pre_table3[4][256],
                  unsigned char Pre_table4[4][256],
                  int nbrRounds)
{
    unsigned char roundKey[16];
    unsigned char state_save[16];
    memcpy(state_save,state,BLOCKSIZE);
    createRoundKey(expandedKey + 16 * 2, roundKey);
    for(int i = 0;i<4;i++)
    {
        state_save[(i*4)] = Pre_table1[i][state[0]];
        state_save[1+(i*4)] = Pre_table2[i][state[1]];
        state_save[2+(i*4)] = Pre_table3[i][state[2]];
        state_save[3+(i*4)] = Pre_table4[i][state[3]];
    }
    shiftRows(state);
    mixColumns(state);
    addRoundKey(state, roundKey);
    for(int i = 3;i<nbrRounds;i++)
    {
        createRoundKey(expandedKey + 16 * i, roundKey);
        aes_round(state, roundKey);
    }
    createRoundKey(expandedKey + 16 * nbrRounds, roundKey);
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKey);
}
//Pre_table을 사전에 생성한다.
void CTR_table_create(unsigned char * state, unsigned char * expandedKey,
                      unsigned char Pre_table1[4][256],
                      unsigned char Pre_table2[4][256],
                      unsigned char Pre_table3[4][256],
                      unsigned char Pre_table4[4][256],
                      int blocknumber)
{
    unsigned char roundKey[16];
    unsigned char save_reset2[BLOCKSIZE];
    for(int i = 0;i<blocknumber;i++)
    {
        memcpy(save_reset2,state,16);
        createRoundKey(expandedKey , roundKey);
        addRoundKey(save_reset2, roundKey);
        createRoundKey(expandedKey , roundKey);
        aes_round(save_reset2, roundKey);
        subBytes(save_reset2);
            for(int k = 0;k<4;k++)
            {
                Pre_table1[k][state[0]] = state[(k*4)];
                Pre_table2[k][state[1]] = state[1+(k*4)];
                Pre_table3[k][state[2]] = state[2+(k*4)];
                Pre_table4[k][state[3]] = state[3+(k*4)];
            }
        increment_counter(state,BLOCKSIZE);
    }
}
//ECB모드
char ECB_aes_encrypt(unsigned char *input,
                 unsigned char *output,
                 unsigned char *key,
                 unsigned char *reset_V,
                 unsigned char  operation_s[10][16],
                 unsigned char ** Pre_table1,
                 unsigned char ** Pre_table2,
                 unsigned char ** Pre_table3,
                 unsigned char ** Pre_table4,
                 enum keySize size,
                         int textsize,
                         bool * isPadding,
                         MODE opMode) {
    int expandedKeySize;
    //라운드를 실행할 횟수를 정의
    int nbrRounds;

    unsigned char *expandedKey;
    bool isDivBlock = true;

    //AES알고리즘은 고정된 길이의 블록 단위로 나눠서 처리하는데 AES알고리즘은 128비트로 나눠서 하게된다. 만약 부족하다면 Padding 처리한다.
    //Padding(패딩) : 암호화 하고자하는 데이터의 길이가 특정 block size로 떨어지지 않을때 block size에 맞게 데이터를 채운다.
    //128비트의 블록을 이용해서 인코딩을 해준다.
    //AES는 블록 크기가 128비트로 정해져 있다. 따라서 16바이트를 적용해 128비트를 해결한다.
    int block_size = textsize/BLOCKSIZE;
    if(textsize%16 != 0)
    {
        block_size++;
    }

    unsigned char block[block_size][BLOCKSIZE];
    unsigned char save_reset[BLOCKSIZE];
    int PtextSize;
    if(textsize%16 != 0&&opMode != CFB)
    {
        addPKCS7Padding(block[block_size-1], BLOCKSIZE, textsize%16);
        PtextSize =  removePKCS7Padding(block[block_size-1],BLOCKSIZE);
        *isPadding = true;
    }
    //AES는 원본키의 사이즈에 따라서 수행하는 라운드의 수가 달라진다.

    switch (size) {
        case SIZE_16:
            nbrRounds = 10;
            break;
        case SIZE_24:
            nbrRounds = 12;
            break;
        case SIZE_32:
            nbrRounds = 14;
            break;
        default:
            return ERROR_AES_UNKNOWN_KEYSIZE;
            break;
    }
    expandedKeySize = ((size * nbrRounds)+size); //가장 초반의 키를 추가하기 위해서 1 더해
    expandedKey = (unsigned char *) malloc(expandedKeySize * sizeof(unsigned char));
    if (expandedKey == NULL) {
        return ERROR_MEMORY_ALLOCATION_FAILED;
    } else {
        for(int a = 0;a<block_size;a++)
        {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {

                        block[a][(i + (j * 4))] = input[(a*BLOCKSIZE)+(i * 4) + j]; // 가로로 저장된 input을 세로로 바꿔서 저장한다.
                }
            }
        }
            memcpy(save_reset, reset_V,16);
            CTR_table_create(save_reset,expandedKey,
                             Pre_table1,
                             Pre_table2,
                             Pre_table3,
                             Pre_table4,block_size);

        expandKey(expandedKey, key, size, expandedKeySize);

        switch(opMode)
        {
            case ECB:
                for(int i = 0;i<block_size;i++)
                {
                    aes_main(block[i], expandedKey, nbrRounds);
                }
                break;
            case CBC:
                XORBlock(block[0], reset_V, BLOCKSIZE);
                aes_main(block[0],expandedKey,nbrRounds);
                for(int i = 1;i<block_size;i++)
                {
                    XORBlock(block[i],block[i-1],BLOCKSIZE);
                    aes_main(block[i],expandedKey, nbrRounds);
                }
                break;
            case CTR:
//                for(int i = 0;i<block_size;i++)
//                {
//                    for (int k = 0; k < 4; k++) {
//                        for (int j = 0; j < 4; j++) {
//                            save_reset[(k + (j * 4))] = reset_V[(k * 4) + j];
//                        }
//                    }
//                    aes_main(save_reset, expandedKey,nbrRounds);
//                    for(int a = 0;a<16;a++)
//                    {
//                        operation_s[i][a] = save_reset[a];
//                    }
//                    XORBlock(block[i], save_reset,BLOCKSIZE);
//                    increment_counter(reset_V,BLOCKSIZE);
//                }
                for(int i = 0 ;i<block_size;i++)
                {
                    for (int k = 0; k < 4; k++) {
                        for (int j = 0; j < 4; j++) {
                            save_reset[(k + (j * 4))] = reset_V[(k * 4) + j];
                        }
                    }
                    CTR_aes_main(save_reset,expandedKey,
                                 Pre_table1,
                                 Pre_table2,
                                 Pre_table3,
                                 Pre_table4,
                                 nbrRounds);
                    XORBlock(block[i], save_reset,BLOCKSIZE);
                    increment_counter(reset_V,BLOCKSIZE);
                }
                break;
            case OFB:
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        save_reset[(i + (j * 4))] = reset_V[(i * 4) + j];
                    }
                }
                for(int i = 0;i<block_size;i++)
                {
                    aes_main(save_reset,expandedKey,nbrRounds);
                    XORBlock(block[i],save_reset,BLOCKSIZE);
                }
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        reset_V[(i * 4) + j] = save_reset[(i + (j * 4))];
                    }
                }
                break;
            case CFB:
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        save_reset[(i + (j * 4))] = reset_V[(i * 4) + j];
                    }
                }
                aes_main(save_reset, expandedKey, nbrRounds);
                XORBlock(block[0], save_reset, BLOCKSIZE);
                for(int i = 1;i<block_size;i++)
                {
                    aes_main(block[i-1], expandedKey,nbrRounds);
                    XORBlock(block[i],block[i-1],BLOCKSIZE);
                }
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        reset_V[(i * 4) + j] = save_reset[(i + (j * 4))];
                    }
                }
                break;
            case GCM:
                break;
        }


        for(int a = 0;a<block_size;a++)
        {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                        output[(a*BLOCKSIZE)+(i * 4) + j] = block[a][(i + (j * 4))];
                }
            }
        }
        if(opMode == CFB)
        {
            Printing("암호문을 출력합니다",output,textsize);
        }
        else
        {
            Printing("암호문을 출력합니다",output,block_size*BLOCKSIZE);
        }


        free(expandedKey);
        expandedKey = NULL;
    }
    return SUCCESS;
}
#endif AES_AES_ENCRYPTION_H