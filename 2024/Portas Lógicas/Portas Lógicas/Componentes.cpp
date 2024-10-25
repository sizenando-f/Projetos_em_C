#include "Componentes.h"

Base::~Base()
{
}

string Base::getNome() const {
	return nome;
}

void Base::setNome(string n)
{
	nome = n;
}

vector<string> Base::getEntradas() const
{
	return entradas;
}

vector<string> Base::getSaidas() const
{
	return saidas;
}

void Base::setEntradas(vector<string> e)
{
	entradas = e;
}

void Base::setSaidas(vector<string> s)
{
	saidas = s;
}

And::And()
{
}

And::And(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("and");
}

void And::editaEntradasSaidas()
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

vector<bool> And::calcula(vector<int> entradas) const
{
	bool result = true;
	for (int i = 0; i < entradas.size(); i++) {
		result = result && entradas[i];
	}
	vector<bool> temp = { result };

	return temp;
}

Base* And::alocar()
{
	return new And(getEntradas(), getSaidas());
}

Or::Or()
{
}

Or::Or(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("or");
}

void Or::editaEntradasSaidas()
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

vector<bool> Or::calcula(vector<int> entradas) const
{
	bool result = false;
	for (int i = 0; i < entradas.size(); i++) {
		result = result || entradas[i];
	}
	vector<bool> temp = { result };

	return temp;
}


Base* Or::alocar()
{
	return new Or(getEntradas(), getSaidas());
}

Not::Not()
{
}

Not::Not(vector<string> e, vector<string> s)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("not");
}

void Not::editaEntradasSaidas()
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


vector<bool> Not::calcula(vector<int> entradas) const
{
	vector<bool> result = { !entradas[0] };
	return result;
}

Base* Not::alocar()
{
	return new Not(getEntradas(), getSaidas());
}