#pragma once
#include "Componentes.h"
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class Resolve
{
	// Vetor com os componentes inseridos no campo de pinos
	vector<Base*> componentes;

	// Campo de pinos e as entradas e saídas de bits
	int campo[8][8], entrada[8], saida[8];
public:
	// Construtor
	Resolve();

	// Insere componente no campo
	void insere(Base*);

	// Calcula a lógica de todos os componentes
	void resolveTotal();

	// Exibe o campo de pinos e as entradas e saídas
	void exibeCampo() const;

	// Edita entrada de bits
	void editaEntradas(string entradas);

	// Remove componente do campo
	void removeComponente(int indice);

	// Retorna os componentes que estão inseridos no campo
	vector<Base*> getComponentes();

	// Ordena os componentes garantindo que não há erros caso o usuário insira um componente antes do outro
	void ordenaComponentes();

	// Salva componentes em um arquivo .txt
	void salvaComponentes(string nome);

	// Reinicia o campo, entradas e saídas para caso o usuário carregue um novo arquivo
	void reinicia();

	// Destrutor
	~Resolve();
};