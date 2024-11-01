#pragma once
#include "Componentes.h"

class ComponentesApp
{
	// Nome do componente
	string nome;
public:
	// Getter e setter para o nome do componente
	string getNome() const;
	void setNomeApp(string n);

	// Função para inserção de entradas e saídas
	virtual int leEntradasESaidas() = 0;

	// Alocação do app
	virtual ComponentesApp* alocarApp() = 0;

	// Função para retornar o número de entradas do componente
	virtual unsigned getNentradas() = 0;

	// Função para carregar componentes do arquivo
	virtual void carregaComponentes(vector<string> e, vector<string> s) = 0;

	// Destrutor
	virtual ~ComponentesApp() {};
};

class AndApp : public And, public ComponentesApp {
public:
	// Construtor
	AndApp();

	// Funções sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;
	
	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class OrApp : public Or, public ComponentesApp {
public:
	// Construtor
	OrApp();

	// Funções sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;

	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class NotApp : public Not, public ComponentesApp {
public:
	// Construtor
	NotApp();

	// Funções sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;
	
	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class WireApp : public Wire, public ComponentesApp {
public:
	// Construtor
	WireApp();

	// Funções sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;
	
	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};