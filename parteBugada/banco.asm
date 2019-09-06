org 0x7c00
jmp 0x0000:start

data:
    string times 11 db 0
    nome db 'Caralho', 0

start:
    mov di, string
	call gets

	mov si, string
	call prints

jmp start

putchar:
    mov ah, 0x0e
    int 10h
    ret
getchar:
    mov ah, 0x00
    int 16h
    ret
delchar:
    mov al, 0x08                    ; backspace
    call putchar
    mov al, ' '
    call putchar
    mov al, 0x08                    ; backspace
    call putchar
    ret
endl:
    mov al, 0x0a                    ; line feed
    call putchar
    mov al, 0x0d                    ; carriage return
    call putchar
    ret
gets:                           ; mov di, string
    push di
    xor cx, cx                  ; zerar contador
    .loop1:
        call getchar
        cmp al, 0x08                ; backspace
        je .backspace
        cmp al, 0x0d                ; carriage return
        je .done
        cmp cl, 10	           ; string limit checker
        je .loop1
        stosb
        inc cl
        call putchar
       
        jmp .loop1
        .backspace:
            cmp cl, 0           ; is empty?
            je .loop1
            dec di
            dec cl
            mov byte[di], 0
            call delchar
        jmp .loop1
    .done:
    mov al, 0
    stosb
    call endl
    pop di
    ret

prints:             ; mov si, string
  .loop:
    lodsb           ; bota character em al 
    cmp al, 0
    je .endloop
    call putchar
    jmp .loop
  .endloop:
  call endl
  ret

times 510-($-$$) db 0
dw 0xaa55