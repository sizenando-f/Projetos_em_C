#include "ComponentesApp.h"

using namespace std;

AndApp::AndApp()
{
	setNomeApp("and");
}

int AndApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if(e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);
	
	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	this->setEntradas(temp);
	this->setNumeroEntradas(static_cast<int>(temp.size()));

	cout << "Entre com a saida: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.clear();
	temp.push_back(e);
	
	this->setSaidas(temp);
	return 1;
}

unsigned AndApp::getNentradas()
{
	return this->getNumeroEntradas();
}

void AndApp::carregaComponentes(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNumeroEntradas(static_cast<int>(e.size()));
}


ComponentesApp* AndApp::alocarApp()
{
	return new AndApp;
}

OrApp::OrApp()
{
	setNomeApp("or");
}

int OrApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.push_back(e);
	this->setEntradas(temp);
	this->setNumeroEntradas(static_cast<int>(temp.size()));

	cout << "Entre com a saida: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.clear();
	temp.push_back(e);

	this->setSaidas(temp);
	return 1;
}

unsigned OrApp::getNentradas()
{
	return this->getNumeroEntradas();
}

void OrApp::carregaComponentes(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNumeroEntradas(static_cast<int>(e.size()));
}

ComponentesApp* OrApp::alocarApp()
{
	return new OrApp;
}

NotApp::NotApp()
{
	setNomeApp("not");
}

int NotApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da entrada: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	this->setEntradas(temp);
	this->setNumeroEntradas(static_cast<int>(temp.size()));

	cout << "Entre com os indices da saida: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.clear();
	temp.push_back(e);

	this->setSaidas(temp);
	return 1;
}

unsigned NotApp::getNentradas()
{
	return this->getNumeroEntradas();
}

void NotApp::carregaComponentes(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNumeroEntradas(static_cast<int>(e.size()));
}

ComponentesApp* NotApp::alocarApp()
{
	return new NotApp;
}


string ComponentesApp::getNome() const
{
	return nome;
}

void ComponentesApp::setNomeApp(string nome)
{
	this->nome = nome;
}

WireApp::WireApp()
{
	setNomeApp("wire");
}

int WireApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da entrada: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	this->setEntradas(temp);
	this->setNumeroEntradas(static_cast<int>(temp.size()));

	cout << "Entre com os indices da saida: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.clear();
	temp.push_back(e);

	this->setSaidas(temp);
	return 1;
}

unsigned WireApp::getNentradas()
{
	return this->getNumeroEntradas();
}

void WireApp::carregaComponentes(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNumeroEntradas(static_cast<int>(e.size()));
}

ComponentesApp* WireApp::alocarApp()
{
	return new WireApp;
}

AndOrApp::AndOrApp()
{
	setNomeApp("andor");
}

int AndOrApp::leEntradasESaidas()
{
	vector<string> temp;
	// Leitura das entradas
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	cout << "Entre com os indices da terceira entrada: ";
	cin >> e;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	cout << "Entre com os indices da quarta entrada: ";
	cin >> e;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;
	temp.push_back(e);

	this->setEntradas(temp);
	this->setNumeroEntradas(static_cast<int>(temp.size()));

	// Leitura das saída
	cout << "Entre com indice da primeira saida (saida AND): ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.clear();
	temp.push_back(e);

	cout << "Entre com índice da segunda saida (saida OR): ";
	cin >> e;
	if (e.size() != 2) return 0;
	if (e[0] != 's' && e[0] != 'e') {
		if (e[0] - '0' < 0 || e[0] - '0' > 7) return 0;
	}
	if (e[1] - '0' < 0 || e[1] - '0' > 7) return 0;

	temp.push_back(e);

	this->setSaidas(temp);
	return 1;
}

unsigned AndOrApp::getNentradas()
{
	return this->getNumeroEntradas();
}

void AndOrApp::carregaComponentes(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNumeroEntradas(static_cast<int>(e.size()));
}

ComponentesApp* AndOrApp::alocarApp()
{
	return new AndOrApp;
}
