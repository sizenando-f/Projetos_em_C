#pragma once
#include "Componentes.h"
#include <vector>

using namespace std;

class Resolve
{
	vector<Base*> componentes;
	int campo[8][8], entrada[8], saida[8];
public:
	void insere(Base*);
	void resolveTotal();
	void exibeCampo() const;
	void editaEntradas(string entradas);
	~Resolve();
};

