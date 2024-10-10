#pragma once
#include <string>

using namespace std;

class Base
{
public:
	virtual Base* alocar() = 0;
	virtual ~Base();
};

class And : public Base {
	string entrada1, entrada2, saida;
public:
	And(string e1, string e2, string s);

	void setEntrada1(const string e1);
	void setEntrada2(const string e2);
	void setSaida(const string s);

	string getEntrada1() const;
	string getEntrada2() const;
	string getSaida() const;

	Base* alocar() override;
};

class Or : public Base {
	string entrada1, entrada2, saida;
public:
	Or(string e1, string e2, string s);

	void setEntrada1(const string e1);
	void setEntrada2(const string e2);
	void setSaida(const string s);

	string getEntrada1() const;
	string getEntrada2() const;
	string getSaida(const string s);

	Base* alocar() override;
};

class Not : public Base {
	string entrada, saida;
public:
	Not(string e, string s);

	void setEntrada(const string e);
	void setSaida(const string s);

	string getEntrada() const;
	string getSaida() const;

	Base* alocar() override;
};