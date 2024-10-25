#include "ComponentesApp.h"
#include <iostream>

using namespace std;

AndApp::AndApp()
{
	setNomeApp("and");
}

void AndApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	temp.push_back(e);
	
	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	temp.push_back(e);
	this->setEntradas(temp);

	cout << "Entre com a saida: ";
	cin >> e;

	temp.clear();
	temp.push_back(e);
	
	this->setSaidas(temp);
}


ComponentesApp* AndApp::alocarApp()
{
	return new AndApp;
}

OrApp::OrApp()
{
	setNomeApp("or");
}

void OrApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	temp.push_back(e);

	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	temp.push_back(e);
	this->setEntradas(temp);

	cout << "Entre com a saida: ";
	cin >> e;

	temp.clear();
	temp.push_back(e);

	this->setSaidas(temp);
}

ComponentesApp* OrApp::alocarApp()
{
	return new OrApp;
}

NotApp::NotApp()
{
	setNomeApp("not");
}

void NotApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da entrada: ";
	cin >> e;
	temp.push_back(e);
	this->setEntradas(temp);

	cout << "Entre com os indices da saida: ";
	cin >> e;

	temp.clear();
	temp.push_back(e);

	this->setSaidas(temp);
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

void WireApp::leEntradasESaidas()
{
	vector<string> temp;
	string e;
	cout << "Entre com os indices da entrada: ";
	cin >> e;
	temp.push_back(e);
	this->setEntradas(temp);

	cout << "Entre com os indices da saida: ";
	cin >> e;

	temp.clear();
	temp.push_back(e);

	this->setSaidas(temp);
}

ComponentesApp* WireApp::alocarApp()
{
	return new WireApp;
}
