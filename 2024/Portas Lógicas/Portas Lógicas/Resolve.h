#pragma once
#include "Componentes.h"
#include <vector>
#include <algorithm>

using namespace std;

class Resolve
{
	vector<Base*> componentes;
	int campo[8][8], entrada[8], saida[8];
public:
	Resolve();
	void insere(Base*);
	void resolveTotal();
	void exibeCampo() const;
	void editaEntradas(string entradas);
	void editaComponente(int indice);
	void removeComponente(int indice);
	vector<Base*> getComponentes();
	void ordenaComponentes();
	~Resolve();
};