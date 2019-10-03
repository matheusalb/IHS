[bits 16]
[org 0x7c00]
    
    jmp 0x0000:kernel_start


gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xFFFF                   ; limit low 
    dw 0x0                      ; base low  
    db 0x0                      ; base middle 
    db 10011010b                ; access
    db 11001111b                ; granularity
    db 0x0                      ; base high 

gdt_data:
    dw 0xFFFF                   ; limit low 
    dw 0x0                      ; base low  
    db 0x0                      ; base middle   
    db 10010010b                ; access
    db 11001111b                ; granularity
    db 0x0                      ; base high 

gdt16_code:
    dw 0xFFFF                   ; limit low 
    dw 0x0                      ; base low  
    db 0x0                      ; base middle   
    db 10011010b                ; access    
    db 10001111b                ; granularity   
    db 0x0                      ; base high 

gdt16_data:
    dw 0xFFFF                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10010010b                ; access
    db 10001111b                ; granularity
    db 0                        ; base high

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
DATA_SEG_16 equ gdt16_data - gdt_start
CODE_SEG_16 equ gdt16_code - gdt_start

string1 db 'Hello World 16!', 0
string3 db 'Hello World 16 again!', 0


print:
	mov ah, 0xe	; chando o sistema de saída
	int 10h		; printando oq estiver em al
ret

printString16:
    lodsb
    call print
    cmp al, 0
    je .done
    cmp al,13
    je .done
    jmp printString16
    
    .done:
ret

kernel_start:
    mov ax, 0
    mov ss, ax
    mov sp, 0xFFFC

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov si, string1  
    call printString16     
    

    cli                     ; Limpa a flag de interrupção
    lgdt[gdt_descriptor]    ; Carrega na tabela global de descritores (gdtr aponta para o limite da GDT e seu endereço base)
    
    ; Coloca 1 no bit PE do reg CRO 
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Realiza o far jump
    jmp CODE_SEG:b32



idt_real:
    dw 0x3ff        ; 256 entries, 4b each = 1K
    dd 0            ; Real Mode IVT @ 0x0000


savcr0:
    dd 0            ; Storage location for pmode CR0.


Entry16: 
    cli             ; Disable interrupts.
 
    ; Need 16-bit Protected Mode GDT entries!
    mov eax,DATA_SEG_16 ; 16-bit Protected Mode data selector.
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
 
 
    ; Disable paging (we need everything to be 1:1 mapped).
    mov eax, cr0
    mov [savcr0], eax   ; save pmode CR0
    and eax, 0x7FFFFFFe ; Disable paging bit & disable 16-bit pmode.
    mov cr0, eax

    jmp 0:GoRMode       ; Perform Far jump to set CS (going to real mode). 

 
GoRMode:
    mov sp, 0x8000      ; pick a stack pointer.
    mov ax,0            ; Reset segment registers.
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    lidt [idt_real]
    
    sti                 ; Restore interrupts -- be careful, unhandled int's will kill it.

    jmp kernel_end
 

kernel_end:
    mov si, string3      
    call printString16

    jmp $



[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

string2 db 'Hello World 32!', 0


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

printString32:
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

    mov ecx,40000000
    .loop:
        loop .loop

    call clear_screen

    ;mov al,'1'
    ;mov ah, 0xe
    ;int 10h

    mov ebx, string2
    call printString32


    mov ecx,40000000
    .loop1:
        loop .loop1

    call clear_screen

    jmp CODE_SEG_16:Entry16


times 510 - ($ - $$) db 0
dw 0AA55h