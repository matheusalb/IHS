[bits 16]
[org 0x7c00]
    
    jmp 0x0000:kernel_start

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

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


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

; MODO PROTEGIDO A PARTIR DAQUI
[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

section .data
    n dq 7.0 ; altera-se
    m1 dq 1.0
    resultado dq 0.0
    m2 dq 2.0
    str: db 'Valor = %f', 10,0        ;string para printf
section .bss
    memoriaResultado resq 1


section .text
global b32

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

    mov ecx, 7

    .loopa:
    fld qword[m2]
    fld qword[n]
    fmulp st1, st0 ; n * 2

    fld qword[m1]
    faddp st1, st0 ; n*2 + 1

    fld qword[m1]
    fdivrp st1, st0 ; st0/st1 == 1 / n*2 + 1

    fld qword[resultado] ; st0 == resultado, st1 == 1 / n*2 + 1

    ; Mod 2
    push edx 
    push eax
    push ebx 
    mov ebx, 0
    mov edx, 0
    mov eax, 0
    mov eax, ecx
    mov ebx, 2
    div ebx ; resto em edx

    cmp edx, 0
    je .soma

    .subtrai:
        fsubp st0, st1 ; resultado - fracao
    jmp .terminasoma
    .soma:
        faddp st0, st1 ;
    .terminasoma:
        fstp qword[memoriaResultado]
        mov resultado, qword[memoriaResultado]  


        push dword [resultado+4]            ;empilhando o valor double
        push dword [resultado]                ;em duas partes de 32 bits
        push dword str                     ;empilhando endereco da string
        call printf                        ;chamada de printf, que irá utilizar os valores empilhados


    loop .loopa
    



    jmp $

[SECTION signature start=0x7dfe]
dw 0AA55h