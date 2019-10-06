extern printf
section .data
n: dq 10000000.0 ; Numero de iterações
i: dq 0.0 ; variável iteratiba
minusone: dq -1.0 ; -1, será bastante útil
CurrentOp: dq 1.0 ; Operação atual, determina se irá somar ou subtrair
str: db 'Valor = %.12f', 10,0		;string para printf
m1 dq 1.0 ; 1
m2 dq 2.0 ; 2
resultado dq 0.0 ; resultado final
termo dq 0.0 ; o termo

section .text
global main

main:


fld qword[n] ; carrego o n
fld qword[m1] ; carrego o 1
faddp 
fstp qword[n] ; carrego o resultado para ser n + 1


loop1:

    fld qword[m2] ; pega o 2
    fld qword[i]
    fmulp st1,st0 ; st1+st0 ==    i * 2




    fld qword[m1]
    faddp st1,st0   ;  sto+st1 ==   2*i +1 





    fld qword[m1]
    fdivrp ; st0/st1 == 1 / i*2 + 1


    	


    fld qword[CurrentOp]
    fmulp          ; st0*st1 == (-1)^i * [1/(2*i +1)]
    fstp qword[termo]
    fstp

    



    fld qword[CurrentOp]
    fld qword[minusone]
    fmulp
    fstp qword[CurrentOp]  ; (-1)^(i+1) == (-1)*(-1)^i
    fstp


    fld qword[termo]
    fld qword[resultado]
    faddp 
    fstp qword[resultado]  ;Soma o termo com o resultado
    fstp





    fld qword[i]
    fld qword[m1]  ;adiciono em 1 o i
    faddp
    fstp qword[i]
    fstp



    fld qword[i]
    fld qword[n]  ; comparo i com n
    fcomip
    je exit1

jmp loop1

exit1:

push dword [resultado+4]			;empilhando o valor double
push dword [resultado]				;em duas partes de 32 bits
push dword str 					;empilhando endereco da string
call printf
add esp, 12







FIM:
MOV EAX, 1 ; exit syscall
MOV EBX, 0 ; program return
INT 80H ; syscall interruption
