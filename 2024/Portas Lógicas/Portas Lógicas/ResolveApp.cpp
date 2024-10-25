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
		system("cls");
		resolve.exibeCampo();
		cout << endl << "MENU" << endl;
		cout << "1. Editar entrada" << endl << "2. Inserir componente" << endl << "3. Editar componente" << endl << "4. Sair" << endl << "Escolha: ";
		cin >> esc;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		int i = 1, esc2;
		switch (esc)
		{
		case 1:
			cout << "               01234567" << endl;
			cout << "Insira 8 bits: ";
			cin >> entradas;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (entradas.size() != 8) break;

			resolve.editaEntradas(entradas);
			break;
		case 2:
			for (const auto& componente : registroDeComponentes) {
				cout << i << ". " << componente->getNome() << endl;
				i++;
			}

			cout << "Escolha: ";
			cin >> esc2;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (esc2 <= registroDeComponentes.size()) {
				registroDeComponentes.at(esc2 - 1)->leEntradasESaidas();
				resolve.insere(dynamic_cast<Base*>(registroDeComponentes.at(esc2 - 1)));
			}
			break;
		case 3:
			resolve.editaComponente();
			break;
		default:
			break;
		}
		resolve.resolveTotal();
	} while (esc != 4);
}

ResolveApp::~ResolveApp()
{
	for (const auto& componente : registroDeComponentes) {
		delete componente;
	}
}