//
// Created by 임준식 on 10/9/24.
//

#ifndef AES_ALGORITHM_SET_AES128_BEFOREHAND_H
#define AES_ALGORITHM_SET_AES128_BEFOREHAND_H


#ifndef AES_ALGORITHM_SET_AES128_H
#define AES_ALGORITHM_SET_AES128_H
#include <stdio.h>
#include "Conversion.h"
#include "Common_Calculate.h"
#include <stdlib.h>

#define BLOCKSIZE 16
#define EXPANDEDKEYSIZE 176
#define ROUNDNUM 10
void Print_ex(unsigned char * state)
{
    for(int i =0;i<16;i++)
    {
        printf("%2.2x%c", (unsigned char)state[i], ((i + 1) % 4) ? ' ' : '\n');

    }
}



char aesMainBefore(unsigned char * state,
                    unsigned char * expandedKey);

void createPreTable(unsigned char Pre_table1[4][256], unsigned char  Pre_table2[4][256],
                      unsigned char  Pre_table3[4][256], unsigned char  Pre_table4[4][256],
                      unsigned char * Counter, unsigned char * expandedKey,
                      int * Block_num);

char aesMainBefore(unsigned char * state,
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

void createPreTable(unsigned char Pre_table1[4][256], unsigned char  Pre_table2[4][256],
                      unsigned char  Pre_table3[4][256], unsigned char  Pre_table4[4][256],
                      unsigned char * Counter, unsigned char * expandedKey,
                      int * Block_num)
{   //Input에 대한 Pre_table를 생성한다. 필요한 블록의 갯수만큼 생성을 한다.
    //15 = pre_table1, 14 = Pre_table2, 13 = Pre_table3, 12 = Pre_table4
    unsigned char roundKey[BLOCKSIZE];
    unsigned char Counter_save[16];
    unsigned char temp[4];
    unsigned char seq[32] = {
            0,5,10,15,0,4,8,12,
            1,6,11,12,1,5,9,13,
            2,7,8,13,2,6,10,14,
            3,4,9,14,3,7,11,15
    };
    unsigned char * Save_expandedKey = expandedKey;
    int num = 0;
    int Block = *Block_num;
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<4;j++)
        {
            Counter_save[(j*4)+i] = Counter[j + (i*4)];
        }
    }
    createRoundKey(Save_expandedKey, roundKey);
    addRoundKey(Counter_save, roundKey);
    //0라운드 종료

    subBytes(Counter_save);
    shiftRows(Counter_save);
    mixColumns(Counter_save);
    createRoundKey(Save_expandedKey + 16, roundKey);
    addRoundKey(Counter_save, roundKey);
    //1라운드 종료
    subBytes(Counter_save);
    Pre_table1[0][Counter[15]] = Counter_save[0];
    Pre_table1[1][Counter[15]] = Counter_save[4];
    Pre_table1[2][Counter[15]] = Counter_save[8];
    Pre_table1[3][Counter[15]] = Counter_save[12];

    Pre_table2[0][Counter[14]] = Counter_save[1];
    Pre_table2[1][Counter[14]] = Counter_save[5];
    Pre_table2[2][Counter[14]] = Counter_save[9];
    Pre_table2[3][Counter[14]] = Counter_save[13];

    Pre_table3[0][Counter[13]] = Counter_save[2];
    Pre_table3[1][Counter[13]] = Counter_save[6];
    Pre_table3[2][Counter[13]] = Counter_save[10];
    Pre_table3[3][Counter[13]] = Counter_save[14];

    Pre_table4[0][Counter[12]] = Counter_save[3];
    Pre_table4[1][Counter[12]] = Counter_save[7];
    Pre_table4[2][Counter[12]] = Counter_save[11];
    Pre_table4[3][Counter[12]] = Counter_save[15];
    Print_ex(Counter_save);
    increment_counter(Counter,BLOCKSIZE);
    for(int i = 1;i<Block;i++)
    {
        for(int a = 0;a<4;a++)
        {
            for(int j = 0;j<4;j++)
            {
                Counter_save[(j*4)+a] = Counter[j + (a*4)];
            }
        }
        createRoundKey(Save_expandedKey, roundKey);
        addRoundKey(Counter_save, roundKey);
        //0라운드 종료

        subBytes(Counter_save);
        shiftRows(Counter_save);
        mixColumns(Counter_save);
        createRoundKey(Save_expandedKey + 16, roundKey);
        addRoundKey(Counter_save, roundKey);
        //1라운드 종료
        subBytes(Counter_save);
//        if(num == 15)
//        {
////            temp[0] = Counter_save[0];
////            temp[1] = Counter_save[5];
////            temp[2] = Counter_save[10];
////            temp[3] = Counter_save[15];
////            Byte4_Round(temp, Save_expandedKey,roundKey,seq);
//            Pre_table1[0][Counter[15]] = Counter_save[0];
//            Pre_table1[1][Counter[15]] = Counter_save[4];
//            Pre_table1[2][Counter[15]] = Counter_save[8];
//            Pre_table1[3][Counter[15]] = Counter_save[12];
//        }
//        else if(num == 14)
//        {
////            temp[0] = Counter_save[1];
////            temp[1] = Counter_save[6];
////            temp[2] = Counter_save[11];
////            temp[3] = Counter_save[12];
////            Byte4_Round(temp, Save_expandedKey,roundKey,seq+8);
//            Pre_table2[0][Counter[14]] = Counter_save[1];
//            Pre_table2[1][Counter[14]] = Counter_save[5];
//            Pre_table2[2][Counter[14]] = Counter_save[9];
//            Pre_table2[3][Counter[14]] = Counter_save[13];
//        }
//        else if(num == 13)
//        {
////            temp[0] = Counter_save[2];
////            temp[1] = Counter_save[7];
////            temp[2] = Counter_save[8];
////            temp[3] = Counter_save[13];
////            Byte4_Round(temp, Save_expandedKey,roundKey,seq+16);
//            Pre_table3[0][Counter[13]] = Counter_save[2];
//            Pre_table3[1][Counter[13]] = Counter_save[6];
//            Pre_table3[2][Counter[13]] = Counter_save[10];
//            Pre_table3[3][Counter[13]] = Counter_save[14];
//        }
//        else
//        {
////            temp[0] = Counter_save[3];
////            temp[1] = Counter_save[4];
////            temp[2] = Counter_save[9];
////            temp[3] = Counter_save[14];
////            Byte4_Round(temp, Save_expandedKey,roundKey,seq+24);
//            Pre_table4[0][Counter[12]] = temp[3];
//            Pre_table4[1][Counter[12]] = temp[7];
//            Pre_table4[2][Counter[12]] = temp[11];
//            Pre_table4[3][Counter[12]] = temp[15];
//        }
        Pre_table1[0][Counter[15]] = Counter_save[0];
        Pre_table1[1][Counter[15]] = Counter_save[4];
        Pre_table1[2][Counter[15]] = Counter_save[8];
        Pre_table1[3][Counter[15]] = Counter_save[12];

        Pre_table2[0][Counter[14]] = Counter_save[1];
        Pre_table2[1][Counter[14]] = Counter_save[5];
        Pre_table2[2][Counter[14]] = Counter_save[9];
        Pre_table2[3][Counter[14]] = Counter_save[13];

        Pre_table3[0][Counter[13]] = Counter_save[2];
        Pre_table3[1][Counter[13]] = Counter_save[6];
        Pre_table3[2][Counter[13]] = Counter_save[10];
        Pre_table3[3][Counter[13]] = Counter_save[14];

        Pre_table4[0][Counter[12]] = Counter_save[3];
        Pre_table4[1][Counter[12]] = Counter_save[7];
        Pre_table4[2][Counter[12]] = Counter_save[11];
        Pre_table4[3][Counter[12]] = Counter_save[15];
        increment_counter(Counter,BLOCKSIZE);
    }

}
void setPreTable(unsigned char Pre_table1[4][256], unsigned char Pre_table2[4][256],
                   unsigned char Pre_table3[4][256], unsigned char Pre_table4[4][256],
                   unsigned char * block, unsigned char * I_Vector)
{
    block[0] = Pre_table1[0][I_Vector[15]];
    block[1] = Pre_table2[0][I_Vector[14]];
    block[2] = Pre_table3[0][I_Vector[13]];
    block[3] = Pre_table4[0][I_Vector[12]];

    block[4] = Pre_table1[1][I_Vector[15]];
    block[5] = Pre_table2[1][I_Vector[14]];
    block[6] = Pre_table3[1][I_Vector[13]];
    block[7] = Pre_table4[1][I_Vector[12]];

    block[8] = Pre_table1[2][I_Vector[15]];
    block[9] = Pre_table2[2][I_Vector[14]];
    block[10] = Pre_table3[2][I_Vector[13]];
    block[11] = Pre_table4[2][I_Vector[12]];

    block[12] = Pre_table1[3][I_Vector[15]];
    block[13] = Pre_table2[3][I_Vector[14]];
    block[14] = Pre_table3[3][I_Vector[13]];
    block[15] = Pre_table4[3][I_Vector[12]];
    Print_ex(block);
}
    void aesEncryptBefore(unsigned char * input,
                 unsigned char * I_Vector,
                 int flag,
                 unsigned char  Pre_table1[4][256],
                 unsigned char  Pre_table2[4][256],
                 unsigned char  Pre_table3[4][256],
                 unsigned char  Pre_table4[4][256],
                 unsigned char * key,
                 unsigned char * output)
{

    int plainTextLength = strlen(input);
    int block_number = (plainTextLength%16 == 0?plainTextLength/BLOCKSIZE : plainTextLength/BLOCKSIZE + 1);
    //블록이 몇개정도 만들어져야 하는지 구한다.
    unsigned char block[10][BLOCKSIZE];
    unsigned char savechar[BLOCKSIZE];
    unsigned char expandedKey[EXPANDEDKEYSIZE];
    unsigned char save_I_Vector[BLOCKSIZE];
    for(int i =0;i<16;i++)
    {
        save_I_Vector[i] = I_Vector[i];
    }

    expandKey(expandedKey,key);
    if(!flag) // 사전연산 테이블을 생성
    {
        createPreTable(Pre_table1, Pre_table2, Pre_table3, Pre_table4,
                         I_Vector, expandedKey,&block_number);
    }
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
    //0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 =>
    //0  | 1 | 2 | 3
    //4  | 5 | 6 | 7
    //8  | 9 | 10| 11
    //12 | 13| 14| 15
    for(int i = 0;i<block_number;i++)
    {
        setPreTable(Pre_table1, Pre_table2, Pre_table3, Pre_table4, savechar, save_I_Vector);
        printf("--------------\n");

        aesMainBefore(savechar, expandedKey);
        XORBlock(block[i],savechar,BLOCKSIZE);
        increment_counter(save_I_Vector,BLOCKSIZE);
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

#endif //AES_ALGORITHM_SET_AES128_BEFOREHAND_H
