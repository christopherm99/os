; Limine requests
section .requests_start_marker
align 8
        dq 0xf6b8f4b39de7d1ae
        dq 0xfab91a6940fcb9cf
        dq 0x785c6ed015d3e316
        dq 0x181e920a7852b9d9
section .requests
        dq 0xf9562b2d5c95a6c8 ; BASE REVISION MAGIC
        dq 0x6a7b384944536bdc
        dq 3                  ; LIMINE PROTOCOL REV 3
section .requests_end_marker
align 8
        dq 0xadc0e0531bb10d03
        dq 0x9572709f31764c62


; Initialize the stack
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

; Linker entrypoint! First code that ever gets executed.
section .text
global _start:function (_start.end - _start)
_start:
        ; Limine has loaded us into 64-bit long mode, with stack, and GDT, and initial paging

        ; We need to setup the IDT and setup 1:1 paging.
        ; First, we will setup 1:1 paging:

        ; We begin by disabling paging
        mov RAX, 0xDEADBEEF
        int 0xF
        cli
.hang:  hlt
        jmp .hang
.end:

