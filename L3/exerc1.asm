[bits 16]
[org 0x7c00]
    
    jmp 0x0000:kernel_start

gdt16_start:
gdt16_null:
    dd 0x0
    dd 0x0

gdt16_code:
    dw 0xFFFF                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10011010b                ; access
    db 10001111b                ; granularity
    db 0                        ; base high
 
gdt16_data:
    dw 0xFFFF                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10010010b                ; access
    db 10001111b                ; granularity
    db 0                        ; base high

gdt16_end:
gdt16_descriptor:
    dw gdt16_end - gdt16_start 
    dd gdt16_start

; CODE_16_SEG equ gdt16_code - gdt16_start
; DATA_16_SEG equ gdt16_data - gdt16_start

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_16_code:
    dw 0xFFFF                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10011010b                ; access
    db 10001111b                ; granularity
    db 0                        ; base high
 
gdt_16_data:
    dw 0xFFFF                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10010010b                ; access
    db 10001111b                ; granularity
    db 0                        ; base high

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
CODE_16_SEG equ gdt_16_code - gdt_start
DATA_16_SEG equ gdt_16_data - gdt_start



kernel_start:
    mov ax, 0
    mov ss, ax
    mov sp, 0xFFFC

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    cli                     ; Limpa a flag de interrupção
    lgdt[gdt_descriptor]    ; Carrega na tabela global de descritores (gdtr aponta para o limite da GDT e seu endereço base)
    
    ; Coloca 1 no bit PE do reg CRO 
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Realiza o far jump
    jmp CODE_SEG:b32

 
GoRMode:
    mov sp, 0x8000      ; pick a stack pointer.
    mov ax, 0       ; Reset segment registers to 0.
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    lidt [idt_real]
    
    sti         ; Restore interrupts -- be careful, unhandled int's will kill it.
    jmp kernel_end
 
kernel_end:
    mov si, string2        
    printString:
        lodsb
        call print
        cmp al, 0
        je .done
    cmp al,13
    je .done
        jmp printString
        .done:
    ret

    print:
        mov ah, 0xe	; chando o sistema de saída
        int 10h		; printando oq estiver em al
    ret
    
    jmp $


[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

string db 'Hello World', 0
string2 db 'Tchau World', 0

clear_screen:
    pushad

    cld
    mov edi, 0xB8000
    mov cx, 80 * 25
    mov ah, 0x0F
    mov al, ' '
    rep stosw

    popad
    ret

print32:
    pusha
    mov edx, VIDEO_MEMORY
.loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je .done
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp .loop
.done:
    popa
    ret

b32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebp, 0x2000
    mov esp, ebp

    call clear_screen

    ; mov al,'1'
    ; mov ah, 0xe
    ; int 10h

    mov ebx, string
    call print32

    call clear_screen

    ; mov ecx,40000000
    mov ecx, 100
    .loop1:
        loop .loop1

    jmp CODE_SEG:Entry16

idt_real:
    dw 0x3ff        ; 256 entries, 4b each = 1K
    dd 0            ; Real Mode IVT @ 0x0000

savcr0:
    dd 0            ; Storage location for pmode CR0.
 
Entry16:
        ; We are already in 16-bit mode here!
 
    cli         ; Disable interrupts.
 
    ; Need 16-bit Protected Mode GDT entries!
    mov eax, DATA_SEG  ; 16-bit Protected Mode data selector.
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
 
 
    ; Disable paging (we need everything to be 1:1 mapped).
    mov eax, cr0
    ; mov [savcr0], eax   ; save pmode CR0
    and eax, 0x7FFFFFFe ; Disable paging bit & disable 16-bit pmode.
    mov cr0, eax

    lgdt[gdt16_descriptor]

    jmp CODE_16_SEG:GoRMode       ; Perform Far jump to set CS.

[SECTION signature start=0x7dfe]
dw 0AA55h