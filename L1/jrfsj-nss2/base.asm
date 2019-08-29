org 0x7c00        ;endereço de memória em que o programa será carregado
jmp 0x0000:start  ;far jump - seta cs para 0

start:
	xor ax, ax	; zera ax para escanear
	mov ds, ax 	; zera ds (contem si (lodsb) e di (stosb))
	mov es, ax 	; zera o iterador (si, source index)
	mov bx, ax
	mov cx, ax

	mov di, string 	; para escanear (stosb)


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
		mov al, 0 ; fecha a string
		stosb ; guarda na memoria
		mov si, string ; aponta o endereço para o começo
		call lineBreak ; quebra de linha

		ret

scan:
	mov ah, 0
	int 16h

	ret

print:
	mov ah, 0xe	; chando o sistema de saída
	int 10h		; printando oq estiver em al

	ret

lineBreak:
	mov ax, 13 ; cria uma nova linha
	call print	

	mov ax, 10 ; coloca
	call print

	ret

debug:
	mov al, 'd'
	call print

	ret

delay:
	push cx
	push dx
	push ax
    MOV     CX, 0FH
    MOV     DX, 4240H
    MOV     AH, 86H
    INT         15H
    pop ax
    pop dx
    pop cx
    ret

printString:
	lodsb
	call print
	cmp al, 0
	je .done
	call printString
	.done:
		ret

end:

times 510 - ($ - $$) db 0
dw 0xaa55       ;assinatura de boot