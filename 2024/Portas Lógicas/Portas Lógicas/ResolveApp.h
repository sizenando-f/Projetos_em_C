#pragma once
#include <vector>
#include "Resolve.h"
#include "ComponentesApp.h"

class ResolveApp
{
	vector<ComponentesApp*> registroDeComponentes;
	Resolve resolve;
public:
	void insereComponente(ComponentesApp*);
	void mainDoApp();
	~ResolveApp();
};

