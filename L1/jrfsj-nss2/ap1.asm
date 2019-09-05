org 0x7c00        ;endereço de memória em que o programa será carregado
jmp 0x0000:start  ;far jump - seta cs para 0

; tamanhos
;   nome 21
;   cpf 12
;   n_conta 2 

banco times 176 db 0 ; user = 35, n_user = 5, 35 * 5 = 175 (+ 1 do \0)
nome times 21 db 0; nome
cpf times 12 db 0 ; cpf
nconta times 2 db 0 ;
newuser times 40 db 0

NOME_LEN equ 21
CPF_LEN equ 12
N_CONTA_LEN equ 2
; actual_len times 1 db

start:
	xor ax, ax	; zerar ax (xor com ele mesmo sempre dá 0)
	mov ds, ax
	mov es, ax
    mov bx, ax  ; bx assume valores do len da string para scanear
	mov ss, ax
	mov sp, 0x7c00 ;carre

	push ax ; coloca o ax (que é 0) na pilha
	; jmp scan
	

    call cadastrar
    
    ; call cadastrar
    ; call printString
    ; call lineBreak

    jmp fim

cadastrar:
    ; ====== escanear nome
    mov bx, NOME_LEN
	mov di, nome 	; para escanear (stosb)
    mov cx, 0 ; zera contador
    call getString;
    mov si, nome ; aponta o endereço para o começo
    call lineBreak ; quebra de linha
    
    call printString


    ; ====== escanear cpf
    mov bx, CPF_LEN
	mov di, cpf 	; para escanear (stosb)
    mov cx, 0 ; zera contador
    call getString;
    mov si, cpf ; aponta o endereço para o começo
	call lineBreak ; quebra de linha

    call printString


    ; ====== escanear num conta
    mov bx, N_CONTA_LEN
	mov di, nconta 	; para escanear (stosb)
    mov cx, 0 ; zera contador
    call getString;
    mov si, nconta ; aponta o endereço para o começo
	call lineBreak ; quebra de linha

    call printString


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
        jne .completar
		mov al, 0; fecha a string
		stosb ; guarda na memoria
        ; call lineBreak
        
        ret


        .completar:
            mov al, ' '
            inc cx
            stosb ; guarda na memoria
            jmp .done

        ; .fimGetString:
        ;     ret
		
printString:
	lodsb
	call print
	cmp al, 0
	je .done
	jmp printString
	.done:
        call lineBreak ; quebra de linha
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