#pragma once

//Menu Scene
void Menu()
{
	play = IMG_LoadTexture(renderer, "../res/gfx/play.png");
	exit = IMG_LoadTexture(renderer, "../res/gfx/exit.png");

	rapidxml::xml_document<> doc;
	std::ifstream file("EscenaGame.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);
	rapidxml::xml_node<> *pRoot = doc.first_node();
	//Welcome message
	cout << "WELCOME TO SNAKE" << endl;
	cout << endl << "Do you want to play?" << endl;
	cout << endl << "PLAY!	Press < 1 >" << endl;
	cout << endl << "EXIT	Press < 2 >" << endl << endl;
	//Character Input, player's choice
	int play;
	cin >> play;
	//Difficulty choice Menu
	if (play == 1)
	{
		cout << endl;
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
			if (rapidxml::xml_node<> *pNode = pRoot->first_node("easy"))
			{

				for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
				{
					cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
				}
			}
			easy = true;
			gameOver = false;
			break;
		case 2:
			cout << "NORMAL mode selected\n";
			if (rapidxml::xml_node<> *pNode = pRoot->first_node("medium"))
			{
				for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
				{
					cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
				}
			}
			medium = true;
			gameOver = false;
			break;
		case 3:
			cout << "HARD mode selected\n";
			if (rapidxml::xml_node<> *pNode = pRoot->first_node("hard"))
			{
				for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
				{
					cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
				}
			}
			hard = true;
			gameOver = false;
			break;
		default:
			cout << "\nError: Invalid key\n\n";
			gameOver = true;
			break;
		}
	}
	//Exit game, player's choice
	else if (play == 2)
	{
		gameOver = true;
	}
	//Pressing an invalid key will exit the game too
	else
	{
		cout << "\nError: Invalid key\n\n";
		gameOver = true;
	}
}
