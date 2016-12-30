#include <string.h>
#include <stdio.h>
#include <iostream>
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
	std::cout << "Nombre de la raiz" << doc.first_node()->name() << "\n";
	rapidxml::xml_node<> *pRoot = doc.first_node();
	for (rapidxml::xml_node<> *pNode = pRoot->first_node("book"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> *pAttr = pNode->first_attribute();		cout << pAttr->name() << '-' << pAttr->value() << '\n';
	}

	int dif;
	cout << "Please select your game dificulty:\n\n";
	cout << "	Press < 1 > for EASY mode\n\n";
	cout << "	Press < 2 > for NORMAL mode\n\n";
	cout << "	Press < 3 > for HARD mode\n\n";
	cin >> dif;
	cout << "\n";
	
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