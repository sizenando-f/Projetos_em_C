#include "ResolveApp.h"
#include <iostream>

void ResolveApp::insereComponente(ComponentesApp* comp)
{
	registroDeComponentes.push_back(comp->alocarApp());
}

void ResolveApp::mainDoApp()
{
	int esc;
	string entradas;

	do {
		resolve.exibeCampo();
		cout << endl << "MENU" << endl;
		cout << "1. Editar entrada" << endl << "2. Inserir componente" << endl << "3. Sair" << endl << "Escolha: ";
		cin >> esc;

		int i = 1, esc2;
		switch (esc)
		{
		case 1:
			cout << "Insira 8 bits: ";
			cin >> entradas;
			resolve.editaEntradas(entradas);
			break;
		case 2:
			for (const auto& componente : registroDeComponentes) {
				cout << i << ". " << componente->getNome() << endl;
				i++;
			}

			cout << "Escolha: ";
			cin >> esc2;

			if (esc2 <= registroDeComponentes.size()) {
				registroDeComponentes.at(esc2 - 1)->leEntradasESaidas();
				resolve.insere((Base*)registroDeComponentes.at(esc2 - 1));
			}
			break;
		default:
			break;
		}

		resolve.resolveTotal();
	} while (esc != 3);
}

ResolveApp::~ResolveApp()
{
	for (const auto& componente : registroDeComponentes) {
		delete componente;
	}
}
