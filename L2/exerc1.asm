org 0x7c00
jmp 0x0000:main


data: 
    stBoaVinda db 'AP2 de IHS', 0


; Função que imprime uma string que está em bx de tamanho armazenado em cx
printString: 
    loop:
        mov al,[bx]    ; coloca em al o que deve ser impresso
        add bx,1       ; anda na string
        dec cx         ; decrementa o contador (que está armazenando o tamanho da string)

        cmp cx, 0      ; ve se o apontador chegou no final da string (contador == 0)
        je endLoop
        
        ; printar o que esta em al
        mov ah, 0eh 
        int 10h 
        jmp loop

        endLoop:
            iret        


main:
    ; Empilha os registradores a serem usados na ISR (guarda o contexto)
    pusha

    ; Codigo de tratamento
    mov bx,stBoaVinda          ; endereço da string
    mov cx,11                  ; tamanho da string (com \0 do final)
    mov ax,0                   ; zerando AX
    mov ds,ax                  ; zerando DS
    mov di,100h                ; offset da interrupção 40h na IVT
    mov word[di],printString   ; salvando IP
    mov word[di+2],0           ; salvando CS

    int 40h

    ; Desempilha os registradores (retoma o contexto)
    popa
    

times 510-($-$$) db 0
dw 0xaa55


