section .text
%macro isr_stub 1
isr%1:
        push rdx
        push r8
        push r9
        mov rdx, %1
        mov r8, 0
        mov r9, [rsp+24]
%if %1 = 80h
        jmp  _isr_handler_err
%else
        jmp  _isr_handler
%endif
%endmacro

%assign i 0
%rep 256
isr_stub i
%assign i i + 1
%endrep

idt:
%assign i 0
%rep 256
        dw 0xdead     ; isr 0..15
        dw 8h         ; kernel cs
        db 0          ; ist (0 disable)
%if i = 80h
        db 11101110b  ; attributes (usermode)
%else
        db 10001110b  ; attributes (kernelmode)
%endif
        dw 0xbeef     ; isr 16..31
        dd 0xcafebabe ; isr 32..63
        dd 0          ; reserved
%assign i i + 1
%endrep

idt_end:
idt_desc:
        dw (idt_end - idt)
        dq (idt)

global idt_load
idt_load:
        push rax
%assign i 0
%rep 256
        mov  rax, isr%+i
        mov  [idt + 16 * i], ax
        shr  rax, 16
        mov  [idt + 16 * i + 6], ax
        shr  rax, 16
        mov  [idt + 16 * i + 8], eax
%assign i i + 1
%endrep
        lidt [idt_desc]
        pop  rax
        ret
.end:

extern interrupt_handler
_isr_handler:
        push r15
        push r14
        push r13
        push r12
        push r11
        push r10
        push rdi
        push rsi
        push rbp
        push rcx
        push rbx
        push rax

        mov  rcx, rsp

        call interrupt_handler

        pop  rax
        pop  rbx
        pop  rcx
        pop  rbp
        pop  rsi
        pop  rdi
        pop  r10
        pop  r11
        pop  r12
        pop  r13
        pop  r14
        pop  r15

        pop  r9
        pop  r8
        pop  rdx

        iretq

_isr_handler_err:

        push r15
        push r14
        push r13
        push r12
        push r11
        push r10
        push r9
        push r8
        push rdi
        push rsi
        push rbp
        push rbx
        push rax

        mov  rcx, rsp

        call interrupt_handler

        add  rsp, 8 ; pop eax (contains return value)
        pop  rbx
        pop  rcx
        pop  rdx
        pop  rbp
        pop  rsi
        pop  rdi
        pop  r8
        pop  r9
        pop  r10
        pop  r11
        pop  r12
        pop  r13
        pop  r14
        pop  r15

        pop  r9
        pop  r8
        pop  rdx
        iretq

