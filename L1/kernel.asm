org 0x7e00 
jmp 0x0000:main

data: 
    opcao times 4 db 0
    string times 101 db 0

    ; do outro arquivo 
    banco times 195 db 0 ; calculo no comentario de cima

    nome times 7 db "NOME:",0 
    cpf times 6  db "CPF:",0 
    nconta times 9 db "N_CONTA:",0
    NOME_LEN equ 21
    CPF_LEN equ 12
    N_CONTA_LEN equ 2


    stBoaVinda db 'Selecione uma opcao: ', 0
    stCadastro db '1) Cadastrar Conta Nova', 0
    stBuscar db '2) Buscar Conta', 0
    stEditarConta db '3) Editar Conta', 0
    stDeletarConta db '4) Deletar Conta', 0
    stSair db '5) Sair', 0
    stAdeus db 'Ate logo!', 0
    stCheio db 'O bando de dados esta cheio', 0
    stTeste db 'achou usuario e printou', 0
    stBank db 'banco: ', 0

    stDigiteConta db 'Digite o numero da conta a ser buscada: ', 0
    stNotFound db 'Deu nao, boy!', 0  
; -------------------------------------------------------    
getString:
	; escaneia e printa
	call scan
	call print

	; apertou enter?
	cmp al, 0x0d
	je .done

	; guarda na memoria
	stosb

	; chamada recursiva
	jmp getString

	; fim da função
	.done:
        cmp cx, bx ; se a string nao tiver 20 chars + enter
        jne .completar ; completa com " " para manter tamanho fixo

		; coloca line break
		mov al, 13
		stosb
		mov al, 10
		stosb

        ret

        .completar:
            mov al, ' '
            inc cx
            stosb ; guarda na memoria
            jmp .done


scan:
    inc cx
	mov ah, 0
	int 16h

	ret


print:
	mov ah, 0xe	; chando o sistema de saída
	int 10h		; printando oq estiver em al

	ret


debug:
	mov al, 'd'
	call print

	ret

    
lineBreak:
	mov ax, 13 ; line feed
	call print

	mov ax, 10 ; carriage return
	call print

	ret
;;-------------------------------------------

printBanco: 
  mov si, banco
  push cx
  mov cx, 0

  .loopa:
    cmp cx, 195
    je .end
    lodsb
    mov ah, 0eh
    int 10h
    inc cx
    jmp .loopa

  .end:
    pop cx
ret

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
    cmp al, 0 
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

BuscarVazio:
  push cx
  push ax
  mov cx, 0
  mov si, banco 
  
  .loopi:
    cmp cx, 4
    je .done2
    lodsb ;pode bugar 
    
    dec si
    cmp al, 0
    je .done
    inc cx
    add si, 35
    jmp .loopi
  .done:
    pop cx
    pop ax
    ret

  .done2:
    pop cx
    pop ax
    mov si, -1
    ret

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

    mov si, stSair
    call printString
    call endl

ret

printConta:
  mov si, stTeste
  call printString
ret

Cadastrar:
  call BuscarVazio
  cmp si, -1
  je .full

  ; ====== escanear num conta ======
    mov bx, N_CONTA_LEN
	mov si, nconta
	call printString ; printa "N_CONTA:" pro usuario
    mov cx, 0 ; zera contador
    call getString
	call lineBreak 
    ; ====== escanear nome ======
    mov bx, NOME_LEN
	mov si,nome
	call printString ; printa "NOME:" 
    mov cx, 0 ; zera contador
    call getString
   	call lineBreak 
    ; ====== escanear cpf ======
    mov bx, CPF_LEN
	mov si,cpf
	call printString ; printa "CPF:"
    mov cx, 0 ; zera contador
    call getString;
	mov al, 0; fecha a string
	stosb ; guarda na memoria

	call lineBreak 
  jmp .end 

  .full:
    mov si, stCheio
    call printString
    call endl

  .end:


ret

Buscar:
  mov si, stDigiteConta
  call printString

  mov di, opcao
  call gets 
   
  mov si, banco
  cmpsb
  jne .naoEOPrimeiro
  dec si
  call printConta
  ret

  .naoEOPrimeiro:
    mov si, banco
    mov di, opcao
    add si, 35
    cmpsb
    jne .naoEOSegundo
    dec si
    call printConta
    ret
  
  .naoEOSegundo:
    mov si, banco
    mov di, opcao
    add si, 70
    cmpsb
    jne .naoEOTerceiro
    dec si
    call printConta
    ret

  .naoEOTerceiro:
    mov si, banco
    mov di, opcao
    add si, 105
    cmpsb
    jne .naoEOQuarto
    dec si
    call printConta
    ret
  
  .naoEOQuarto:
    mov si, banco
    mov di, opcao
    add si, 140
    cmpsb
    jne .naoEOQuinto
    dec si
    call printConta
    ret

  .naoEOQuinto:
    mov si, stNotFound
    call printString
    
ret

Editar: 


ret

Deletar:

ret

main:
    xor ax, ax ;zerando registradores
    mov ds, ax
    mov es, ax

    .principal:
        xor ax, ax ;zerando registradores
        mov ds, ax
        mov es, ax
        mov bx, ax
        mov cx, ax
        mov al, 0
        mov bl, 0

        mov si, stBank
        call printString
        call printBanco

        call printInterface

        mov di, opcao
        call gets ;escreve em quem di aponta

        mov si, opcao
        call stoi ;opcao em ax

        cmp ax, 1
        jne .naoCadastar
        call Cadastrar
        jmp .principal

        .naoCadastar:
          cmp ax, 2
          jne .naoBuscar
          call Buscar
          jmp .principal

        .naoBuscar:
          cmp ax, 3
          jne .naoEditar
          call Editar
          jmp .principal
        
        .naoEditar:
          cmp ax, 4
          jne .naoDeletar
          call Deletar
          jmp .principal

        .naoDeletar:
          cmp ax, 5 ; Sair
          jne .principal
          mov si, stAdeus
          call printString