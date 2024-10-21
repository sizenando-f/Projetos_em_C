#include "Resolve.h"
#include <iostream>

using namespace std;

void Resolve::insere(Base* componente)
{
	componentes.push_back(componente->alocar());	// Aloca o componente no vetor 
}

void Resolve::resolveTotal()
{
	for (const auto& componente : componentes) {
		vector<string> entradasIndices = componente->retornaEntradas(); // Recebe as entradas a quais os componentes estão conectados
		vector<string> saidasIndices = componente->retornaSaidas();	// Recebe as saídas a quais os componentes estão conectados
		vector<int> entradas;

		for (const auto& indices : entradasIndices) {
			if (indices != "") {	// Se houver índices de entrada
				if (indices[0] == 'e') {	// Se estiver conectado no vetor de entradas
					if (indices[1] - '0' >= 0 and indices[1] - '0' < 8) {
						entradas.push_back(entrada[indices[1] - '0']); // Pega o que está no vetor de entradas a partir do índice convertido para inteiro
					}
				}
				else {
					entradas.push_back(campo[indices[0] - '0'][7 - (indices[1] - '0')]); // Pega o que está na matriz a partir dos índices convertido para inteiro
				}
			}
		}

		vector<bool> resultados = componente->calcula(entradas);	// Armazena os resultados do componente num vetor pra caso tenha mais de uma saída

		int i = 0;
		for (const auto& indices : saidasIndices) {
			if (indices != "") {	// Se houver índices de saída
				if (indices[0] == 's') {	// Se estiver conectado no vetor de saída
					if (indices[1] - '0' >= 0 and indices[1] - '0' < 8) {
						saida[indices[1] - '0'] = resultados[i];	// Insere o resultado no vetor de saída
					}
				}
				else {
					campo[indices[0] - '0'][7 - (indices[1] - '0')] = resultados[i];	// Insere o resultado no índice convertido para inteiro
				}
				i++;	// Incrementa para ir para próximo resultado caso exista
			}
		}
		
	}
}

void Resolve::exibeCampo() const
{
	for (int i = 0; i < 8; i++) {
		cout << saida[i] << " ";
	}

	cout << endl << endl;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << campo[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	
	for (int i = 0; i < 8; i++) {
		cout << entrada[i] << " ";
	}

	cout << endl;
}

void Resolve::editaEntradas(string entradas)
{
	if (entradas.size() != 8) {
		return;
	}
	for (int i = 0; i < 8; i++) {
		if (entradas[i] - '0' == 0 or entradas[i] - '0' == 1) {
			entrada[i] = entradas[i] - '0';
		}
	}
}

Resolve::~Resolve()
{
	for (const auto& componente : componentes) {
		delete componente;	// Desaloca cada componente 
	}
}
