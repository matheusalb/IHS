org 0x7c00
jmp 0x0000:data

data:
xor ax,ax
mov ds,ax

mov ax, 0xB800
mov es, ax

call clear_screen

mov cx, 11
mov si, msg
call print

hlt

clear_screen:
    pusha
    mov cx, 80 * 25
    mov ax, 0x0F00

    xor di,di
    rep 
    stosw
    popa
    ret

print:
    pusha
    xor di,di

    .loop1:
        lodsb
        mov ah, 2
        cld

        stosw
        loop .loop1
        popa

        ret

msg db "Hello World"

times 510-($-$$) db 0
dw 0AA55H
