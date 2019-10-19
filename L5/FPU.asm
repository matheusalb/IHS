extern printf
section .data
n: dq 500.0 ; Numero de iterações
i: dq 0.0 ; variável iteratiba
minusone: dq -1.0 ; -1, será bastante útil
CurrentOp: dq 1.0 ; Operação atual, determina se irá somar ou subtrair
str: db 'Valor = %.12f', 10,0       ;string para printf
m1 dq 1.0 ; 1
m2 dq 2.0 ; 2
resultado dq 0.0 ; resultado final
termo dq 0.0 ; o termo
x dq 1.57
xsq dq 0.0
xpot dq 0.0
fat dq 1.0
seq_i dq 0.0
divisor dq -1.0
section .text
global main

main:


fld qword[n] ; carrego o n
fld qword[m1] ; carrego o 1
faddp ; st0 = n+1
fstp qword[n] ; n := st0
fstp

fld qword[x]
fstp qword[xpot] ;;Coloca x em xpot


fld qword[x]
fld qword[x]
fmulp ; x*x
fstp qword[xsq] ; Faz xsq = x*x
fstp


loop1:

    fld qword[i] 
    fld qword[m2] 
    fmulp ; st0 = 2*i
    fstp qword[divisor] ; divisor = st0
    fstp

    




    fld qword[divisor]
    fld qword[m1]
    faddp st1,st0   ; faz st0 = st0 +st1 com sto+st1 ==   2*i +1 e st1 
    fstp qword[divisor] ; divisor = st0
    fstp

    

    fld qword[divisor]
    fld qword[fat]
    fmulp ; st0 = fat*divisor
    fstp qword[divisor] ;divisor = st0
    fstp

    fld qword[divisor]
    fstp qword[fat] ; fat = divisor



    fld qword[seq_i]
    fld qword[m2]
    faddp ; st0 = i+1
    fstp qword[seq_i] ;seq_i = st0
    fstp

    
    
    fld qword[seq_i]
    fld qword[fat]
    fmulp ; st0 = fat*(i+1) 
    fstp qword[fat] ; fat = st0
    fstp 

    

    fld qword[divisor]
    fld qword[m1]
    fdivrp ;temos st0 = st0/st1 com st0/st1 == 1 / (i*2 + 1)!]
    fstp qword[divisor] ; divisor = st0
    fstp



        

    fld qword[divisor]
    fld qword[CurrentOp]
    fmulp;      temos st0 = st0*st1 com st0*st1 == (-1)^i * [1/(2*i +1)!]
    fstp qword[termo] ;termo = st0
    fstp

    



    fld qword[CurrentOp]
    fld qword[minusone]
    fmulp ; temos st0 = st0*st1 com st0* st1 == (-1)^(i+1) == (-1)*(-1)^i
    fstp qword[CurrentOp]  ;CurrentOp = st0

    fld qword[xpot]
    fld qword[termo] 
    fmulp ;st0 = st0*st1 com st0*st1 == xpot * (-1)^i * [1/(2*i +1)!]
    fstp qword[termo] ; termo = st0
    fstp






    fld qword[termo]
    fld qword[resultado]
    faddp  ;st0 = st0 +st1 com st0+st1 = termo+resultado
    fstp qword[resultado]  ;Soma o termo com o resultado
    fstp




    fld qword[i]
    fld qword[m1]  
    faddp   ;adiciono em 1 o i
    fstp qword[i]
    fstp


    fld qword[xpot]
    fld qword[xsq] 
    fmulp  ; st0 = xpot * x^2
    fstp qword[xpot] ; xpot = st0
    fstp


    fld qword[i]
    fld qword[n]  ; comparo i com n
    fcomip ; e coloco no EFLAGS

    je exit1

jmp loop1

exit1:


    push dword [resultado+4]            ;empilhando o valor double
    push dword [resultado]              ;em duas partes de 32 bits
    push dword str                  ;empilhando endereco da string
    call printf
    add esp, 12

    fld qword[resultado] ; carrego o resultado na FPU



    jmp FIM







FIM:
MOV EAX, 1 ; exit syscall
MOV EBX, 0 ; program return
INT 80H ; syscall interruption