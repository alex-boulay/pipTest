#define __ASSEMBLY__
#include <pip/vidt.h>
.section .text
.global boot
.extern main
.extern Pip_VCLI
boot:
    call Pip_VCLI
    push %ebx
    call main

loop:
    jmp loop



INTERRUPT_HANDLER(timerAsm,timerHandler)
INTERRUPT_HANDLER(takebackAsm,takebackHandler)
