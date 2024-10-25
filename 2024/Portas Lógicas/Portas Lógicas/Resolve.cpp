#include "Resolve.h"
#include <iostream>

using namespace std;

Resolve::Resolve()
{
	for (int i = 0; i < 8; ++i) {
		entrada[i] = 0;
		saida[i] = 0;
		for (int j = 0; j < 8; ++j) {
			campo[i][j] = 0;
		}
	}
}

void Resolve::insere(Base* componente)
{
	componentes.push_back(componente->alocar());	// Aloca o componente no vetor 

}

void Resolve::resolveTotal()
{
	for (const auto& componente : componentes) {
		vector<string> entradasIndices = componente->getEntradas(); // Recebe as entradas a quais os componentes estão conectados
		vector<string> saidasIndices = componente->getSaidas();	// Recebe as saídas a quais os componentes estão conectados
		vector<int> entradas;

		for (const auto& indices : entradasIndices) {
			if (indices != "") {	// Se houver índices de entrada
				if (indices[0] == 'e') {	// Se estiver conectado no vetor de entradas
					if (indices[1] - '0' >= 0 and indices[1] - '0' < 8) {
						entradas.push_back(entrada[indices[1] - '0']); // Pega o que está no vetor de entradas a partir do índice convertido para inteiro
					}
				}
				else {
					entradas.push_back(campo[7 - (indices[0] - '0')][indices[1] - '0']); // Pega o que está na matriz a partir dos índices convertido para inteiro
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
					campo[7 - (indices[0] - '0')][indices[1] - '0'] = resultados[i];	// Insere o resultado no índice convertido para inteiro
				}
				i++;	// Incrementa para ir para próximo resultado caso exista
			}
		}

	}
}

void Resolve::exibeCampo() const
{
	cout << "s | ";
	for (int i = 0; i < 8; i++) {
		cout << i << " ";
	}
	cout << endl << "    ";
	for (int i = 0; i < 8; i++) {
		cout << saida[i] << " ";
	}

	cout << endl << endl;

	cout << "    ";
	for (int i = 0; i < 8; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i < 8; i++) {
		cout << 7-i << " | ";
		for (int j = 0; j < 8; j++) {
			cout << campo[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl << "    ";

	for (int i = 0; i < 8; i++) {
		cout << entrada[i] << " ";
	}
	cout << endl << "e | ";
	for (int i = 0; i < 8; i++) {
		cout << i << " ";
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
void Resolve::editaComponente()
{
	int i = 1, esc;
	for (const auto& componente : componentes) {
		cout << i++ << ". " << componente->getNome() << " ";
		for (const auto& indices : componente->getEntradas()) {
			cout << indices << " ";
		}
		for (const auto& indices : componente->getSaidas()) {
			cout << indices << " ";
		}
		cout << endl;
	}
	cout << "Escolha: ";
	cin >> esc;

	vector<string> ent = componentes[esc - 1]->getSaidas();

	if (ent.size() != 0) {
		for (const auto& indices : ent) {
			if (indices[0] == 'e') {
				entrada[indices[1] - '0'] = 0;
			}
			else if (indices[0] == 's') {
				saida[indices[1] - '0'] = 0;
			}
			else {
				campo[7 - (indices[0] - '0')][indices[1] - '0'] = 0;
			}
		}
	}
	if (esc > 0 && esc <= componentes.size()) {
		componentes[esc - 1]->editaEntradasSaidas();
	}
}

Resolve::~Resolve()
{
	for (const auto& componente : componentes) {
		delete componente;	// Desaloca cada componente 
	}
}