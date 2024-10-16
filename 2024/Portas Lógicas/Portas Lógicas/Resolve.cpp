#include "Resolve.h"

using namespace std;

void Resolve::insere(Base* componente)
{
	componentes.push_back(componente->alocar());
}

void Resolve::resolveTotal()
{
	for (const auto& componente : componentes) {
		vector<string> entradasIndices = componente->retornaEntradas();
		vector<string> saidasIndices = componente->retornaSaidas();
		vector<int> entradas;

		for (const auto& indices : entradasIndices) {
			entradas.push_back(campo[indices[0] - '0'][7 - (indices[1] - '0')]);
		}

		vector<bool> resultados = componente->calcula(entradas);

		for (const auto& indices : saidasIndices) {
			for (const auto& resultado : resultados) {
				campo[indices[0] - '0'][7 - (indices[1] - '0')] = resultado;
			}
		}
		
	}
}

Resolve::~Resolve()
{
	for (const auto& componente : componentes) {
		delete componente;
	}
}
