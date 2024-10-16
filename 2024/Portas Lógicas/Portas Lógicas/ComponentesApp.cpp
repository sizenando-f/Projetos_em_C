#include "ComponentesApp.h"
#include <iostream>

using namespace std;

AndApp::AndApp()
{
}

void AndApp::leEntradasESaidas()
{
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	this->setEntrada1(e);

	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	this->setEntrada2(e);

	cout << "Entre com a saida: ";
	cin >> e;
	this->setSaida(e);
}

ComponentesApp* AndApp::alocarApp()
{
	return new AndApp;
}

OrApp::OrApp()
{
}

void OrApp::leEntradasESaidas()
{
	string e;
	cout << "Entre com os indices da primeira entrada: ";
	cin >> e;
	this->setEntrada1(e);

	cout << "Entre com os indices da segunda entrada: ";
	cin >> e;
	this->setEntrada2(e);

	cout << "Entre com a saida: ";
	cin >> e;
	this->setSaida(e);
}

ComponentesApp* OrApp::alocarApp()
{
	return new OrApp;
}

NotApp::NotApp()
{
}

void NotApp::leEntradasESaidas()
{
	string e;
	cout << "Entre com os indices da entrada: ";
	cin >> e;
	this->setEntrada(e);

	cout << "Entre com a saida: ";
	cin >> e;
	this->setSaida(e);
}

ComponentesApp* NotApp::alocarApp()
{
	return new NotApp;
}
