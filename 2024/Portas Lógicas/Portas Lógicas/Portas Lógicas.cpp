#include <iostream>
#include <string>
#include "ResolveApp.h"

using namespace std;

int main()
{
	ResolveApp app;
	AndApp andGate;
	OrApp orGate;
	NotApp notGate;

	app.insereComponente(&andGate);
	app.insereComponente(&orGate);
	app.insereComponente(&notGate);

	app.mainDoApp();
	return 0;
}