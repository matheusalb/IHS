
getCpf:
    mov di, cpf 	; para escanear (stosb)
	; escaneia e printa
	call scan
	call print

	; apertou enter?
	cmp al, 0x0d
	je .done

	; guarda na memoria
	stosb

	; chamada recursiva
	jmp getCpf

	; fim da função
	.done:
		mov al, 0 ; fecha a string
		stosb ; guarda na memoria
		mov si, cpf ; aponta o endereço para o começo
		call lineBreak ; quebra de linha

		ret

printCpf:
	lodsb
	call print
	cmp al, 0
	je .done
	call printCpf
	.done:
		ret