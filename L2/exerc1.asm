org 0x7c00
jmp 0x0000:main


data: 

    stBoaVinda db 'Monitoria de IHS', 0

printString: ;printa a string apontada por si

    inc ax ; ++tamanho da string
    mov si,bx ; faz si apontar para o comeco da string
    
    mov cx, 0 ; cx como contador
    mov bx, ax ; bx o topo do contador (tamanho da string)

    loop:
        mov al,[si]
        add si,1
        inc cx

        cmp cx, bx ; ve se o contador chegou no tamanho da string
        je endLoop
        
        ; printar o que esta em al
        mov ah, 0eh 
        int 10h 
        jmp loop

        endLoop:
            iret



main:

    mov bx,stBoaVinda
    mov ax,7



    mov di,100h     
    mov word[di],printString
    mov word[di+2],0




    int 40h





times 510-($-$$) db 0
dw 0xaa55


