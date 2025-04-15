#include "ResolveApp.h"

using namespace std;

int main()
{
	ResolveApp app;
	AndApp andGate;
	OrApp orGate;
	NotApp notGate;
	WireApp wire;
	AndOrApp andor;

	app.insereComponente(&andGate);
	app.insereComponente(&orGate);
	app.insereComponente(&notGate);
	app.insereComponente(&wire);
	app.insereComponente(&andor);

	app.mainDoApp();
	return 0;
}