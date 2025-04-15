#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Resolve.h"
#include "ComponentesApp.h"

class ResolveApp
{
	// Vetor de componentes dispon�veis para inser��o
	vector<ComponentesApp*> registroDeComponentes;

	// Objeto para manipula��o do campo
	Resolve resolve;
public:
	// Insere um componente externo
	void insereComponente(ComponentesApp*);

	// Fun��o principal
	void mainDoApp();

	// Destrutor
	~ResolveApp();
};