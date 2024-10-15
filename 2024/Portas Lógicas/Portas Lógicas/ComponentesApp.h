#pragma once
#include "Componentes.h"

class ComponentesApp
{
	virtual void leEntradasESaidas() = 0;
	virtual ComponentesApp* alocarApp() = 0;
};

class AndApp : private And, public ComponentesApp {
	void leEntradasESaidas();

	ComponentesApp* alocarApp() override;
};

