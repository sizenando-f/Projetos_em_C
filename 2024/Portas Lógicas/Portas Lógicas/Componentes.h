#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Base
{
	vector<string> entradas, saidas;
	string nome;
public:
	virtual Base* alocar() = 0;
	virtual string getNome() const;
	virtual void setNome(string n);
	virtual vector<string> getEntradas() const;
	virtual vector<string> getSaidas() const;
	virtual void setEntradas(vector<string> e);
	virtual void setSaidas(vector<string> s);
	virtual void editaEntradasSaidas() = 0;
	virtual vector<bool> calcula(vector<int> entradas) const = 0;
	virtual ~Base();
};

class And : public Base {;
public:
	And();
	And(vector<string> e, vector<string> s);

	void editaEntradasSaidas() override;

	vector<bool> calcula(vector<int> entradas) const;

	Base* alocar() override;
};

class Or : public Base {
public:
	Or();
	Or(vector<string> e, vector<string> s);

	void editaEntradasSaidas() override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Not : public Base {
public:
	Not();
	Not(vector<string> e, vector<string> s);

	void editaEntradasSaidas() override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Wire : public Base {
public:
	Wire();
	Wire(vector<string> e, vector<string> s);

	void editaEntradasSaidas() override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};