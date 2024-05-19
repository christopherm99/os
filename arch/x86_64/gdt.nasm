section .text
global gdt_load
gdt_load:
        mov  rax, gdt_desc
        lgdt [rax]
        push 8h
        lea  rax, [rel .reload_CS]
        push rax
        retfq
.reload_CS:
        mov ax, 10h
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        ret
.end:

section .data
align 4096
gdt:
.null:
        dq 0
.kernel_code:
        dw 0         ; first 16 bits of limit
        dw 0         ; first 16 bits of base address
        db 0         ; next 8 bits of base address
        db 9Ah       ; access byte
        db 10100000b ; last 4 bits of limit and flags
        db 0         ; last 8 bits of base address
.kernel_data:
        dw 0FFFFh
        dw 0
        db 0
        db 92h
        db 11000000b
        db 0
.end:
gdt_desc:
        dw (gdt.end - gdt)
        dq (gdt)

