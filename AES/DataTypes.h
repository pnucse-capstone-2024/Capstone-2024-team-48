//
// Created by 임준식 on 8/11/24.
//

#ifndef AES_DATATYPES_H
#define AES_DATATYPES_H

enum keySize
{
    SIZE_16 = 16,
    SIZE_24 = 24,
    SIZE_32 = 32
}keySize;
enum errorCode
{
    SUCCESS = 0,
    ERROR_AES_UNKNOWN_KEYSIZE,
    ERROR_MEMORY_ALLOCATION_FAILED,
};
typedef enum operationMode
{
    ECB = 1,
    CBC,
    CTR,
    OFB,
    CFB,
    GCM
} MODE;
#endif //AES_DATATYPES_H
