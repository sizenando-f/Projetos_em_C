#	@file: main.asm
#	Implementa��o do projeto usando Digital Lab Sim.
#	Inser��o em pilha, c�lculo de m�dia aritm�tica, desvio padr�o, sequ�ncia de van eck e fibbonacci
#
#	@authors: Sizenando S. Fran�a (50575), Alexandre... (-----)
#	@date: 27-10-2025

# Defini��o de constantes para facilitar implementa��o e evitar confus�o
.data
	# Endere�os do Digital Lab Sim de acordo com o manual
	VISOR_D:			.word 0xFFFF0010	# Diplay direito
	VISOR_E:			.word 0xFFFF0011	# Display esquerdo
	CHAVE_CONTROLE:		.word 0xFFFF0012	# Para o controle do teclado na escrita
	CONTADOR_CONTOROLE:	.word 0xFFFF0013	# Para controle do contador
	CHAVE_DADO:			.word 0xFFFF0014	# Para ler o teclado
	
	# Tabela pra usar de consulta dos 7 segmentos, de 0 ao 9, E e o ponto
	MAPA_SEGMENTO:	.byte	0x3F,	# 0: 0111111
							0X06,	# 1: 0000110
							0x5B, 	# 2: 1011011
							0x4F, 	# 3: 1001111
                       		0x66, 	# 4: 1100110
                      		0x6D, 	# 5: 1101101
                       		0x7D, 	# 6: 1111101
                       		0x07, 	# 7: 0000111
                       		0x7F, 	# 8: 1111111
                       		0x6F, 	# 9: 1101111
                       		0x79, 	# E: 1111001 -> Para o erro EE quando o valor passar de 3 digitos
	PONTO:			.byte	0x80	# Pra ligar o ponto   
	
	PILHA_ARRAY:	.space 40		# Aloca 40 bytes (10 elementos)
	PILHA_TOPO:		.word 0			# Contador da pilha
	
	# Para decodificar as teclas baseado nos 16 c�digos do help
					# 1, 0, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	CODIGOS_TECLAS:	.byte 0x11, 0x21, 0x41, 0x81, 0x12, 0x22, 0x42, 0x82, 0x14, 0x24, 0x44, 0x84, 0x18, 0x28, 0x48, 0x88
	VALORES_TECLAS:	.byte 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15
	
# In�cio do que � execut�vel
.text
# Indica o ponto de entrada do programa para o SO, nesse caso, o simulador
.globl main

# Para inicializa��o de "vari�veis"
main:
	# Carrega o endere�o de CHAVE_CONTROLE para ($t0)
	lui $t0, 0xFFFF			# Primeira metade de bits
	ori $t0, $t0, 0x0012	# Segunda metade de bits
		
	# Carrega o endere�o de CHAVE_DADO para ($t1)
	lui $t1, 0xFFFF			# Primeira metade de bits
	ori $t1, $t1, 0x0014	# Segunda mentade de bits
		
	# Para salvar a linha que est� sendo varrida
	addiu $s0, $zero, 1		# ($s0) = 1 -> primeira linha
	
	# Numero atual do display
	addiu $s3, $zero, 0

# Para ficar varrendo o teclado
main_loop:
	# Ativa a linha atual ao escrever $s0 em $t0 (controle do teclado)
	sb $s0, 0($t0)						# Grava palavra de $s1 -> $t0
		
	# L� o resultado do teclado
	lb $t2, 0($t1)						# Recebe palavra de $t1 -> $t2
		
	# Verifica se alguma tecla foi pressionada
	bne $t2, $zero, tecla_pressionada	# $t2 != 0 ? tecla_pressioanda
		
	# Para caso nenhuma foi pressionada, avan�a para pr�xima linha
	sll $s0, $s0, 1						# Multiplica $s0 por 2, ou seja, "sobe" um bit na sequ�ncia de bits
		
	# Verifica se j� passou da �ltima linha
	# 16 significa que as 4 linhas j� foram
	sltiu $t3, $s0, 16					# $t3 = 1 se $s0 < 16 (n chegou ao fim), caso contr�rio $t3 = 0 (chegou ao fim)
	bne $t3, $zero, main_loop			# Se for menor que 16 ent�o continua loop
	addiu $s0,$zero,1					# Sen�o reseta $s0 para 1
		
	j main_loop							# Volta pro inicio pra varrer pr�xima linha

# C�digo est� em $t2
tecla_pressionada:

# Para esperar o usu�rio soltar a tecla
aguarda_soltar_tecla:
	lb $t3, 0($t1)							# L� o teclado novamente
	bne $t3, $zero, aguarda_soltar_tecla	# Se ainda n�o for zero, espera mais
	
	# Tecla foi solta, ent�o processa o que esta�em $t2
	# $s1 -> indice de leitura para CODIGOS_TECLAS
	addiu $s1, $zero, 0						# Inicia $s1 com 0

# Para buscar o c�digo
busca_tecla_loop:
	# Se o indice chegar ao final, ent�o n�o foi encontrado
	sltiu $t3, $s1, 16			# $t3 = 1 se $s1 < 16, caso contr�rio $t3 = 0
	beq $t3, $zero, fim_busca	# Se $s1 >= 16 ent�o ele sai do loop
	
	# Carrega o endere�o base de CODIGOS_TECLAS em $t4
	lui $t4, 0x1001				# Carrega a parte alta do endere�o
	ori $t4, $t4, 0x004C		# Carrega a parte baixa
	
	# Para o �ndice para encontrar o endere�o do elemento
	addu $t4, $t4, $s1
	
	# Carrega o byte no endere�o calculado
	lb $t3, 0($t4)
	
	# Compara o c�digo da tabela ($t3) com o c�digo da tecla pressionado ($t2)
	beq $t3, $t2, tecla_encontrada	# Se for iguais, foi encontrado
	
	# Se n�o encontrou ent�o avan�a o �ndice
	addiu $s1, $s1, 1				# $s1++     
	
	j busca_tecla_loop 				# Reinicia loop		

# $s1 possui o �ndice da tecla
# Para quando encontrar tecla
tecla_encontrada:
	# Carrega o endere�o base de VALORES_TECLAS em $t4
	lui $t4, 0x1001			# Carrega a parte alta do endere�o
	ori $t4, $t4, 0x005C	# Carrega a parte baixa
	
	# Para o �ndice para encontrar o valor correspondente
	addu $t4, $t4, $s1
	
	# Carrega o byte do valor real
	lb $s2, 0($t4)
	
	# VAi para o bloco certo
	sltiu $t3, $s2, 10					# $t3 = 1 se $s2 < 10, quer dizer que � um n�mero
	bne $t3, $zero, processa_numero		# Se for n�mero
	
	# Se n�o, � uma fun��o
	j processa_funcao
	
fim_busca:
	# Se n�o foi encontrado ou tudo j� foi processado, volta ao loop principal
	j main_loop

# Valor do digito pressionado -> $s2
# N�mero atual do visor -> $s3
# numero_atual = (numero atual * 10) + novo digito	
processa_numero:
	# Verifica se o n�mero passou de 3 digitos
	sltiu $t3, $s3, 10			# $t3 = 1 se $s3 < 10, se for 0 ent�o $s3 j� tem 2 digitos
	beq $t3, $zero, overflow	# se $t3 >= 10 ent�o n�o adiciona digito
	
	# Multiplica o n�mero atual por 10
	addiu $t3, $zero, 10	# Prepara multiplicando
	multu $s3, $t3			# numero atual * 10
	mflo $s3				# $s3 <- resultado
	
	# Adiciona o novo d�gito
	addu $s3, $s3, $s2		# $s3 <- ($s3 * 10) + $s2
	
	# Atualiza o visor
	add $a0, $s3, $zero		# Coloca o n�mero a ser exibido para ser passado como par�metro
	jal atualiza_visor

	j fim_busca
	
overflow:
	j fim_busca
	
#-------------------------
# @brief Escolhe fun��o
#
# Escolhe para qual fun��o pular de acordo com a tecla pressionada
#-------------------------
# Valor da fun��o -> $s2
processa_funcao:
	# Para tecla 'a' (10 em hexa)
	addiu $t3, $zero, 10
	beq $s2, $t3, funcao_a
	
	# Para a tecla 'b'
	addiu $t3, $zero, 11
	beq $s2, $t3, funcao_b
	
	# Para a tecla 'e'
	addiu $t3, $zero, 14
	beq $s2, $t3, funcao_e
	
	# Para tecla 'f'
	addiu $t3, $zero, 15
	beq $s2, $t3, funcao_f
	
	# Se n�o for nenhuma das fun��es
	j fim_busca

#----------------------------------------
# @brief Empilha o n�mero
#
# Empilha o valor apresentado no visor no vetor
#----------------------------------------
funcao_a:
	lui $t4, 0x1001
	ori $t4, $t4, 0x0048		# Endere�o do topo da pilha (0x10010020 + 40)
	lw $t5, 0($t4)				# $t5 <- valor do topo da pilha
	
	# Verifica se pilha est� cheia
	addiu $t6, $zero, 10
	slt $t6, $t5, $t6			# $t6 <- 1 se topo < 10, sen�o 0
	beq $t6, $zero, fim_busca	# Se topo >= 10 ent�o a pilha est� cheia
	
	# Calcula endere�o de destino -> base + (topo * 4)
	lui $t6, 0x1001
	ori $t6, $t6, 0x0020		# Endere�o base da PILHA_ARRAY
	sll $t7, $t5, 2				# $t7 <- topo * 4
	addu $t6, $t6, $t7			# $t6 <- endere�o de destino
	
	# Armazena n�mero atual na ppilha
	sw $s3, 0($t6)
	
	# Incrementa o topo da pilha
	addiu $t5, $t5, 1	# Incrementa o valor em $t5
	sw $t5, 0($t4)		# Salva o novo valor
	
	# Reseta o n�mero atual e atualiza o visor para 00
	addiu $s3, $zero, 0
	add $a0, $s3, $zero
	jal atualiza_visor
	
	j fim_busca

#----------------------------------------
# @brief Calcula a m�dia aritm�tica
#
# Realiza o c�lculo da m�dia aritm�tica de todos valores na pilha
#----------------------------------------
funcao_b:
	# Carrega o endere�o e valor da PILHA_TOPO
	lui $t4, 0x1001
	ori $t4, $t4, 0x0048	# Endere�o da PILHA_TOPO
	lw $s4, 0($t4)			# ($s4) <- n�mero de elementos na pilha
	
	# Verifica se a pilha est� vazia
	bne $s4, $zero, calcular_soma
	addiu $a0, $zero, 0
	jal atualiza_visor
	j fim_busca

# Inicializa vari�veis para o loop de soma
calcular_soma:
	addiu $s5, $zero, 0		# $s5 <- soma total
	addiu $t5, $zero, 0		# $t5 <- �ndice do loop
	lui $t6, 0x1001			
	ori $t6, $t6, 0x0020	# $t6 <- endere�o base do PILHA_ARRAY

# Realiza a soma dos elementos da pilha
soma_loop:
	# Verifica se i >= n
	slt $t3, $t5, $s4				# $t3 = 1 se i < N
	beq $t3, $zero, fim_soma_loop
	
	# Calcula o endere�o do elemento atual. base + (i * 4)
	sll $t7, $t5, 2		# $t7 <- i * 4
	addu $t7, $t6, $t7	# $t7 <- endere�o do elemento
	
	# Carrega elemento e adiciona na soma
	lw $t8, 0($t7)		# $t8 <- pilha[i]
	addu $s5, $s5, $t8	# soma total <- soma total + pilha[i]
	
	# Incrementa o �ndice
	addiu $t5, $t5, 1	# i++
	j soma_loop
	
# $s5 <- soma | $s4 <- N
fim_soma_loop:
	# Verifica se a m�dia � exata
	divu $s5, $s4
	mflo $t3			# $t3 <- soma / N
	mfhi $t4			# $t4 < soma % N
	
	# Verifica se o resto � zero
	bne $t4, $zero, media_nao_inteira

# Para lidar com m�dias inteiras
media_inteira:
	# Verifica se cabe no visor
	addiu $t5, $zero, 100
	slt $t5, $t3, $t5			# $t5 = 1 se parte inteira < 100
	beq $t5, $zero, exibe_erro	# se for maior ou igual a 100, d� erro
	
	# Mostra o resultado no visor
	add $a0, $t3, $zero
	jal atualiza_visor
	j fim_busca

media_nao_inteira:
	# Verifica se parte inteira possui apenas um d�gito
	addiu $t5, $zero, 10
	slt $t5, $t3, $t5			# $t5 = 1 se parte inteira < 10
	beq $t5, $zero, exibe_erro	# Sen�o da erro
	
	# (soma * 10 + N / 2) / N
	# Multiplica a soma por 10
	addiu $t5, $zero, 10	# Multiplicador
	multu $t4, $t5			
	mflo $t4				# $t4 <- soma * 10

	# Calcula o arredondamento
	srl $t5, $s4, 1			# $t5 <- N/2, desloca bit
	
	# Soma os dois termos calculados
	addu $t4, $t4, $t5		# $t4 <- (soma * 10 + N / 2)
			
	divu $t4, $s4			# Divide pelo n�mero de elementos
	mflo $t4				# $t4 <- digito decimal
	
	# Verifica se cabe no visor
	addiu $t5, $zero, 10
	multu $t3, $t5
	mflo $a0
	addu $a0, $a0, $t4
	
	# Atualiza visor se estiver tudo certo
	jal atualiza_visor_ponto
	j fim_busca

#-----------------------------------
# @brief Atualiza os visores com ponto decimal
# @param $a0 - N�mero a ser exibido
#------------------------------------
atualiza_visor_ponto:
	# Carrega os endere�os dos visores
	lui $t5, 0xFFFF
	ori $t5, $t5, 0x0011	# Lado esquerdo
	lui $t6, 0xFFFF
	ori $t6, $t6, 0x0010	# lado direito
	
	# Calcula a parte inteira
	addiu $t7, $zero, 10
	divu $a0, $t7
	mflo $t8				# $t8 <- Digito esquerdo, inteiro
	mfhi $t9				# $t9 = Digito direito, fracionado
	
	# Carrega endere�o base do MAPA_SEGMENTO
	lui $t7, 0x1001
	ori $t7, $t7, 0x0014
	
	# Carrega os bits do ponto
	lui $t4, 0x1001
	ori $t4, $t4, 0x001F	# Endere�o  0014 + 11 = 001F
	lb $t4, 0($t4)			# $t4 <- 0x080 como no .data
	
	# Exibe no visor esquerdo com o ponto
	addu $t3, $t7, $t8		# Endere�o do d�gito esquerdo
	lb $t3, 0($t3)			# Carrega d�gito
	or $t3, $t3, $t4		# Combina com o ponto
	sb $t3, 0($t5)			# Mostra no visor esquerdo
	
	# Exibe no visor direito
	addu $t3, $t7, $t9		# Endere�o do d�gito direito
	lb $t3, 0($t3)			# Carrega o d�gito
	sb $t3, 0($t6)			# Mostra no visor direito
	
	# Zera $s3 pra n�o ser empilhado
	addiu $s3, $zero, 0
	
	jr $ra

#----------------------------------------
# @brief Prepara Fibbonacci
#
# Prepara a chamada e lida com o resultado obtido da recurs�o
#----------------------------------------
# n -> $s3
funcao_e:
	# n � passado como par�metro
	add $a0, $s3, $zero
	jal fib_recursivo
	
	# Verifica se o resultado pode ser mostrado no visor (< 100)
	addiu $t3, $zero, 100
	slt $t3, $v0, $t3			# $t3 = 1 se resultado for < 100
	beq $t3, $zero, exibe_erro	# Mostra erro se >= 100
	
	# Atualia o resultado no visor
	add $a0, $v0, $zero
	jal atualiza_visor
	j fim_busca

#----------------------------------------
# @brief Fun��o recursiva para Fibonacci
# @param $a0 - n
# @return $v0 - fib_recursivo(n)
#----------------------------------------
fib_recursivo:
	# Processo mostrado em sala para fun��es recursivas
	# Salva estado da pilha
	addiu $sp, $sp, -12			# Abre 3 palavras (12 bytes) na pilha
	sw $ra, 8($sp)				# Salva endere�o de retorno
	sw $s0, 4($sp)				# Salva o registrador $s0
	sw $a0, 0($sp)				# Salva argumento 'n'

	# Caso base
	bne $a0, $zero, nao_eh_zero # Se n == 0, retorna 0
	addiu $v0, $zero, 0			# fib_recursivo(0) = 0
	j fib_fim					# Pula para o fim

# Segundo caso base
nao_eh_zero:
	addiu $t3, $zero, 1				# Se n == 1, retorna 1
	bne $a0, $t3, passo_recursivo
	addiu $v0, $zero, 1				# fib_recursivo(1) = 1
	j fib_fim						# Pula para o fim

# fib_recursivo(n-1) + fib_recursivo(n-2)
passo_recursivo:
	# Calcula fib_recursivo(n-1)
	addiu $a0, $a0, -1	# n <- n - 1
	jal fib_recursivo	# Chama fib_recursivo(n-1), volta em $v0
	
	add $s0, $v0, $zero	# Guarda resultado em $s0
	
	# Calcula fib_recursivo(n-2)
	lw $a0, 0($sp)		# Restaura o 'n' original
	addiu $a0, $a0, -2	# n <- n - 2 
	jal fib_recursivo	# Chama fib_recursivo(n-2), volta em $v0
	
	# Soma os resultados fib_recursivo = fib_recursivo(n-1) + fib_recursivo(n-2)
	addu $v0, $s0, $v0

# Finaliza chamada
fib_fim:
	lw $ra, 8($sp)		# Restaura endere�o de retorno
	lw $s0, 4($sp)		# Retaura $s0
	addiu $sp, $sp, 12	# Fecha espa�o na pilha
	jr $ra				# Retorna pra quem chamou
	
#----------------------------------------
# @brief Exibe erro no visor. "EE"
#----------------------------------------
exibe_erro:
	# Carrega o padr�o de 7 segmentos
	lui $t7, 0x1001			
	ori $t7, $t7, 0x0014	# Endere�o base do MAPA_SEGMENTO
	addiu $t4, $zero, 10	# �ndice da letra 'E' � 10
	addu $t7, $t7, $t4		# Endere�o padr�o do 'E'
	lb $t7, 0($t7)			# Salva em $s7
	
	# Endere�os dos visores
	lui $t5, 0xFFFF
    ori $t5, $t5, 0x0011 # Visor esquerdo
    lui $t6, 0xFFFF
    ori $t6, $t6, 0x0010 # Visor direito
    
    # Escreve 'E' nos dois visores
    sb $t7, 0($t5)
    sb $t7, 0($t6)
    
    # Reseta n�mero total pra n�o ser empilhado
    addiu $s3, $zero, 0
    
    j fim_busca

#----------------------------------------
# @brief Limpa tudo
#
# Limpa pilha e valores inseridos
#----------------------------------------
funcao_f:
	# Reseta o topo da pilha para 0
	lui $t4, 0x1001
	ori $t4, $t4, 0x0048	# Endere�o de PILHA_TOPO
	sw $zero, 0($t4)		# Salva o valor 0
	
	# Reseta o n�mero atual do visor
	addiu $s3, $zero, 0
	
	# Atualiza visor
	add $a0, $s3, $zero
	jal atualiza_visor
	
	j fim_busca

#-----------------------------------
# @brief Atualiza os visores
#	
# Realiza c�lculo de dezena e unidade, mostrando ambas em seus
# respectivos visores
#
# @param $a0 - N�mero a ser exibido
#------------------------------------
atualiza_visor:
	# Carrega os endere�os do visor
	# Visor esquerda vai ficar em ($t5)
	lui $t5, 0xFFFF
	ori $t5, $t5, 0x0011
	
	# Visor direito fica em ($t6)
	lui $t6, 0xFFFF
	ori $t6, $t6, 0x0010
	
	# Calcula o d�gito da dezena (dezena = numero / 10)
	addiu $t7, $zero, 10
	divu $a0, $t7			# Armazena quociente em LO e resto em HI de acordo com help
	mflo $t8				# $t8 <- Digito da dezena
	
	# Calcula o digito da unidade (unidade = numero % 10 -> o resto)
	mfhi $t9				# $t9 <- Digito da unidade
	
	# Carrega o endere�o base do mapa de segmentos
	lui $t7, 0x1001
	ori $t7, $t7, 0x0014	# Mapa come�a em 0x10010014
	
	# Exibe o digito da dezena no visor esquerdo
	addu $t4, $t7, $t8		# Endere�o do padr�o = base ($t7) + digito da dezena ($t8)
	lb $t4, 0($t4)			# Carrega o padr�o de 7 segmentos
	sb $t4, 0($t5)			# Escreve no visor esquerdo
	
	# Exibe o digito da unidade no visor direito
	addu $t4, $t7, $t9		# Endere�o do padr�o = base ($t7) + digito da unidade ($t9)
	lb $t4, 0($t4)			# Carrega o padr�o de 7 segmentos
	sb $t4, 0($t6)			# Escreve no direito
	
	jr $ra					# Retorna da fun��o
