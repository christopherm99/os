section .text
%define ISR_BASE 0x20000c
%macro isr_stub 1
isr%1:
        push 0
        push %1
%if %1 = 80h
        jmp _isr_handler_err
%else
        jmp _isr_handler
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
        dw 0xdead ; isr low
        dw 8h     ; kernel cs
        db 0      ; reserved
%if i = 80h
        db 11101110b ; attributes (usermode)
%else
        db 10001110b ; attributes (kernelmode)
%endif
        dw 0xbeef ; isr high
%assign i i + 1
%endrep

idt_end:
idt_desc:
        dw (idt_end - idt)
        dd (idt)

global idt_load:function (idt_load.end - idt_load)
idt_load:
        push eax
        mov eax, isr0
%assign i 0
%rep 256
        mov [idt + 8 * i], ax
        shr eax, 16
        mov [idt + 8 * i + 6], ax
        add eax, 9h ; assume that isr stubs are 9 bytes long!
%assign i i + 1
%endrep
        lidt [idt_desc]
        pop eax
        ret
.end:

_isr_handler:
        push ebp
        push edi
        push esi
        push edx
        push ecx
        push ebx
        push eax

        extern interrupt_handler
        call interrupt_handler

        pop eax
        pop ebx
        pop ecx
        pop edx
        pop esi
        pop edi
        pop ebp

        add esp, 8 ; pop interrupt vector and error code

        iret

_isr_handler_err:

        push ebp
        push edi
        push esi
        push edx
        push ecx
        push ebx
        push eax

        extern interrupt_handler
        call interrupt_handler

        add esp, 4 ; pop eax (contains return value)
        pop ebx
        pop ecx
        pop edx
        pop esi
        pop edi
        pop ebp

        add esp, 8 ; pop interrupt vector and error code

        iret


