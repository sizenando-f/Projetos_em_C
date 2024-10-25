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
	WireApp wire;

	app.insereComponente(&andGate);
	app.insereComponente(&orGate);
	app.insereComponente(&notGate);
	app.insereComponente(&wire);

	app.mainDoApp();
	return 0;
}