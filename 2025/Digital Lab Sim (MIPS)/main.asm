#	@file: main.asm
#	Implementação do projeto usando Digital Lab Sim.
#	Inserção em pilha, cálculo de média aritmética, desvio padrão, sequência de van eck e fibbonacci
#
#	@authors: Sizenando S. França (50575), Alexandre... (-----)
#	@date: 27-10-2025

# Definição de constantes para facilitar implementação e evitar confusão
.data
	# Endereços do Digital Lab Sim de acordo com o manual
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
	
	# Para decodificar as teclas baseado nos 16 códigos do help
					# 1, 0, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	CODIGOS_TECLAS:	.byte 0x11, 0x21, 0x41, 0x81, 0x12, 0x22, 0x42, 0x82, 0x14, 0x24, 0x44, 0x84, 0x18, 0x28, 0x48, 0x88
	VALORES_TECLAS:	.byte 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15
	
# Início do que é executável
.text
# Indica o ponto de entrada do programa para o SO, nesse caso, o simulador
.globl main

# Para inicialização de "variáveis"
main:
	# Carrega o endereço de CHAVE_CONTROLE para ($t0)
	lui $t0, 0xFFFF			# Primeira metade de bits
	ori $t0, $t0, 0x0012	# Segunda metade de bits
		
	# Carrega o endereço de CHAVE_DADO para ($t1)
	lui $t1, 0xFFFF			# Primeira metade de bits
	ori $t1, $t1, 0x0014	# Segunda mentade de bits
		
	# Para salvar a linha que está sendo varrida
	addiu $s0, $zero, 1		# ($s0) = 1 -> primeira linha
	
	# Numero atual do display
	addiu $s3, $zero, 0

# Para ficar varrendo o teclado
main_loop:
	# Ativa a linha atual ao escrever $s0 em $t0 (controle do teclado)
	sb $s0, 0($t0)						# Grava palavra de $s1 -> $t0
		
	# Lê o resultado do teclado
	lb $t2, 0($t1)						# Recebe palavra de $t1 -> $t2
		
	# Verifica se alguma tecla foi pressionada
	bne $t2, $zero, tecla_pressionada	# $t2 != 0 ? tecla_pressioanda
		
	# Para caso nenhuma foi pressionada, avança para próxima linha
	sll $s0, $s0, 1						# Multiplica $s0 por 2, ou seja, "sobe" um bit na sequência de bits
		
	# Verifica se já passou da última linha
	# 16 significa que as 4 linhas já foram
	sltiu $t3, $s0, 16					# $t3 = 1 se $s0 < 16 (n chegou ao fim), caso contrário $t3 = 0 (chegou ao fim)
	bne $t3, $zero, main_loop			# Se for menor que 16 então continua loop
	addiu $s0,$zero,1					# Senão reseta $s0 para 1
		
	j main_loop							# Volta pro inicio pra varrer próxima linha

# Código está em $t2
tecla_pressionada:

# Para esperar o usuário soltar a tecla
aguarda_soltar_tecla:
	lb $t3, 0($t1)							# Lê o teclado novamente
	bne $t3, $zero, aguarda_soltar_tecla	# Se ainda não for zero, espera mais
	
	# Tecla foi solta, então processa o que esta´em $t2
	# $s1 -> indice de leitura para CODIGOS_TECLAS
	addiu $s1, $zero, 0						# Inicia $s1 com 0

# Para buscar o código
busca_tecla_loop:
	# Se o indice chegar ao final, então não foi encontrado
	sltiu $t3, $s1, 16			# $t3 = 1 se $s1 < 16, caso contrário $t3 = 0
	beq $t3, $zero, fim_busca	# Se $s1 >= 16 então ele sai do loop
	
	# Carrega o endereço base de CODIGOS_TECLAS em $t4
	lui $t4, 0x1001				# Carrega a parte alta do endereço
	ori $t4, $t4, 0x004C		# Carrega a parte baixa
	
	# Para o índice para encontrar o endereço do elemento
	addu $t4, $t4, $s1
	
	# Carrega o byte no endereço calculado
	lb $t3, 0($t4)
	
	# Compara o código da tabela ($t3) com o código da tecla pressionado ($t2)
	beq $t3, $t2, tecla_encontrada	# Se for iguais, foi encontrado
	
	# Se não encontrou então avança o índice
	addiu $s1, $s1, 1				# $s1++     
	
	j busca_tecla_loop 				# Reinicia loop		

# $s1 possui o índice da tecla
# Para quando encontrar tecla
tecla_encontrada:
	# Carrega o endereço base de VALORES_TECLAS em $t4
	lui $t4, 0x1001		# Carrega a parte alta do endereço
	ori $t4, $t4, 0x005C	# Carrega a parte baixa
	
	# Para o índice para encontrar o valor correspondente
	addu $t4, $t4, $s1
	
	# Carrega o byte do valor real
	lb $s2, 0($t4)
	
	# VAi para o bloco certo
	sltiu $t3, $s2, 10					# $t3 = 1 se $s2 < 10, quer dizer que é um número
	bne $t3, $zero, processa_numero		# Se for número
	
	# Se não, é uma função
	j processa_funcao
	
fim_busca:
	# Se não foi encontrado ou tudo já foi processado, volta ao loop principal
	j main_loop

# Valor do digito pressionado -> $s2
# Número atual do visor -> $s3
# numero_atual = (numero atual * 10) + novo digito	
processa_numero:
	# Verifica se o número passou de 3 digitos
	sltiu $t3, $s3, 10			# $t3 = 1 se $s3 < 10, se for 0 então $s3 já tem 2 digitos
	beq $t3, $zero, overflow	# se $t3 >= 10 então não adiciona digito
	
	# Multiplica o número atual por 10
	addiu $t3, $zero, 10	# Prepara multiplicando
	multu $s3, $t3			# numero atual * 10
	mflo $s3				# $s3 = resultado
	
	# Adiciona o novo dígito
	addu $s3, $s3, $s2		# $s3 = ($s3 * 10) + $s2
	
	# Atualiza o visor
	add $a0, $s3, $zero		# Coloca o número a ser exibido para ser passado como parâmetro
	jal atualiza_visor

	j fim_busca
	
overflow:
	j fim_busca
	
# Valor da função -> $s2
processa_funcao:
	# Para tecla 'a' (10 em hexa)
	addiu $t3, $zero, 10
	beq $s2, $t3, funcao_a
	
	# Para tecla 'f'
	addiu $t3, $zero, 15
	beq $s2, $t3, funcao_f
	
	# Se não for nenhuma das funções
	j fim_busca

#----------------------------------------
# @brief Empilha o número
#
# Empilha o valor apresentado no visor no vetor
#----------------------------------------
funcao_a:
	lui $t4, 0x1001
	ori $t4, $t4, 0x0048		# Endereço do topo da pilha (0x10010020 + 40)
	lw $t5, 0($t4)				# $t5 = valor do topo da pilha
	
	# Verifica se pilha está cheia
	addiu $t6, $zero, 10
	slt $t6, $t5, $t6			# $t6 = 1 se topo < 10, senão 0
	beq $t6, $zero, fim_busca	# Se topo >= 10 então a pilha está cheia
	
	# Calcula endereço de destino -> base + (topo * 4)
	lui $t6, 0x1001
	ori $t6, $t6, 0x0020		# Endereço base da PILHA_ARRAY
	sll $t7, $t5, 2				# $t7 = topo * 4
	addu $t6, $t6, $t7			# $t6 = endereço de destino
	
	# Armazena número atual na ppilha
	sw $s3, 0($t6)
	
	# Incrementa o topo da pilha
	addiu $t5, $t5, 1	# Incrementa o valor em $t5
	sw $t5, 0($t4)		# Salva o novo valor
	
	# Reseta o número atual e atualiza o visor para 00
	addiu $s3, $zero, 0
	add $a0, $s3, $zero
	jal atualiza_visor
	
	j fim_busca

#----------------------------------------
# @brief Limpa tudo
#
# Limpa pilha e valores inseridos
#----------------------------------------
funcao_f:
	# Reseta o topo da pilha para 0
	lui $t4, 0x1001
	ori $t4, $t4, 0x0048	# Endereço de PILHA_TOPO
	sw $zero, 0($t4)		# Salva o valor 0
	
	# Reseta o número atual do visor
	addiu $s3, $zero, 0
	
	# Atualiza visor
	add $a0, $s3, $zero
	jal atualiza_visor
	
	j fim_busca

#-----------------------------------
# @brief Atualiza os visores
#	
# Realiza cálculo de dezena e unidade, mostrando ambas em seus
# respectivos visores
#
# @param $a0 - Número a ser exibido
# @return Nada
#------------------------------------
atualiza_visor:
	# Carrega os endereços do visor
	# Visor esquerda vai ficar em ($t5)
	lui $t5, 0xFFFF
	ori $t5, $t5, 0x0011
	
	# Visor direito fica em ($t6)
	lui $t6, 0xFFFF
	ori $t6, $t6, 0x0010
	
	# Calcula o dígito da dezena (dezena = numero / 10)
	addiu $t7, $zero, 10
	divu $a0, $t7			# Armazena quociente em LO e resto em HI de acordo com help
	mflo $t8				# $t8 = Digito da dezena
	
	# Calcula o digito da unidade (unidade = numero % 10 -> o resto)
	mfhi $t9				# $t9 = Digito da unidade
	
	# Carrega o endereço base do mapa de segmentos
	lui $t7, 0x1001
	ori $t7, $t7, 0x0014	# Mapa começa em 0x10010014
	
	# Exibe o digito da dezena no visor esquerdo
	addu $t4, $t7, $t8		# Endereço do padrão = base ($t7) + digito da dezena ($t8)
	lb $t4, 0($t4)			# Carrega o padrão de 7 segmentos
	sb $t4, 0($t5)			# Escreve no visor esquerdo
	
	# Exibe o digito da unidade no visor direito
	addu $t4, $t7, $t9		# Endereço do padrão = base ($t7) + digito da unidade ($t9)
	lb $t4, 0($t4)			# Carrega o padrão de 7 segmentos
	sb $t4, 0($t6)			# Escreve no direito
	
	jr $ra					# Retorna da função