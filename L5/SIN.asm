section .data
 
    str: db 'Valor = %.12f', 10,0   ;string para printf
 
    ; Auxiliar Debug
    varPrint dq 0.0
 
    ; Parâmetros
    x dq 90.0
    erro dq 0.000002
 
    ; Auxiliar comparador
    resultadoReal dq 0.0
    resultadoCalc dq 0.0
 
    ; Auxiliares conversão
    paraRad dq 180.0
    tratOver dq 360.0
 
    ; Cálculo dos termos
    fatorMult dq 1.0
    fatorNum dq 0.0
    indice dw 1
 
 
section .text
global Sin
 
 
 
Sin:

    enter 0,0 ; Reajusta, salva ebp na pilha
    finit

    fld qword[ebp+8]
    fstp qword[x] ;Coloca da pilha para o x

    fld qword[ebp+12]
    fstp qword[erro] ;Coloca da pilha para o erro




 
    xor eax, eax ; Zera o ax, será usado como contador
    inc ax ; Iteração Inicial

    
 
 
    ; TRATAMENTO OVERFLOW: Ângulo truncado para 360
    fld qword[tratOver] ; st0 := 360
    fld qword[x] ; st0 := parâmetro
    fprem ; st0 := parâmetro%360.0
 
 
    ; CONVERSÃO PARA RADIANOS: Multiplicar por 2pi/360
    fldpi ; st0 := pi
    fmul ; st0 := pi*st1
    fdiv qword[paraRad] ; st0 := pi/180 (Ângulo em Rad)
    ; Parâmetro em radianos na ST(0)
 
 
    ; INICIALIZAÇÃO PARÂMETROS: fatorMult e resultadoCalc
    fst qword[fatorMult]        ; Inicialização fator multiplicativo
    fst qword[resultadoCalc]    ; Iteração inicial
    fst qword[x]                ; Armazena x já convertido para radianos
 
 
    ; ENCONTRAR VALOR FUNÇÃO
    fsin                        ; Calcula sin(x)
    fstp qword[resultadoReal]   ; Coloca o seno em resultadoReal
 
 
    ; INICIALIZAÇÃO PARÂMETROS 2: fatorNum
    fld qword[x]                ; Carrega angulo
    fmul qword[x]               ; st0 := angulo*angulo
    fchs                        ; st0 := -(angulo*angulo)
    fstp qword[fatorNum]
 
 
    mov eax,0 ; Inicializa eax como 0

loopIter:

    inc eax ;incrementa eax

   
    ; CÁLCULO DO ERRO ATUAL
    fld qword[resultadoReal]
    fld qword[resultadoCalc]
    fsubp
    fabs
 
    ; COMPARAÇÃO COM O ERRO SOLICITADO
    fld qword[erro]
    fcomip
    fstp ; RETIRA VALORES COMPARADOS DA PILHA
    fstp ; RETIRA VALORES COMPARADOS DA PILHA
 
 
    jae exit1 ; CONDIÇÃO DE SAÍDA
    inc eax ; incremento qt iterações
 
    ; CÁLCULO FATOR MULTIPLICATIVO
    fld qword[fatorMult]
    fld qword[fatorNum]
    fmulp               ; st0 := fatorMult*fatorNum
    fild qword[indice]  ; st1 := indice, st0 := 1
    fld1
    faddp               ; st0 := indice+1
    fist dword[indice]  ; indice := st0
    fdivp               ; st1 = st1/st0 + pop st0
    fild qword[indice]  ; st1 := indice, st0 := 1
    fld1               
    faddp               ; st0 := indice+1
    fist dword[indice]  ; indice := st0
    fdivp               ; st1 = st1/st0 + pop st0
    fst qword[fatorMult]    ; guardar valor de fatorMult
 
 
    ; SOMA DO FATOR MULT AO RESULTADO
    fadd qword[resultadoCalc]   ; st0 += resultadoCalc
    fstp qword[resultadoCalc]   ; resultadoCalc := st0
 
 
    ; NOVA ITERAÇÃO
    jmp loopIter
 
 
exit1:

    mov ecx,[ebp +12]
    fst qword[ecx]; Armazena em x o valor certo
 

    leave ;Desfaz o que o enter faz
    ret
 
