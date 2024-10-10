#include "Componentes.h"

Base::~Base()
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

Base* And::alocar()
{
	return new And(this->entrada1, this->entrada2, this->saida);
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

Base* Or::alocar()
{
	return new Or(this->entrada1, this->entrada2, this->saida);
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

Base* Not::alocar()
{
	return new Not(this->entrada, this->saida);
}
