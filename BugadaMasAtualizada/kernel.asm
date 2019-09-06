org 0x7e00
jmp 0x0000:start

data:

    banco times 70 db 0
    tam_banco db 0

    nome times 21 db 0
    cpf times 21 db 0
    numero times 21 db 0

    string times 21 db 0

    titulo db 'Banco de Dados', 0
    option db 'Digite:', 0
    option1 db '1 - Cadastrar nova conta', 0
    option2 db '2 - Buscar conta', 0

    pressione_tecla db 'Pressione qualquer tecla', 0
    continuar db 'para continuar', 0

    cadastro_titulo db 'Cadastrando nova conta', 0
    cadastro_nome db 'Nome:', 0
    cadastro_cpf db 'CPF: ', 0
    cadastro_id db 'ID:  ', 0
    cadastro_feito db 'Cadastro realizado', 0

    busca_titulo db 'Buscando conta', 0

    white equ 15
    black equ 0

    key_backspace equ 0x08
    key_enter equ 0x0d


start:
    call initVideo
    jmp menu


;================================
;======== MENU PRINCIPAL ========
;================================


tela_menu:
    
    call clean
    mov bl, white

    mov dl, 13
    mov dh, 6
    mov ah, 02h
    int 10h
    mov si, titulo
    call printString

    mov dl, 6
    mov dh, 13
    mov ah, 02h
    int 10h
    mov si, option
    call printString

    mov dl, 6
    mov dh, 17
    mov ah, 02h
    int 10h
    mov si, option1
    call printString

    mov dl, 6
    mov dh, 19
    mov ah, 02h
    int 10h
    mov si, option2
    call printString
ret


menu:

    call tela_menu

    detectOption:

        call read_char

        mov cl, '1'
        cmp cl, al
        je cadastro

        mov cl, '2'
        cmp cl, al
        je busca

        jmp detectOption

jmp menu


;================================
;=========== CADASTRO ===========
;================================

tela_card:

    mov dl, 4
    mov dh, 12
    mov ah, 02h
    int 10h
    mov si, cadastro_nome
    call printString

    mov dl, 4
    mov dh, 14
    mov ah, 02h
    int 10h
    mov si, cadastro_cpf
    call printString

    mov dl, 4
    mov dh, 16
    mov ah, 02h
    int 10h
    mov si, cadastro_id
    call printString
ret

tela_cadastro:
    
    call clean

    mov dl, 8
    mov dh, 4
    mov ah, 02h
    int 10h
    mov si, cadastro_titulo
    call printString

    call tela_card
ret

tela_cadastro_concluido:

    call clean

    mov dl, 10
    mov dh, 6
    mov ah, 02h
    int 10h
    mov si, cadastro_feito
    call printString

    mov dl, 7
    mov dh, 17
    mov ah, 02h
    int 10h
    mov si, pressione_tecla
    call printString

    mov dl, 12
    mov dh, 19
    mov ah, 02h
    int 10h
    mov si, continuar
    call printString
ret 


registrar:

    call pointToNew
	mov si, nome
	call strcpy

	call toCPF
	mov si, cpf
	call strcpy

	call toID
	mov si, numero
	call strcpy

	call incTam
ret


cadastro:

    push cx
    xor cx, cx
    call tela_cadastro

    mov di, nome
    mov dl, 10
    mov dh, 12
    mov ah, 02h
    int 10h
    call read_and_print

    mov di, cpf
    mov dl, 10
    mov dh, 14
    mov ah, 02h
    int 10h
    call read_and_print

    mov di, numero
    mov dl, 10
    mov dh, 16
    mov ah, 02h
    int 10h
    call read_and_print

    call registrar

    call tela_cadastro_concluido
    call read_char

    call clean
    mov si, nome
    mov dl, 10
    mov dh, 12
    mov ah, 02h
    int 10h
    call printString

    mov si, banco
    mov dl, 10
    mov dh, 16
    mov ah, 02h
    int 10h
    call printString

    call read_char

    pop cx
jmp menu


;================================
;======= BUSCA DE USUÁRIO =======
;================================

busca:

    call clean

    mov dl, 4
    mov dh, 8
    mov ah, 02h
    int 10h
    mov si, busca_titulo
    call printString

    call read_char
    call printBanco
jmp menu


;================================
;===== FUNÇÕES AUX P/ BANCO =====
;================================

pointToNew:
    mov di, banco
    call incTam
    ret

    ;mov cl, 0
    ;push cx
   ;.point_new_loop:
    ;    cmp cl, 0
     ;   je .point_new_done
      ;  dec cl
       ; add di, 64
    ;.point_new_done:
    ;pop cx
    ;call incTam
;ret

pointToKey:
    push cx
    mov di, banco
    .point_loop:
        cmp cl, 0
        je .point_done
        dec cl
        add di, 64
    .point_done:
    pop cx
ret

incAmount:
    .inc_loop:
        cmp cl, 0
        je .inc_done
        dec cl
        inc di
    .inc_done:
ret

; di aponta p/ nome do usuario[cl]
toName:
    push cx
    call pointToKey
    pop cx
ret

; di aponta p/ CPF do usuario[cl]
toCPF:
    push cx
    call pointToKey
    mov cl, 22
    call incAmount
    pop cx
ret

; di aponta p/ ID do usuario[cl]
toID:
    push cx
    call pointToKey
    mov cl, 43
    call incAmount
    pop cx
ret

incTam:
    push ax
    mov ax, [tam_banco]
    inc ax
    mov [tam_banco], ax
    pop ax
ret

decTam:
    push ax
    mov ax, [tam_banco]
    dec ax
    mov [tam_banco], ax
    pop ax
ret

printUser:
    
    call clean
    call tela_card

    mov dl, 10
    mov dh, 12
    mov ah, 02h
    int 10h

    ;call toName
    mov si, banco
    call printString

    ret

    mov dl, 10
    mov dh, 12
    mov ah, 02h
    int 10h
    
    ;call toCPF
    mov si, di
    call printString

    mov dl, 10
    mov dh, 12
    mov ah, 02h
    int 10h
    
    ;call toID
    mov si, di
    call printString
ret

printBanco:

    pusha
    xor cx, cx

    .pb_loop:

        push ax
        mov ax, [tam_banco]
        cmp cl, al
        je .pb_done

        pop ax
        call printUser

        call read_char
        inc cx

        jmp .pb_loop

    .pb_done:
    pop ax
    popa
ret


;================================
;====== FUNÇÕES AUXILIARES ======
;================================

initVideo:
    mov al, 13h
    mov ah, 0
    int 10h
ret

print_char:
    mov ah, 0eh
    int 10h
ret

read_char:
    mov ah, 0
    int 16h
ret

new_line:
    mov al, 10
    call print_char
    mov al, 13
    call print_char
ret

del_char:
    mov al, key_backspace
    call print_char
    mov al, ' '
    call print_char
    mov al, key_backspace
    call print_char
ret

printString:
    lodsb
    mov cl, 0
    cmp cl, al
    je .done
    mov ah, 0xe
    int 0x10
    jmp printString
    .done:
ret

read_and_print:
    push cx
    xor cx, cx
    .loop_rp:
        call read_char
        cmp al, key_backspace
        je .rp_backspace
        cmp al, key_enter
        je .rp_done
        cmp cl, 20
        je .loop_rp

        mov byte[di], al
        inc di

        inc cl
        call print_char
        jmp .loop_rp

        .rp_backspace:
            cmp cl, 0
            je .loop_rp

            dec di
            dec cl
            mov byte[di], 0
            call del_char
            jmp .loop_rp

    .rp_done:
    cmp cl, 0
    je .loop_rp

    mov al, 0
    mov byte[di], al
    pop cx
ret

strcmp:
    .loop_cmp:
        lodsb
        cmp al, byte[di]
        jne .notequal
        cmp al, 0
        je .equal
        inc di
        jmp .loop_cmp
    .equal:
        mov cl, 0
ret
    .notequal:
        mov cl, 1
ret

strcpy:
    .loop_cpy:
        mov al, byte[si]
        inc si
        mov byte[di], al
        inc di
        cmp al, 0
        je .end_cpy
        jmp .loop_cpy
    .end_cpy:
ret

print_pixel:
    mov ah, 0Ch
    int 10h
ret

clean:
    push ax
    mov al, black
    push dx
    push cx
    mov dx, 0
    .ForLA:
        cmp dx, 200
        je .endForLA
        mov cx, 0
        .forinLA:
            cmp cx, 320
            je .endForinLA
            call print_pixel
            inc cx
            jmp .forinLA
        .endForinLA:
        inc dx
        jmp .ForLA
    .endForLA:
    pop cx
    pop dx
    pop ax
ret
