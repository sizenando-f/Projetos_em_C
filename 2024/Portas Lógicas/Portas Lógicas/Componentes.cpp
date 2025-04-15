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

And::And(vector<string> e, vector<string> s, unsigned nEntradas)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("and");
	this->setNumeroEntradas(nEntradas);
}

void And::setNumeroEntradas(unsigned n)
{
	this->nEntradas = n;
}

unsigned And::getNumeroEntradas() const
{
	return this->nEntradas;
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
	return new And(getEntradas(), getSaidas(), getNumeroEntradas());
}

Or::Or()
{
}

Or::Or(vector<string> e, vector<string> s, unsigned nEntradas)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("or");
	this->setNumeroEntradas(nEntradas);
}

void Or::setNumeroEntradas(unsigned n)
{
	this->nEntradas = n;
}

unsigned Or::getNumeroEntradas() const
{
	return this->nEntradas;
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
	return new Or(getEntradas(), getSaidas(), getNumeroEntradas());
}

Not::Not()
{
}

Not::Not(vector<string> e, vector<string> s, unsigned nEntradas)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("not");
	this->setNumeroEntradas(nEntradas);
}

void Not::setNumeroEntradas(unsigned n)
{
	this->nEntradas = n;
}

unsigned Not::getNumeroEntradas() const
{
	return this->nEntradas;
}

vector<bool> Not::calcula(vector<int> entradas) const
{
	vector<bool> result = { !entradas[0] };
	return result;
}

Base* Not::alocar()
{
	return new Not(getEntradas(), getSaidas(), getNumeroEntradas());
}

Wire::Wire()
{
}

Wire::Wire(vector<string> e, vector<string> s, unsigned nEntradas)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("wire");
	this->setNumeroEntradas(nEntradas);
}

void Wire::setNumeroEntradas(unsigned n)
{
	this->nEntradas = n;

}

unsigned Wire::getNumeroEntradas() const
{
	return this->nEntradas;
}

vector<bool> Wire::calcula(vector<int> entradas) const
{
	vector<bool> vetor = { 0 };
	if (entradas[0] == 1) {
		vetor = { 1 };
	}
	return vetor;
}

Base* Wire::alocar()
{
	return new Wire(getEntradas(), getSaidas(), getNumeroEntradas());
}

AndOr::AndOr()
{
}

AndOr::AndOr(vector<string> e, vector<string> s, unsigned nEntradas)
{
	this->setEntradas(e);
	this->setSaidas(s);
	this->setNome("andor");
	this->setNumeroEntradas(nEntradas);
}

void AndOr::setNumeroEntradas(unsigned n)
{
	this->nEntradas = n;
}

unsigned AndOr::getNumeroEntradas() const
{
	return this->nEntradas;
}

vector<bool> AndOr::calcula(vector<int> entradas) const
{
	bool result = true;
	vector<bool> temp;
	// Teste AND
	for (int i = 0; i < entradas.size(); i++) {
		result = result && entradas[i];
	}
	
	temp.push_back(result);

	// Teste OR
	result = false;
	for (int i = 0; i < entradas.size(); i++) {
		result = result || entradas[i];
	}

	temp.push_back(result);

	return temp;
}

Base* AndOr::alocar()
{
	return new AndOr(getEntradas(), getSaidas(), getNumeroEntradas());
}
