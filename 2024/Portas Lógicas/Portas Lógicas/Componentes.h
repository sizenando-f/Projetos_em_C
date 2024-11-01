#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Base
{
	//Vetores com índices
	vector<string> entradas, saidas;

	// Nome do componente
	string nome;
public:
	// Getter e setter para o nome
	string getNome() const;
	void setNome(string n);

	// Getters e setters para entradas e saídas
	vector<string> getEntradas() const;
	vector<string> getSaidas() const;
	void setEntradas(vector<string> e);
	void setSaidas(vector<string> s);

	// Getter e setter para o númmero de entradas aceitas pelo componente
	virtual void setNumeroEntradas(unsigned n) = 0;
	virtual unsigned getNumeroEntradas() const = 0;

	// Funcão para alteração de entradas e saídas do componente
	virtual void editaEntradasSaidas() = 0;

	// Cálculo do componente lógico
	virtual vector<bool> calcula(vector<int> entradas) const = 0;

	// Alocação do componente e destrutor
	virtual Base* alocar() = 0;
	virtual ~Base();
};

/*
	- A criação de uma variável de número de entradas é necessário para o futuro carregamento de 
	componenentes, como uma variável de controle.
	- O uso de cin e cout em um componente não é recomendado, porém, o uso dele nesse caso se torna especial pois é necessário
	para a edição unitária de um componente já existente no campo de pinos.
*/

class And : public Base {
	// Inicialização do número de entradas aceitas pelo componente
	unsigned nEntradas = 2;
public:
	// Construtores
	And();
	And(vector<string> e, vector<string> s, unsigned nEntradas);

	// Funções sobrescritas
	void editaEntradasSaidas() override;

	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Or : public Base {
	// Inicialização do número de entradas aceitas pelo componente
	unsigned nEntradas = 2;
public:
	// Construtores
	Or();
	Or(vector<string> e, vector<string> s, unsigned nEntradas);

	// Funções sobrescritas
	void editaEntradasSaidas() override;

	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Not : public Base {
	// Inicialização do número de entradas aceitas pelo componente
	unsigned nEntradas = 1;
public:
	// Construtores
	Not();
	Not(vector<string> e, vector<string> s, unsigned nEntradas);

	// Funções sobrescritas
	void editaEntradasSaidas() override;

	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};

class Wire : public Base {
	// Inicialização do número de entradas aceitas pelo componente
	unsigned nEntradas = 1;
public:
	// Construtores
	Wire();
	Wire(vector<string> e, vector<string> s, unsigned nEntradas);

	// Funções sobrescritas
	void editaEntradasSaidas() override;

	void setNumeroEntradas(unsigned n) override;
	unsigned getNumeroEntradas() const override;

	vector<bool> calcula(vector<int> entradas) const override;

	Base* alocar() override;
};