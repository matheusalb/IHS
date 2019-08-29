org 0x7c00 
jmp 0x0000:main

data: 
    nome times 22 db 0
    cpf times 15 db 0
    numderoDaConta times 2 db 0
    opcao times 4 db 0
    string times 101 db 0

    stBoaVinda db 'Selecione uma opcao: ', 0
    stCadastro db '1) Cadastrar Nova Conta', 0
    stBuscar db '2) Buscar Conta', 0
    stEditarConta db '3) Editar Conta', 0
    stDeletarConta db '4) Deletar Conta', 0
    stSair db '5) Sair', 0
    
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
    xor cx, cx                  ; zerar contador
    .loop1:
        call getchar
        cmp al, 0x08                ; backspace
        je .backspace
        cmp al, 0x0d                ; carriage return
        je .done
        cmp cl, 100             ; string limit checker
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
    ret
   
printString: ;printa a string apontada por si
    lodsb 
    cmp al,0 
    je endPrint
    mov ah, 0eh 
    int 10h 
    jmp printString
    endPrint:
ret

reverse:              ; mov si, string
  mov di, si
  xor cx, cx          ; zerar contador
  .loop1:             ; botar string na stack
    lodsb
    cmp al, 0
    je .endloop1
    inc cl
    push ax
    jmp .loop1
  .endloop1:
  .loop2:             ; remover string da stack        
    pop ax
    stosb
    loop .loop2
  ret

tostring:              ; mov ax, int / mov di, string
  push di
  .loop1:
    cmp ax, 0
    je .endloop1
    xor dx, dx
    mov bx, 10
    div bx            ; ax = 9999 -> ax = 999, dx = 9
    xchg ax, dx       ; swap ax, dx
    add ax, 48        ; 9 + '0' = '9'
    stosb
    xchg ax, dx
    jmp .loop1
  .endloop1:
  pop si
  cmp si, di
  jne .done
  mov al, 48
  stosb
  .done:
  mov al, 0
  stosb
  call reverse
ret

stoi:                ; mov si, string ; Coloca o resultado em ax
  xor cx, cx
  xor ax, ax
  .loop1:
    push ax
    lodsb
    mov cl, al
    pop ax
    cmp cl, 0        ; check EOF(NULL)
    je .endloop1
    sub cl, 48       ; '9'-'0' = 9
    mov bx, 10
    mul bx           ; 999*10 = 9990
    add ax, cx       ; 9990+9 = 9999
    jmp .loop1
  .endloop1:
ret

printInterface:
    mov si, stBoaVinda
    call printString
    call endl

    mov si, stCadastro
    call printString
    call endl

    mov si, stBuscar
    call printString
    call endl

    mov si, stEditarConta
    call printString
    call endl

    mov si, stDeletarConta
    call printString 
    call endl

ret

main:
    xor ax, ax ;zerando registradores
    mov ds, ax
    mov es, ax

    .loop:
        call printInterface

        mov di, opcao
        call gets ;escreve em quem di aponta

        mov si, opcao
        call stoi ;opcao em ax
        
        mov di, string
        call tostring
        mov si, string
        call printString
        call endl

times 510-($-$$) db 0
dw 0xaa55