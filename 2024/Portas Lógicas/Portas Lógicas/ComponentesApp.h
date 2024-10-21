#pragma once
#include "Componentes.h"

class ComponentesApp
{
	string nome;
public:
	string getNome() const;
	void setNome(string nome);
	virtual void leEntradasESaidas() = 0;
	virtual ComponentesApp* alocarApp() = 0;
};

class AndApp : private And, public ComponentesApp {
public:
	AndApp();
	void leEntradasESaidas();

	ComponentesApp *alocarApp() override;
};

class OrApp : private Or, public ComponentesApp {
public:
	OrApp();
	void leEntradasESaidas();

	ComponentesApp* alocarApp() override;
};

class NotApp : private Not, public ComponentesApp {
public:
	NotApp();
	void leEntradasESaidas();

	ComponentesApp* alocarApp() override;
};