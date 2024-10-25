#pragma once
#include "Componentes.h"

class ComponentesApp
{
	string nome;
public:
	string getNome() const;
	void setNomeApp(string n);
	virtual void leEntradasESaidas() = 0;
	virtual ComponentesApp* alocarApp() = 0;
	virtual ~ComponentesApp() {};
};

class AndApp : public And, public ComponentesApp {
public:
	AndApp();
	void leEntradasESaidas();

	ComponentesApp* alocarApp() override;
};

class OrApp : public Or, public ComponentesApp {
public:
	OrApp();
	void leEntradasESaidas();

	ComponentesApp* alocarApp() override;
};

class NotApp : public Not, public ComponentesApp {
public:
	NotApp();
	void leEntradasESaidas();

	ComponentesApp* alocarApp() override;
};