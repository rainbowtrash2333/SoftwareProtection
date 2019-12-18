#pragma once
#include<Windows.h>
void Encryption(char* username, char* key, char* parameter)
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

    int b = 0;
    char* a = username;
    char* char2 = key;
    int index = 0;
    int i = 0;
    char tempc = ' ';
    // 48 75
    while (a[i] != '\0') {
        index = (i % 4) * 8;
        tempc = (char)(((((int)a[i] ^ (int)(d1 >> index))) % 5) + 48 + 5);
        if (tempc != char2[i]) {
            b = 1;
            break;
        }
        ++i;
    }

    __asm {
        push eax
        push ebx
        mov eax, parameter
        mov ebx, b
        mov byte ptr ds : [eax] , -68
        inc eax
        mov byte ptr ds : [eax] , -92
        inc eax
        mov byte ptr ds : [eax] , -69
        inc eax
        mov byte ptr ds : [eax] , -18
        inc eax

        cmp ebx, 0
        je L2
        mov byte ptr ds : [eax] , -54
        inc eax
        mov byte ptr ds : [eax] , -89
        inc eax
        mov byte ptr ds : [eax] , -80
        inc eax
        mov byte ptr ds : [eax] , -36
        jmp ok

        L2 :
        mov byte ptr ds : [eax] , -77
            inc eax
            mov byte ptr ds : [eax] , -55
            inc eax
            mov byte ptr ds : [eax] , -71
            inc eax
            mov byte ptr ds : [eax] , -90


            ok :
            inc eax
            mov byte ptr ds : [eax] , -93
            inc eax
            mov byte ptr ds : [eax] , -95
            inc eax
            mov byte ptr ds : [eax] , 0
            pop eax
            pop ebx
    }

}
