
#ifndef AES_FILEIO_H
#define AES_FILEIO_H
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define BUFFER_SIZE 1024


void Create_Write_File(char * FilePath, const char *data, const char *mode,int textsize);
void Create_Folder(char * FolderPath);

int Reading_File(unsigned char * FilePath, char *Save);

int Exists_Folder(const char *FolderPath);
int Exists_File(const char *FileName);

//FilePath에 파일을 생성할 경로와 파일의 이름을 모두 입력한다.
void Create_Write_File(char * FilePath, const char *data, const char *mode,int textsize)
{
    if(Exists_File(FilePath))
    {
        printf("동일한 파일이 이미 존재합니다.\n");
    }
    FILE * file = fopen(FilePath, mode);

    if(file == NULL)
    {
        perror("파일을 여는데 실패했습니다.\n");
        return;
    }
    //파일에 데이터를 입력한다.
    for(int i = 0;i< textsize;i++)
    {
        fprintf(file, "%2.2x%c", (unsigned char)data[i], ((i + 1) % 16) ? ' ' : '\n');
    }

    //생성한 파일을 입력하고 닫아준다.
    fclose(file);
    printf("파일의 작성이 안료되었습니다.\n");
}
//파일에 있는 내용을 읽어오는 함수,

int Reading_File(unsigned char * FilePath, char *Save)
{
    char hexString[3];
    int textsize = 0;
    int index = 0;
    unsigned char value;
    FILE *file;
    file = fopen(FilePath, "r");
    if(file == NULL)
    {
        perror("파일을 여는데 실패했습니다.");
        return 0;
    }
    while(fscanf(file, "%2s", hexString) == 1)
    {
        sscanf(hexString, "%hhx", &value);
        Save[index] = value;
        index++;
        textsize++;
    }
    fclose(file);
    return textsize;
}
//폴더의 이름을 포함한 경로를 FolderPath에 적어주면 된다.
void Create_Folder(char * FolderPath)
{
if(!Exists_Folder(FolderPath)) {
#ifdef _WIN32
    if(CreateDirectory(FolderPath, NULL))
    {
        printf("새로운 파일을 만드는데 성공했습니다\n");
    }
    else
    {
        printf("새로운 파일을 만드는데 실패했습니다.\n");
    }
#else
    if (mkdir(FolderPath, 0755) == 0) {
        printf("새로운 파일을 생성하는데 성공했습니다.\n");
    } else {
        perror("새로운 파일을 만드는데 실패했습니다\n");
    }
#endif
}
    printf("이미 폴더가 존재합니다.\n");
}
//폴더가 있는지 없는지 여부를 확인하는 함수
//FolderPath에 파일을 생성할 경로와 파일의 이름을 모두 입력한다.
//만약 동일한 폴더가 존재한다 : 0을 반환한다.
//만약 동일한 폴더가 존재하지 않는다 : 1을 반환한다.
int Exists_Folder(const char *FolderPath)
{
#ifdef _WIN32
    struct _stat statbuf;
    if(_stat(FolderPath, &statbuf) != -1){
        if(statbuf.st_mode & _S_IFDIr)
            {
            return 1;
            }
    }
#else
    struct stat statbuf;
    if(stat(FolderPath, &statbuf) != -1)
    {
        if(S_ISDIR(statbuf.st_mode)){
            return 1;
        }
    }
#endif
    return 0;
}

//파일이 존재하는지 존재여부를 판단하는 함수
//동일한 이름의 파일이 존재 : 1을 반환한다.
//동일한 이름의 파일이 없다 : 0을 반환한다.
//FilePath에 파일을 생성할 경로와 파일의 이름을 모두 입력한다.
int Exists_File(const char *FileName)
{
#ifdef _WIN32
    struct _stat buffer;
    return (_stat(FileName, &buffer) == 0);
#else
    struct stat buffer;
    return (stat(FileName, &buffer) == 0);
#endif
}
#endif //AES_FILEIO_H
