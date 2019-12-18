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