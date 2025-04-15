#include "ResolveApp.h"

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
		cout << "1. Editar entrada" << endl << "2. Inserir componente" << endl << "3. Editar componente" << endl << "4. Remover componente" << endl << "5. Salvar" << endl << "6. Carregar" << endl << "7. Sair" << endl << "Escolha: ";
		cin >> esc;

		int i = 1, esc2;
		switch (esc)
		{
		// Edição da entrada de bits
		case 1:
			cout << "               01234567" << endl;
			cout << "Insira 8 bits: ";
			cin >> entradas;

			if (entradas.size() != 8) break;

			resolve.editaEntradas(entradas);
			break;

		// Inserção de componentes
		case 2:
			for (const auto& componente : registroDeComponentes) {
				cout << i << ". " << componente->getNome() << endl;
				i++;
			}
			cout << i << ". voltar" << endl;

			cout << "Escolha: ";
			cin >> esc2;
			
			if (esc2 >= 1 && esc <= i) {
				if (esc2 <= registroDeComponentes.size()) {
					if(registroDeComponentes.at(esc2 - 1)->leEntradasESaidas()) resolve.insere(dynamic_cast<Base*>(registroDeComponentes.at(esc2 - 1)));
				}
			}

			break;

		// Edição dos componentes
		case 3: {
			vector<Base*> comp = resolve.getComponentes();
			if (comp.size() != 0) {
				int i = 1;
				for (const auto& componente : comp) {
					cout << i++ << ". " << componente->getNome() << " ";
					for (const auto& indices : componente->getEntradas()) {
						cout << indices << " ";
					}
					for (const auto& indices : componente->getSaidas()) {
						cout << indices << " ";
					}
					cout << endl;
				}
				cout << i << ". voltar" << endl;
				cout << "escolha: ";
				cin >> esc2;

				if (esc2 >= 1 && esc2 < i) {
					resolve.removeComponente(esc2); // Apaga componente escolhido
					for (int j = 0; j < registroDeComponentes.size(); j++) {
						if (registroDeComponentes.at(j)->getNome() == comp.at(esc2 - 1)->getNome()) {
							if (registroDeComponentes.at(j)->leEntradasESaidas()) {	// Reinicia a inserção
								resolve.insere(dynamic_cast<Base*>(registroDeComponentes.at(j)));
								break;
							}
						}
					}
				}
			}
			break;
		}

		// Remove componente
		case 4: {
			vector<Base*> comp = resolve.getComponentes();
			if (comp.size() == 0) break;
			int i = 1, esc;
			for (const auto& componente : comp) {
				cout << i++ << ". " << componente->getNome() << " ";
				for (const auto& indices : componente->getEntradas()) {
					cout << indices << " ";
				}
				for (const auto& indices : componente->getSaidas()) {
					cout << indices << " ";
				}
				cout << endl;
			}
			cout << i << ". voltar" << endl;
			cout << "escolha: ";
			cin >> esc;

			if (esc >= 1 && esc <= i) resolve.removeComponente(esc);
			break;
		}

		// Salva estado atual de todos componentes
		case 5: {
			string nomeArquivo = "";
			cout << "Escolha o nome do arquivo para ser salvo: ";
			cin >> nomeArquivo;

			resolve.salvaComponentes(nomeArquivo);
			break;
		}

		// Carrega arquivo desejado
		case 6: {
			string arquivoNome;
			cout << "Insira o nome do arquivo que deseja carregar: ";
			cin >> arquivoNome;

			ifstream arquivo(arquivoNome);

			if (arquivo.is_open()) {
				resolve.reinicia();
				string linha;

				while (getline(arquivo, linha)) {
					istringstream palavra(linha);
					string cursor;
					vector<string> entradasBuff, saidasBuff;

					palavra >> cursor;

					for (int i = 0; i < registroDeComponentes.size(); i++) {
						if (registroDeComponentes.at(i)->getNome() == cursor) {
							for (int j = 0; j < registroDeComponentes.at(i)->getNentradas(); j++) {
								palavra >> cursor;

								if (cursor.size() != 2) break;

								if (cursor[0] != 's' && cursor[0] != 'e') {
									if (cursor[0] - '0' < 0 || cursor[0] - '0' > 7) break;
								}

								if (cursor[1] - '0' < 0 || cursor[1] - '0' > 7) break;

								entradasBuff.push_back(cursor);
							}
							while (palavra >> cursor) {
								if (cursor.size() != 2) break;

								if (cursor[0] != 's' && cursor[0] != 'e') {
									if (cursor[0] - '0' < 0 || cursor[0] - '0' > 7) break;
								}

								if (cursor[1] - '0' < 0 || cursor[1] - '0' > 7) break;

								saidasBuff.push_back(cursor);
							}

							registroDeComponentes.at(i)->carregaComponentes(entradasBuff, saidasBuff);

							resolve.insere(dynamic_cast<Base*>(registroDeComponentes.at(i)));
							break;
						}
					}
				}
			}
			else {
				cout << "ERRO::RESOLVEAPP::MAINDOAPP::CASE6: Erro ao abrir o arquivo, verifique se ele existe" << endl;
				system("pause");
			}
			break;
		}
		default:
			break;
		}

		// Calcula todos os componenntes após cada escolha
		resolve.resolveTotal();
	}while (esc != 7);
}

ResolveApp::~ResolveApp()
{
	for (const auto& componente : registroDeComponentes) {
		delete componente;
	}
}