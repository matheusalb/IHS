Fixado o bug de leitura no modo kernel (stosb por mov byte[di], al + inc di).
Ainda tem o bug de copiar uma string para outro local. (função strcpy, funciona no boot1, copiada do git de Beza).
Precisa ajeitar esse antes de prosseguir.
