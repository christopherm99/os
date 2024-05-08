[BITS 32]
section .text
global gdt_load:function (gdt_load.end - gdt_load)
gdt_load:
        lgdt [gdt_desc]
        jmp 8h:.reload_CS
.reload_CS:
        mov ax, 10h
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        ret
.end:

gdt:
gdt_null:
        dq 0
gdt_code:
        dw 0FFFFh    ; first 16 bits of limit
        dw 0         ; first 16 bits of base address
        db 0         ; next 8 bits of base address
        db 9Ah       ; access byte
        db 11001111b ; last 4 bits of limit and flags
        db 0         ; last 8 bits of base address
gdt_data:
        dw 0FFFFh
        dw 0
        db 0
        db 92h
        db 11001111b
        db 0
gdt_end:
gdt_desc:
        dw (gdt_end - gdt)
        dd (gdt)

