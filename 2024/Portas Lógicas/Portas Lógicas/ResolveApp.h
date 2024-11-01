#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Resolve.h"
#include "ComponentesApp.h"

class ResolveApp
{
	// Vetor de componentes disponíveis para inserção
	vector<ComponentesApp*> registroDeComponentes;

	// Objeto para manipulação do campo
	Resolve resolve;
public:
	// Insere um componente externo
	void insereComponente(ComponentesApp*);

	// Função principal
	void mainDoApp();

	// Destrutor
	~ResolveApp();
};