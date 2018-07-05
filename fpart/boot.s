.section .text
.global boot
.extern main
.extern Pip_VCLI
boot:
    call Pip_VCLI
    call main

loop:
    jmp loop
