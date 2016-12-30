#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;


int main() {
	rapidxml::xml_document<> doc;
	std::ifstream file("EscenaGame.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);
	std::cout << "Nombre de la raiz\n" << doc.first_node()->name() << "\n";
	rapidxml::xml_node<> *pRoot = doc.first_node();
	for (rapidxml::xml_node<> *pNode = pRoot->first_node("dificulty"); pNode; pNode = pNode->next_sibling())
	{
		cout << pNode->name() << ':' << '\n';		for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())		{			cout << pNodeI->name() << ':' << pNodeI->value() << '\n';		}
	}

	//Inicializamos variable que introducirá el usuario
	int dif;
	//Imprimimos en consola texto
	cout << "Please select your game dificulty:\n\n";
	cout << "	Press < 1 > for EASY mode\n\n";
	cout << "	Press < 2 > for NORMAL mode\n\n";
	cout << "	Press < 3 > for HARD mode\n\n";

	//Esperamos a que el usuario pulse una tecla
	cin >> dif;
	cout << "\n";

	//Dependiendo de la tecla puslada se seleccionará un modo o dará error
	switch (dif) {
	case 1:
		cout << "EASY mode selected\n";
		break;
	case 2:
		cout << "NORMAL mode selected\n";
		break;
	case 3:
		cout << "HARD mode selected\n";
		break;
	default:
		cout << "Error. Press 1, 2 or 3 to select a game mode.\n";
		break;
	}

	return 0;
}