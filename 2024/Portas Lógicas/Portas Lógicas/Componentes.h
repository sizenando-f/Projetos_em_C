#pragma once
#include <string>
#include <vector>

using namespace std;

class Base
{
public:
	virtual Base* alocar() = 0;
	virtual vector<bool> calcula(vector<int> entradas) const = 0;
	virtual vector<string> retornaEntradas() const = 0;
	virtual vector<string> retornaSaidas() const = 0;
	virtual ~Base();
};

class And : public Base {
	string entrada1, entrada2, saida;
public:
	And();
	And(string e1, string e2, string s);

	void setEntrada1(const string e1);
	void setEntrada2(const string e2);
	void setSaida(const string s);

	string getEntrada1() const;
	string getEntrada2() const;
	string getSaida() const;

	vector<bool> calcula(vector<int> entradas) const;
	vector<string> retornaEntradas() const override;
	vector<string> retornaSaidas() const override;

	Base* alocar() override;
};

class Or : public Base {
	string entrada1, entrada2, saida;
public:
	Or();
	Or(string e1, string e2, string s);

	void setEntrada1(const string e1);
	void setEntrada2(const string e2);
	void setSaida(const string s);

	string getEntrada1() const;
	string getEntrada2() const;
	string getSaida();

	vector<bool> calcula(vector<int> entradas) const override;

	vector<string> retornaEntradas() const override;
	vector<string> retornaSaidas() const override;

	Base* alocar() override;
};

class Not : public Base {
	string entrada, saida;
public:
	Not();
	Not(string e, string s);

	void setEntrada(const string e);
	void setSaida(const string s);

	string getEntrada() const;
	string getSaida() const;

	vector<bool> calcula(vector<int> entradas) const override;
	vector<string> retornaEntradas() const override;
	vector<string> retornaSaidas() const override;

	Base* alocar() override;
};