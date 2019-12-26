#pragma once
#ifndef FLOWER1
#define FLOWER1 __asm{      \
    __asm _emit 0x55 \
    __asm _emit 0x90 \
    __asm _emit 0x83 \
    __asm _emit 0xC6 \
    __asm _emit 0x14 \
    __asm _emit 0x5D \
    __asm jnz $+13    \
    __asm jz $+7     \
    __asm _emit 0xCC \
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
};
#endif
#ifndef FLOWER2
#define FLOWER2 __asm{  \
    __asm jz label2 \
    __asm jnz label2 \
    __asm _emit 0xe8 \
    __asm label2: \
}
#endif
#ifndef FLOWER3
#define FLOWER3 __asm { \
    __asm xor esi, 011223344h \
    __asm add esi, eax \
    __asm add eax, ebx \
    __asm mov edx, eax \
    __asm shl edx, 4 \
    __asm mul ecx \
    __asm xor esi, ecx \
}
#endif
#ifndef FLOWER4
#define FLOWER4 __asm{      \
    __asm _emit 0x55 \
    __asm _emit 0x90 \
    __asm _emit 0x83 \
    __asm _emit 0xC6 \
    __asm _emit 0x14 \
    __asm _emit 0x5D \
    __asm jnz $+13    \
    __asm jz $+7     \
    __asm _emit 0xCC \
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
}
#endif
