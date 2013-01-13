typedef struct
{
    unsigned edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned eip, eflags;
} jmp_buf[1];
