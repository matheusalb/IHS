org 0x7e00 
jmp 0x0000:main

data: 
  opcao times 2 db 0
  string times 101 db 0

    
  ; CALCULO BANCO:
  ; numero de usuarios: n_user = 5
  ;
  ; fechar string = \0 (tamanho 1)
  ; endl = 2
  ;
  ; nhttps://github.com/matheusalb/IHS_conta = 1 + endl = 3
  ; nome = 20 + endl = 22
  ; cpf = 11 + endl + \0 = 14
  ;
  ; tamanho do usuario: user_len = n_conta + nome + cpf = 3 + 22 + 14 = 39
  ; banco = user_len * n_user = 39 * 5 = 195 
  ;
  banco times 195 db 0 ; calculo no comentario de cima

  nome db "NOME:",0 
  cpf db "CPF:",0 
  nconta db "N_CONTA:",0

  NOME_LEN equ 20
  CPF_LEN equ 11
  N_CONTA_LEN equ 1

  std1 db "Achou no 1",0
  std2 db "Achou no 2",0
  std3 db "Achou no 3",0
  std4 db "Achou no 4",0
  std5 db "Achou no 5",0

  stBoaVinda db 'Selecione uma opcao: ', 0
  stCadastro db '1) Cadastrar Conta Nova', 0
  stBuscar db '2) Buscar Conta', 0
  stEditarConta db '3) Editar Conta', 0
  stDeletarConta db '4) Deletar Conta', 0
  stSair db '5) Sair', 0
  stAdeus db 'Ate logo!', 0
  stCheio db 'O banco de dados esta cheio!', 13,0
  stBank db 'banco: ', 0
  stdCadastrar db 'Digite os dados do novo cadastro:',13,0
  stDigiteConta db 'Digite o numero da conta a ser buscada:',0
  stDigiteContaEdit db 'Digite o numero da conta a ser editada:', 0
  stDigiteContaDel db 'Digite o numero da conta a ser deletada:', 0
  stNotFound db 'Conta inexistente!', 0  

;============================================================================== CÓDIGOS BÁSICOS =============================================================================================
delchar:
  mov al, 0x08                    ; backspace
  call print
  mov al, ' '
  call print
  mov al, 0x08                    ; backspace
  call print
ret


endl:
  mov al, 0x0a                    ; line feed
  call print
  mov al, 0x0d                    ; carriage return
  call print
ret


gets:                           ; mov di, string
  xor cx, cx                  ; zerar contador
  .loop1:
      call scan
      cmp al, 0x08                ; backspace
      je .backspace
      cmp al, 0x0d                ; carriage return
      je .done
      cmp cl, 100             ; string limit checker
      je .loop1
      stosb
      inc cl
      call print
      
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


scan:
	mov ah, 0
	int 16h
ret


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

; =================================================================================== CÓDIGO DO CADASTRO ===================================================================================
cadastrar:
  mov di, opcao
  mov al, 0
  stosb
  call Buscar
  cmp si, 0
  je .full

	; ====== escanear num conta ======
  mov bx, N_CONTA_LEN
	mov si, nconta
	call printString ; printa "N_CONTA:" pro usuario
  mov cx, 0 ; zera contador
  call getString
	call endl 

  ; ====== escanear nome ======
  mov bx, NOME_LEN
	mov si,nome
	call printString ; printa "NOME:" 
  mov cx, 0 ; zera contador
  call getString
  call endl 

  ; ====== escanear cpf ======
  mov bx, CPF_LEN
	mov si,cpf
	call printString ; printa "CPF:"
  mov cx, 0 ; zera contador
  call getString;
	mov al, 0; fecha a string
	stosb ; guarda na memoria

	call endl 

  .done:
    ret 
  .full:
    mov si, stCheio
    call printString
    ret 


;Função para realizar o input para o CADASTRO -> corrige o tamanho
getString:                 
  xor cx, cx               ; zerar contador
  .loop1:
    call scan
    cmp al, 0x08           ; backspace
    je .backspace
    cmp al, 0x0d           ; carriage return
    je .done
    cmp cl, bl             ; string limit checker
    je .loop1
    stosb
    inc cl
    call print
    
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
    inc bl
    .loopCompletar:
      cmp cl, bl ; se a string nao tiver 20 chars + enter
      jne .completa ; completa com " " para manter tamanho fixo

      ; coloca line break
      mov al, 13
      stosb
      mov al, 10
      stosb
      ret

      .completa:
        mov al, ' '
        inc cl
        stosb ; guarda na memoria
        jmp .loopCompletar

; =================================================================================== CÓDIGOS DE BUSCAR =====================================================================================
printBuscar:
  mov si, stDigiteConta
  call printString

  mov di, opcao
  call gets
ret


Buscar:
  mov di, opcao
  mov si, banco
  cmpsb
  jne .naoEOPrimeiro
  dec si
  mov di,si
  ret

  .naoEOPrimeiro:
    mov si, banco
    mov di, opcao
    add si, 39
    cmpsb
    jne .naoEOSegundo
    dec si
    mov di,si
    ret
  
  .naoEOSegundo:
    mov si, banco
    mov di, opcao
    add si, 78
    cmpsb
    jne .naoEOTerceiro
    dec si
    mov di,si
    ret

  .naoEOTerceiro:
    mov si, banco
    mov di, opcao
    add si, 117
    cmpsb
    jne .naoEOQuarto
    dec si
    mov di,si
    ret
  
  .naoEOQuarto:
    mov si, banco
    mov di, opcao
    add si, 156
    cmpsb
    jne .naoEOQuinto
    dec si
    mov di,si
    ret

  .naoEOQuinto:
    mov si, 0
    
ret


notFound:
  mov si, stNotFound
  call printString
ret
   

printConta:
  ;============= print "N_CONTA: nconta"=============
  push si
  mov si,nconta
  call printString
  pop si
  call printString
  call endl 

  ;=============== print "NOME: nome" ===============  
  inc si
  push si
  mov si,nome
  call printString
  pop si
  call printString
  call endl 

  ;================ print "CPF: cpf" ================
  inc si
  push si
  mov si,cpf
  call printString
  pop si
  call printString
  call endl 
ret

; =================================================================================== CÓDIGOS DE EDITAR =====================================================================================
printBuscarEdit:
  mov si, stDigiteContaEdit
  call printString

  mov di, opcao
  call getString

  call endl
ret


Editar: 

  call printBuscarEdit
  call Buscar
  cmp si, 0
  jne .achouContaEdit
          
  call notFound
  call endl
  ret


  .achouContaEdit:

    add di,4


    ; ====== escanear nome ======
    mov bx, NOME_LEN
    push si
    mov si,nome
    call printString ; printa "NOME:"
    pop si
    mov cx, 0 ; zera contador
    call getString
    call endl
    ; ====== escanear cpf ======
    mov bx, CPF_LEN
    push si
    mov si,cpf
    call printString ; printa "CPF:"
    mov cx, 0 ; zera contador
    pop si
    call getString;
    mov al, 0; fecha a string
    stosb ; guarda na memoria

    call endl

    ret

  
; =================================================================================== CÓDIGOS DE DELETAR ====================================================================================
printBuscarDel:
  mov si, stDigiteContaDel
  call printString

  mov di, opcao
  call getString

  call endl
ret


Deletar:
  call printBuscarDel
  call Buscar
  cmp si, 0
  jne .deleteConta
          
  call notFound
  call endl
  ret

  .deleteConta:
    mov al,0
    stosb
    add si,4
    xor cx,cx
    
    mov bx, NOME_LEN
    call .loopCompletar

    xor cx,cx
    mov bx, CPF_LEN

    .loopCompletar:
        cmp cl, bl ; se a string nao tiver 20 chars + enter
        jne .completa ; completa com " " para manter tamanho fixo

        ; coloca line break
        mov al, 13
        stosb
        mov al, 10
        stosb
        ret

        .completa:
          mov al, ' '
          inc cl
          stosb ; guarda na memoria
          jmp .loopCompletar

ret



; =================================================================================== CÓDIGO INTERFACE ======================================================================================
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

 ;========================================================================================= MENU ============================================================================================
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

        call endl 
        call printInterface
        call endl 

        mov di, opcao
        call gets ;escreve em quem di aponta

        mov si, opcao
        call stoi ;opcao em ax

        cmp ax, 1
        jne .naoCadastar
        call cadastrar
        jmp .principal

        .naoCadastar:
          cmp ax, 2
          jne .naoBuscar
          call printBuscar
          call Buscar
          cmp si, 0
          jne .achouConta
          
          call notFound
          call endl
          jmp .principal

          .achouConta:
            call printConta
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

fim: