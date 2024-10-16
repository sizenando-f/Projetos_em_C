#include "Componentes.h"

Base::~Base()
{
}

And::And()
{
}

And::And(string e1, string e2, string s): entrada1(e1), entrada2(e2), saida(s)
{
}

void And::setEntrada1(const string e1)
{
	entrada1 = e1;
}

void And::setEntrada2(const string e2)
{
	entrada2 = e2;
}

void And::setSaida(const string s)
{
	saida = s;
}

string And::getEntrada1() const
{
	return entrada1;
}

string And::getEntrada2() const
{
	return entrada2;
}

string And::getSaida() const
{
	return saida;
}

vector<bool> And::calcula(vector<int> entradas) const
{
	bool result = true;
	for (int i = 0; i < entradas.size(); i++) {
		result = result and entradas[i];
	}
	vector<bool> temp = { result };

	return temp;
}

vector<string> And::retornaEntradas() const
{
	vector<string> temp = { entrada1, entrada2 };
	return temp;
}

vector<string> And::retornaSaidas() const
{
	vector<string> temp = { saida };
	return temp;
}


Base* And::alocar()
{
	return new And(this->entrada1, this->entrada2, this->saida);
}

Or::Or()
{
}

Or::Or(string e1, string e2, string s) : entrada1(e1), entrada2(e2), saida(s)
{
}

void Or::setEntrada1(const string e1)
{
	entrada1 = e1;
}

void Or::setEntrada2(const string e2)
{
	entrada2 = e2;
}

void Or::setSaida(const string s)
{
	saida = s;
}

string Or::getEntrada1() const
{
	return entrada1;
}

string Or::getEntrada2() const
{
	return entrada2;
}

string Or::getSaida(const string s)
{
	return saida;
}

vector<bool> Or::calcula(vector<int> entradas) const
{
	bool result = false;
	for (int i = 0; i < entradas.size(); i++) {
		result = result or entradas[i];
	}
	vector<bool> temp = { result };

	return temp;
}

vector<string> Or::retornaEntradas() const
{
	vector<string> temp = { entrada1, entrada2 };
	return temp;
}

vector<string> Or::retornaSaidas() const
{
	vector<string> temp = { saida };
	return temp;
}


Base* Or::alocar()
{
	return new Or(this->entrada1, this->entrada2, this->saida);
}

Not::Not()
{
}

Not::Not(string e, string s): entrada(e), saida(s)
{
}

void Not::setEntrada(const string e)
{
	entrada = e;
}

void Not::setSaida(const string s)
{
	saida = s;
}

string Not::getEntrada() const
{
	return entrada;
}

string Not::getSaida() const
{
	return saida;
}

vector<bool> Not::calcula(vector<int> entradas) const
{
	vector<bool> result = { not entradas[0] };
	return result;
}

vector<string> Not::retornaEntradas() const
{
	vector<string> temp = { entrada };
	return temp;
}

vector<string> Not::retornaSaidas() const
{
	vector<string> temp = { saida };
	return temp;
}


Base* Not::alocar()
{
	return new Not(this->entrada, this->saida);
}
