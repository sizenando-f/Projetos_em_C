#include "Resolve.h"

using namespace std;

Resolve::Resolve()
{
	/*
		Inicializa campos, entradas e sa�das
		- Zera entrada
		- Zera saida
		- Zera campo
	*/
	for (int i = 0; i < 8; ++i) {
		entrada[i] = 0;
		saida[i] = 0;
		for (int j = 0; j < 8; ++j) {
			campo[i][j] = 0;
		}
	}
}

void Resolve::insere(Base* componente)
{
	componentes.push_back(componente->alocar());	// Aloca o componente no vetor 

}

void Resolve::resolveTotal()
{
	/*
		Faz o c�lculo geral dos componentes inseridos
		- Ordena componentes garantindo fidelidade nos resultados
		- Realiza o c�lculo componente por componente
		- Exibe resultado no campo ou sa�da
	*/

	ordenaComponentes();
	for (const auto& componente : componentes) {
		vector<string> entradasIndices = componente->getEntradas(); // Recebe as entradas a qual o componente est� conectado
		vector<string> saidasIndices = componente->getSaidas();	// Recebe as sa�das a qual componente est� conectado
		vector<int> entradas;

		for (const auto& indices : entradasIndices) {
			if (indices != "") {	// Se houver �ndices de entrada
				if (indices[0] == 'e') {	// Se estiver conectado no vetor de entradas
					if (indices[1] - '0' >= 0 and indices[1] - '0' < 8) {
						entradas.push_back(entrada[indices[1] - '0']); // Pega o que est� no vetor de entradas a partir do �ndice convertido para inteiro
					}
				}
				else {
					entradas.push_back(campo[7 - (indices[0] - '0')][indices[1] - '0']); // Pega o que est� na matriz a partir dos �ndices convertido para inteiro
				}
			}
		}

		vector<bool> resultados = componente->calcula(entradas);	// Armazena os resultados do componente num vetor pra caso tenha mais de uma sa�da

		int i = 0;
		for (const auto& indices : saidasIndices) {
			if (indices != "") {	// Se houver �ndices de sa�da
				if (indices[0] == 's') {	// Se estiver conectado no vetor de sa�da
					if (indices[1] - '0' >= 0 and indices[1] - '0' < 8) {
						saida[indices[1] - '0'] = resultados[i];	// Insere o resultado no vetor de sa�da
					}
				}
				else {
					campo[7 - (indices[0] - '0')][indices[1] - '0'] = resultados[i];	// Insere o resultado no �ndice convertido para inteiro
				}
				i++;	// Incrementa para ir para pr�ximo resultado caso exista
			}
		}

	}
}

void Resolve::exibeCampo() const
{
	/*
		Exibe o campo de forma gr�fica e intuitiva
	*/

	cout << "s | ";
	for (int i = 0; i < 8; i++) {
		cout << i << " ";
	}
	cout << endl << "    ";
	for (int i = 0; i < 8; i++) {
		cout << saida[i] << " ";
	}

	cout << endl << endl;

	cout << "    ";
	for (int i = 0; i < 8; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i < 8; i++) {
		cout << 7-i << " | ";
		for (int j = 0; j < 8; j++) {
			cout << campo[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl << "    ";

	for (int i = 0; i < 8; i++) {
		cout << entrada[i] << " ";
	}
	cout << endl << "e | ";
	for (int i = 0; i < 8; i++) {
		cout << i << " ";
	}

	cout << endl;
}

void Resolve::editaEntradas(string entradas)
{
	/*
		Edita a entrada de bits
		- Verifica se foi entrado 8 bits
		- Insere os bits que forem 0 ou 1
	*/
	if (entradas.size() != 8) {
		return;
	}
	for (int i = 0; i < 8; i++) {
		if (entradas[i] - '0' == 0 or entradas[i] - '0' == 1) {
			entrada[i] = entradas[i] - '0';
		}
	}
}

void Resolve::removeComponente(int indice)
{
	/*
		Remove um componente desejado
		- Zera a sa�da a qual o componente est� conectado
		- Remove o componente do vetor de componentes
	*/

	if (indice > 0 && indice <= componentes.size()) {
		vector<string> ent = componentes[indice - 1]->getSaidas();

		if (ent.size() != 0) {
			for (const auto& indices : ent) {
				if (indices[0] == 'e') {
					entrada[indices[1] - '0'] = 0;
				}
				else if (indices[0] == 's') {
					saida[indices[1] - '0'] = 0;
				}
				else {
					campo[7 - (indices[0] - '0')][indices[1] - '0'] = 0;
				}
			}
		}

		componentes.erase(next(componentes.begin(), indice - 1));
	}

}

vector<Base*> Resolve::getComponentes()
{
	return componentes;
}

void Resolve::ordenaComponentes()
{
	/*
		Ordena componentes garantindo integridade nos resultados
		- Os componentes que tiverem a sa�da mais pr�xima do vetor de entrada s�o colocados no �nicio do vetor
		- � utilizado a ide�a de fun��es lambda apresentadas na aula de Lisp
	*/

	sort(componentes.begin(), componentes.end(), [](Base* a, Base* b) {
		auto saidasA = a->getSaidas();
		auto saidasB = b->getSaidas();

		// Fun��o auxiliar para obter o menor �ndice num�rico
		auto obterMenorIndice = [](const vector<string>& saidas) -> int {
			int menorIndice = INT_MAX;
			for (const auto& saida : saidas) {
				if (saida[0] != 'e' && saida[0] != 's') {  // Se n�o for 'e' ou 's'
					int indiceNumerico = saida[1] - '0';    // Converte o segundo caractere para inteiro
					menorIndice = min(menorIndice, indiceNumerico);
				}
			}
			return menorIndice;
			};

		int menorIndiceA = obterMenorIndice(saidasA);
		int menorIndiceB = obterMenorIndice(saidasB);

		// Compara��o baseada no menor �ndice de sa�da
		return menorIndiceA < menorIndiceB;
		});
}

void Resolve::salvaComponentes(string nome)
{
	/*
		Grava��o de cada componente no arquivo com suas respectivas sa�das e entradas
	*/

	ofstream arquivo(nome);
	if (arquivo.is_open()) {
		for (const auto& componente : componentes) {
			vector<string> entradas = componente->getEntradas();
			vector<string> saidas = componente->getSaidas();
			arquivo << componente->getNome() << " ";
			for (const auto& indice : entradas) {
				arquivo << indice << " ";
			}
			for (const auto& indice : saidas) {
				arquivo << indice << " ";
			}
			arquivo << endl;
		}
		arquivo.close();
	}
	else {
		return;
	}
}

void Resolve::reinicia()
{
	/*
		Reinicia todos os vetores
	*/

	componentes.clear();
	componentes.shrink_to_fit();
	for (int i = 0; i < 8; ++i) {
		saida[i] = 0;
		for (int j = 0; j < 8; ++j) {
			campo[i][j] = 0;
		}
	}
}

Resolve::~Resolve()
{
	for (const auto& componente : componentes) {
		delete componente;	// Desaloca cada componente 
	}
}