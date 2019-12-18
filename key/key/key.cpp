// key.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Windows.h>
# include<Windows.h>
# include<ostream>
#include"md5.h"

#pragma once
#ifndef FLOWER1
#define FLOWER1 __asm{      \
    push eax \
    mov eax, 0x1 \
    xor eax,eax \
    cmp ebx, 0 \
    je label \
    __asm _emit 0x25 \
    __asm _emit 0xFF \
    __asm _emit 0xFF \
    __asm _emit 0xFF \
    __asm _emit 0xFF \
    __asm _emit 0x83 \
    __asm _emit 0xEE \
    __asm _emit 0x05 \
    __asm _emit 0x83 \
    __asm _emit 0xEE \
    __asm _emit 0x0F \
    label:\
    pop eax \
};
#endif
#ifndef FLOWER2
#define FLOWER2 __asm{      \
    jz label\
    jnz label\
    _emit 0xe8\
    label:\
};
#endif
#ifndef FLOWER3
#define FLOWER3 __asm{      \
    push eax \
    push ebx \
    __asm _emit 0x55 \
    __asm _emit 0x90 \
    __asm _emit 0x83 \
    mov eax 0x12 \
    xor eax ebx \
    __asm _emit 0x90 \
    __asm _emit 0x83 \
    __asm _emit 0xC6 \
    pop eax \
    pop ebx \
};
#endif

UINT64 get_cpuid(char* username)
{

    DWORD dwStandard = 0;
    DWORD dwFeature = 0;

    _asm {
        mov eax, 1
        cpuid
        mov dwStandard, eax
        mov dwFeature, edx
    }
    DWORD d1 = dwStandard ^ dwFeature;

    bool b = true;
    char* a = username;

    int index = 0;
    int i = 0;
    // 48 75
    while (a[i] != '\0') {
        index = (i % 4) * 8;
        a[i] = (char)(((((int)a[i] ^ (int)(d1 >> index))) % 5) + 48 + 5);
        ++i;
    }
   
}


int main(int argc, char* argv[]) {
    __asm {      
    push eax 
        mov eax, 0x1 
        xor eax, eax 
        cmp ebx, 0 
        je label 
        __asm _emit 0x25 
    __asm _emit 0xFF 
    __asm _emit 0xFF 
    __asm _emit 0xFF 
    __asm _emit 0xFF 
    __asm _emit 0x83 
    __asm _emit 0xEE 
    __asm _emit 0x05 
    __asm _emit 0x83 
    __asm _emit 0xEE 
    __asm _emit 0x0F 
    label:
    pop eax 
    };   
    char* name;
    if (argc == 2)
    {
        name = argv[1];
    }
    else if (argc > 2)
    {
        printf("参数过多\n");
        return 0;
    }
    else
    {
        printf("请输入密钥\n");
        return 0;
    }
    char char1[100];
    std::string username = name;
    strcpy(char1, username.c_str());
    get_cpuid(char1);
    std::cout << "激活码: " << char1 << std::endl;
}

