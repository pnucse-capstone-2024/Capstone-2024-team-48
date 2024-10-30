#include <stdio.h>
#include <stdlib.h>
#include "headerFile/Conversion.h"
#include "headerFile/AES_128_CTR.h"
#define KEYSIZE128 16
#define PlainText_Size 1024
#define CipherText_SIZE 1024

void clearInputBuffer();
void Pringting_Block(unsigned char * Block, int size);
void Printing_Block_Bit(unsigned char *Block, int size);
void printBits(unsigned char byte);
int hexCharToInt(char c);
int xnor(int a, int b);
void hexStringToBytes(const char *hexString, unsigned char *bytes, int byteLength);

int xnor(int a,int b)
{
    return (a ^ b);
}

void Start_Option_1(unsigned char * Plaintext,
                  unsigned char * Ini_V,
                  unsigned char * Key,
                  unsigned char * Counter,
                  int * Algorithm,
                  int * operatingMode)
{
    unsigned char input_Plaintext[2048];   // 충분한 버퍼 크기
    unsigned char input_key[33];           // 충분한 크기
    unsigned char input_Counter[33];       // 충분한 크기
    unsigned char input_IniV[33];          // 충분한 크기

    // 알고리즘 선택 입력
    printf("Select algorithm(1 = AES) : ");
    scanf("%d", Algorithm);
    system("clear");

    // 운영 모드 선택 입력
    printf("Select the operating mode(1 = ECB, 2 = CBC, 3 = CFB, 4 = OFB, 5 = CTR, 6 = GCM): ");
    scanf("%d", operatingMode);
    system("clear");
    clearInputBuffer();
    // Plaintext 입력
    printf("Input PlainText (hex): ");
    fgets(input_Plaintext, sizeof(input_Plaintext), stdin);
    system("clear");

    // Key 입력
    printf("Input encryption Key Value (hex): ");
    fgets(input_key, sizeof(input_key), stdin);
    system("clear");
    clearInputBuffer();
    // Counter 값 입력 (CTR 또는 GCM 모드일 때)
    if(*operatingMode == 5 || *operatingMode == 6)
    {
        printf("Input counter value (hex): ");
        fgets(input_Counter, sizeof(input_Counter), stdin);
        hexStringToBytes(input_Counter, Counter, strlen(input_Counter)/2);  // 변환
    }

    // Initialize Vector 값 입력 (CBC, CFB, OFB 모드일 때)
    if(*operatingMode == 2 || *operatingMode == 3 || *operatingMode == 4)
    {
        printf("Input Initialize Vector (hex): ");
        if (fgets(input_IniV, sizeof(input_IniV), stdin) != NULL) {
            input_IniV[strcspn(input_IniV, "\n")] = 0;         // 개행 문자 제거
            hexStringToBytes(input_IniV, Ini_V, strlen(input_IniV)/2);  // 변환
        }
    }

    system("clear");

    // 입력한 16진수 문자열을 바이너리로 변환
    hexStringToBytes(input_Plaintext, Plaintext, strlen(input_Plaintext)/2);  // 변환
    hexStringToBytes(input_key, Key, strlen(input_key)/2);  // 변환
}
void Start_Option_2(unsigned char * Counter,
                    unsigned char * Key,
                    int * Counter_count)
{
    clearInputBuffer();
    unsigned char input_key[33];           // 충분한 크기
    unsigned char input_Counter[33];       // 충분한 크기
    int input_Counter_Count;
    printf("Input encryption Key Value (hex): ");
    fgets(input_key, sizeof(input_key), stdin);
    system("clear");
    clearInputBuffer();
    printf("Input counter value (hex): ");
    fgets(input_Counter, sizeof(input_Counter), stdin);
    system("clear");
    printf("Input Counter Count");
    scanf("%d", &input_Counter_Count);
    system("clear");
    *Counter_count = input_Counter_Count;
    hexStringToBytes(input_Counter, Counter, strlen(input_Counter)/2);  // 변환
    hexStringToBytes(input_key, Key, strlen(input_key)/2);  // 변환
}
void Printing_Block(unsigned char * Block, int size)
{
        for(int q = 0;q<size; q++)
        {
            printf("+--++--++--++--+      ");
        }
        printf("\n");
        for(int i = 0;i<4;i++)
        {
            for(int a = 0; a<size;a++)
            {
                for(int j = 0;j<4;j++)
                {
                    printf("|");
                    printf("%02x", Block[(a*16)+(j * 4) + i]);
                    printf("|");
                }
                if(a!=size-1)
                {
                    printf("  ->  ");
                }
            }
            printf("\n");
        }
    for(int q = 0;q<size; q++)
    {
        printf("+--++--++--++--+      ");
    }

        printf("\n");
}
void Printing_Block_Bit(unsigned char *Block, int size)
{
    for(int q = 0;q<size; q++)
    {
        printf("+---------++---------++---------++---------+      ");
    }
    printf("\n");
    for(int i = 0;i<4;i++)
    {
        for(int a = 0; a<size;a++)
        {
            for(int j = 0;j<4;j++)
            {
                printf("|");
                printBits(Block[(a*16)+(j*4) + i]);
                printf("|");
            }
            if(a!=size-1)
            {
                printf("  ->  ");
            }
        }
        printf("\n");
    }
    for(int q = 0;q<size; q++)
    {
        printf("+---------++---------++---------++---------+      ");
    }

    printf("\n");

}
void Printing_Block_Compare_bit(unsigned char * Block, int num1, int num2)
{
    unsigned char Comp_Block[48];
    for(int k = 0;k<10;k++)
    {
        for(int i = 0;i<4;i++)
        {
            for(int j = 0;j<4;j++)
            {
                Comp_Block[(4 * i) + j] = Block[(160 * (num1-1)) + (k * 16) + (4 * i) + j];
                Comp_Block[16 + (4 * i) + j] = Block[(160 * (num2-1)) + (k * 16) + (4 * i) + j];
                Comp_Block[32 + (4 * i) + j] = xnor(Block[(160 * (num1-1)) + (k * 16) + (4 * i) + j],Block[(160 * (num2-1)) + (4 * i) + j]);
            }
        }
        Printing_Block_Bit(Comp_Block,3);
    }



}
void Printing_Block_Compare_bit_2(unsigned char * Save_Cipher, int * Count)
{
    int diff[4];
    int num1 = 0;
    for(int i = 0; i < *Count; i++)
    {
        for(int j = 0;j < 3;j++)
        {
            printf("+---------++---------++---------++---------+      ");
        }
        printf("\n");
        for(int j = 0;j < 4;j++)
        {
            for(int a = 0;a<2;a++)
            {
                for(int k = 0;k<4;k++) //이게 한번 끝날때 한줄이 출력된다.
                {
                    printf("|");
                    printBits(Save_Cipher[((num1 + a) * 16) + (k*4) + j]);
                    if(a == 0)
                    {
                        diff[k] = Save_Cipher[((num1 + a) * 16) + (k*4) + j] ^ Save_Cipher[((num1 + (a+1)) * 16) + (k*4) + j];
                    }
                    printf("|");
                }
                if(a == 0)
                {
                    printf("  ->  ");
                }
            }
            printf("  ==  ");
            for(int a = 0;a<4;a++)
            {
                printf("|");
                printBits(diff[a]);
                printf("|");
            }
            printf("\n");
        }
        num1++;
        for(int j = 0;j < 3;j++)
        {
            printf("+---------++---------++---------++---------+      ");
        }
        printf("\n");
    }

}
void printBits(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        if(i == 3)
        {
            printf(" ");
        }
        printf("%d", (byte >> i) & 1);  // 각 비트를 오른쪽으로 시프트 후 마스크로 추출
    }
}
// 16진수 문자를 숫자로 변환하는 함수
int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

// 16진수 문자열을 unsigned char 배열로 변환
void hexStringToBytes(const char *hexString, unsigned char *bytes, int byteLength) {
    for (int i = 0; i < byteLength; i++) {
        bytes[i] = hexCharToInt(hexString[2*i]) * 16 + hexCharToInt(hexString[2*i + 1]);
    }
}
void clearInputBuffer(){
    int c;
    // getchar()로 입력 버퍼에 남아 있는 문자들을 모두 제거
    while ((c = getchar()) != '\n' && c != EOF);
}

void display_1(unsigned char * PlainText,
               unsigned  char * CipherText,
               unsigned char * Save_CipherRound,
               int * Block_num)
{
    int select = 0;
    int select_CipherNumber;
    int select_num1;
    int select_num2;
    while(1)
    {
        printf("select display Mode : (0 = Stop Program, 1 = Printf Plain, Cipher, 2 = Round Procedure, 3 = Compare Two Round)");
        scanf("%d",&select);
        system("clear");
        if(select == 0)
        {
            break;
        }
        switch(select)
        {
            case 1: //평문에 대한 암호문을 출력
                printf("Print Text\n");
                Printing_Block(PlainText, *Block_num);
                printf("Print CipherText\n");
                Printing_Block(CipherText,*Block_num);
                break;
            case 2: //특정 암호화에 대한 라운드 단계 배열을 모두 출력
                printf("Select Cipher Number : ");
                scanf("%d", &select_CipherNumber);
                Printing_Block(Save_CipherRound + ((select_CipherNumber-1)*160) , 10);
                break;
            case 3: //원하는 평문 블록에 대한 라운드를 비교
                printf("select Cipher Number1 : "); // 원하는 평문 블록
                scanf("%d",&select_num1);
                printf("select Cipher Number2 : "); // 원하는 평문 블록
                scanf("%d",&select_num2);
                Printing_Block_Compare_bit(Save_CipherRound, select_num1, select_num2);
                break;
            case 4: //평문 블록에서 나온 암호문의 비트를 비교하는 방법
                printf("select Cipher Number1 : "); // 원하는 평문 블록
                scanf("%d",&select_num1);
                printf("select Cipher Number2 : "); // 원하는 평문 블록
                scanf("%d",&select_num2);
                break;
            case 5: //CTR 모드를 사용한다고 했을때 원하는 키값을 넣었을때

        }

    }
}

void display_2(unsigned char * Save_Cipher, int * Count)
{
    Printing_Block_Compare_bit_2(Save_Cipher, Count);
}
int main() {
    int Algorithm;
    int operatingMode;
    int Block_num = 0;
    int DisplayMode;
    int Counter_num;
    unsigned char Key[KEYSIZE128];
    unsigned char PlainText[PlainText_Size];
    unsigned char Initialize_V[BLOCKSIZE];
    unsigned char Counter[BLOCKSIZE];
    unsigned char CipherText[CipherText_SIZE];
    unsigned char Save_CipherRound[1600]; // 이렇게 수행하면 하나의 블록을 한번에 저장이 가능하다.
    unsigned char Save_Cipher[1600];
    unsigned char Save_Counter[1600];

    printf("Insert DisplayMode : ");
    scanf("%d", &DisplayMode);
    switch (DisplayMode) {
        case 1:
            Start_Option_1(PlainText,Initialize_V, Key, Counter, &Algorithm, &operatingMode);
            Block_num = strlen(PlainText)/16;
            if(strlen(PlainText)%16 != 0)
            {
                Block_num++;
            }
            aes_encrypt_CTR(PlainText, CipherText, Key, Counter,Save_CipherRound);
            display_1(PlainText, CipherText, Save_CipherRound, &Block_num);
            break;
        case 2: //CTR 모드의 패턴을 파악하기 위해서 만든 모드, 카운터의 값이 1씩 증가하면서
            Start_Option_2(Counter, Key, &Counter_num);//카운터,키, 몇개를 입력받을지를 입력해준다.
            for(int i = 0;i<Counter_num;i++)
            {
                aes_encrypt_CTR_Testing(Counter, Save_Cipher + (i*BLOCKSIZE) ,Key);
                increment_counter(Counter,BLOCKSIZE);
            }
            display_2(Save_Cipher, &Counter_num);
            break;

    }


    return 0;
}
