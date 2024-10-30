//
// Created by 임준식 on 8/11/24.
//
#include <stdlib.h>
#include <stdbool.h>

#include "Substitution.h"
#include "Calculate.h"
#include "AES_encryption.h"

#ifndef AES_AES_DECRYPTION_H
#define AES_AES_DECRYPTION_H

#define BLOCKSIZE 16

void invSubBytes(unsigned char *state);
void invShiftRows(unsigned char *state);
void invShiftRow(unsigned char *state, unsigned char nbr);
void invMixColumns(unsigned char *state);
void invMixColumn(unsigned char *column);

void aes_invRound(unsigned char *state, unsigned char *roundKey);
void aes_invMain(unsigned char *state, unsigned char *expandedKey, int nbrRounds);
char ECB_aes_decrypt(unsigned char *input,
                     unsigned char *output,
                     unsigned char *key,
                     unsigned char *reset_V,
                     unsigned char operation_s[10][16],
                     unsigned char ** Pre_table1,
                     unsigned char ** Pre_table2,
                     unsigned char ** Pre_table3,
                     unsigned char ** Pre_table4,
                     enum keySize size,int textsize,bool * isPadding,  MODE opMode);



void invSubBytes(unsigned char * state)
{
    int i;
    for(int i =0;i<16;i++)
    {
        state[i] = getSBoxInvert(state[i]);
    }
}

void invShiftRows(unsigned char *state)
{
    for(int i =0;i<4;i++)
    {
        invShiftRow(state + i * 4, i);
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
void aes_invRound(unsigned char * state, unsigned char *roundKey)
{
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state,roundKey);
    invMixColumns(state);
}
void aes_invMain(unsigned char *state, unsigned char * expandedKey, int nbrRounds)
{
    unsigned char roundKey[16];
    createRoundKey(expandedKey + 16 *nbrRounds, roundKey);
    addRoundKey(state, roundKey);

    for(int i = nbrRounds - 1; i > 0;i--)
    {
        createRoundKey(expandedKey + 16 * i, roundKey);
        aes_invRound(state, roundKey);
    }

    createRoundKey(expandedKey, roundKey);
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, roundKey);
}
char ECB_aes_decrypt(unsigned char *input,
                 unsigned char *output,
                 unsigned char *key,
                 unsigned char *reset_V,
                 unsigned char operation_s[10][16],
                     unsigned char ** Pre_table1,
                     unsigned char ** Pre_table2,
                     unsigned char ** Pre_table3,
                     unsigned char ** Pre_table4,
                 enum keySize size,
                         int textsize,
                         bool *isPadding,
                         MODE opMode)
{
    int expandedKeySize;
    int nbrRounds;

    unsigned char *expandedKey;
    int block_size = textsize/BLOCKSIZE;
    if(textsize%BLOCKSIZE != 0)
    {
        block_size++;
    }

    unsigned char block[block_size][BLOCKSIZE];
    int PtextSize = 1;
    if(*isPadding)
    {
        PtextSize = BLOCKSIZE - removePKCS7Padding(block[block_size-1],BLOCKSIZE);
    }
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
    expandedKeySize = ((BLOCKSIZE * nbrRounds)+BLOCKSIZE);
    expandedKey = (unsigned char *) malloc(expandedKeySize * sizeof(unsigned  char));
    if(expandedKey == NULL)
    {
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    else
    {
        for(int a = 0;a<block_size;a++)
        {
            for(int i = 0;i<4;i++)
            {
                for(int j = 0;j<4;j++)
                {
                    block[a][(i + (j * 4))] = input[(a*BLOCKSIZE)+(i * 4) + j]; // 가로로 저장된 input을 세로로 바꿔서 저장한다.
                }
            }
        }

        expandKey(expandedKey, key, size, expandedKeySize);
        char save_reset[BLOCKSIZE];
        switch(opMode)
        {
            case ECB:
                for(int i = 0;i<block_size;i++)
                {
                    aes_invMain(block[i], expandedKey, nbrRounds);
                }
                break;
            case CBC:
                for(int i = block_size-1;i>0;i--)
                {
                    aes_invMain(block[i],expandedKey, nbrRounds);
                    XORBlock(block[i],block[i-1],BLOCKSIZE);
                }
                aes_invMain(block[0],expandedKey,nbrRounds);
                XORBlock(block[0],reset_V,BLOCKSIZE);
                break;
            case CTR:
                for(int i = 0;i<block_size;i++)
                {
//                    for (int k = 0; k < 4; k++) {
//                        for (int j = 0; j < 4; j++) {
//                            save_reset[(k + (j * 4))] = reset_V[(k * 4) + j];
//                        }
//                    }
                    //aes_main(save_reset, expandedKey,nbrRounds);
//                    XORBlock(block[i], operation_s[i],BLOCKSIZE);
//                    increment_counter(reset_V,BLOCKSIZE);
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
               for(int i = block_size-1;i>=0;i--)
               {
                   XORBlock(block[i],save_reset,BLOCKSIZE);
                   aes_invMain(save_reset,expandedKey,nbrRounds);
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
                for(int i = block_size-1;i>0;i--)
                {
                    XORBlock(block[i],block[i-1],BLOCKSIZE);
                    aes_invMain(block[i-1],expandedKey,nbrRounds);
                }
                XORBlock(block[0],save_reset,BLOCKSIZE);
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
        Printing("복호문을 출력합니다",output,textsize);
        free(expandedKey);
        expandedKey = NULL;
    }
    return SUCCESS;
}

#endif AES_AES_DECRYPTION_H