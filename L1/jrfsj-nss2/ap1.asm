org 0x7c00        ;endereço de memória em que o programa será carregado
jmp 0x0000:start  ;far jump - seta cs para 0


; CALCULO BANCO:
; numero de usuarios: n_user = 5
;
; fechar string = \0 (tamanho 1)
; linebreak = 2
;
; n_conta = 1 + linebreak = 3
; nome = 20 + linebreak = 22
; cpf = 11 + linebreak + \0 = 14
;
; tamanho do usuario: user_len = n_conta + nome + cpf = 3 + 22 + 14 = 39
; banco = user_len * n_user = 39 * 5 = 195 
;

banco times 195 db 0 ; calculo no comentario de cima

nome times 7 db "NOME:",0 
cpf times 6  db "CPF:",0 
nconta times 9 db "N_CONTA:",0

NOME_LEN equ 21
CPF_LEN equ 12
N_CONTA_LEN equ 2


start:
	xor ax, ax	; zerar ax (xor com ele mesmo sempre dá 0)
	mov ds, ax	; zerar o ds
	mov es, ax	; zerar o es
    mov bx, ax  ; bx assume valores do len da string para scanear
	mov ss, ax	; zerar o ss
	mov sp, 0x7c00 ; carre

	push ax ; coloca o ax (que é 0) na pilha
	
	mov di, banco 	; para escanear (stosb)
    call cadastrar

    jmp fim


cadastrar:
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
    ret


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


printString:
	lodsb
	call print
	cmp al, 0
	je .done
	jmp printString
	.done:
		;call lineBreak
		ret


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


fim:

times 510 - ($ - $$) db 0
dw 0xaa55       ;assinatura de boot