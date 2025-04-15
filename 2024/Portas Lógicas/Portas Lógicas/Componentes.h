#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Base
{
	//Vetores com �ndices
	vector<string> entradas, saidas;

	// Nome do componente
	string nome;

public:
	// Getter e setter para o nome
	string getNome() const;
	void setNome(string n);

	// Getters e setters para entradas e sa�das
	vector<string> getEntradas() const;
	vector<string> getSaidas() const;
	void setEntradas(vector<string> e);
	void setSaidas(vector<string> s);

	// Getter e setter para o n�mmero de entradas aceitas pelo componente
	virtual void setNumeroEntradas(unsigned n) = 0;
	virtual unsigned getNumeroEntradas() const = 0;

	// C�lculo do componente l�gico
	virtual vector<bool> calcula(vector<int> entradas) const = 0;

	// Aloca��o do componente e destrutor
	virtual Base* alocar() = 0;
	virtual ~Base();
};

class And : public Base {
	int nEntradas = 2;
public:
	// Construtores
	And();
	And(vector<string> e, vector<string> s, unsigned nEntradas);

	// Fun��es sobrescritas
	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Or : public Base {
	int nEntradas = 2;
public:
	// Construtores
	Or();
	Or(vector<string> e, vector<string> s, unsigned nEntradas);

	// Fun��es sobrescritas
	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Not : public Base {
	int nEntradas = 1;
public:
	// Construtores
	Not();
	Not(vector<string> e, vector<string> s, unsigned nEntradas);

	// Fun��es sobrescritas
	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Wire : public Base {
	int nEntradas = 1;
public:
	// Construtores
	Wire();
	Wire(vector<string> e, vector<string> s, unsigned nEntradas);

	// Fun��es sobrescritas
	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class AndOr : public Base {
	int nEntradas = 4;
public:
	// Construtores
	AndOr();
	AndOr(vector<string> e, vector<string> s, unsigned nEntradas);

	// Fun��es sobrescritas
	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};