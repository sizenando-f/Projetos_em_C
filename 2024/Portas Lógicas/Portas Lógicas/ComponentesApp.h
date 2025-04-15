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

	// Fun��o para inser��o de entradas e sa�das
	virtual int leEntradasESaidas() = 0;

	// Aloca��o do app
	virtual ComponentesApp* alocarApp() = 0;

	// Fun��o para retornar o n�mero de entradas do componente
	virtual unsigned getNentradas() = 0;

	// Fun��o para carregar componentes do arquivo
	virtual void carregaComponentes(vector<string> e, vector<string> s) = 0;

	// Destrutor
	virtual ~ComponentesApp() {};
};

class AndApp : public And, public ComponentesApp {
public:
	// Construtor
	AndApp();

	// Fun��es sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;

	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class OrApp : public Or, public ComponentesApp {
public:
	// Construtor
	OrApp();

	// Fun��es sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;

	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class NotApp : public Not, public ComponentesApp {
public:
	// Construtor
	NotApp();

	// Fun��es sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;
	
	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class WireApp : public Wire, public ComponentesApp {
public:
	// Construtor
	WireApp();

	// Fun��es sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;
	
	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};

class AndOrApp : public AndOr, public ComponentesApp {
public:
	// Construtor
	AndOrApp();

	// Fun��es sobrescritas
	int leEntradasESaidas() override;

	unsigned getNentradas() override;

	void carregaComponentes(vector<string> e, vector<string> s) override;

	ComponentesApp* alocarApp() override;
};