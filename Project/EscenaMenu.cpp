#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

bool gameOver;
bool easy, medium, hard = false;
bool prevUp, prevLeft, prevRight, prevDown = false;
int width = 15;
int height = 10;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int lives = 3;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

//Menu Scene
void Menu()
{
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
			if (rapidxml::xml_node<> *pNode = pRoot->first_node("normal"))
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
//In this setup we will declare all variables according to the difficulty chosen by the player
void Setup()
{
	if (easy)
	{
		width *= 5;
		height *= 5;
	}
	else if (medium)
	{
		width *= 2;
		height *= 2;
	}
	else if (hard)
	{

	}
	//Initial direction of the snake is 0 = Stop for any difficulty
	dir = STOP;
	//Initial position of the snake in any map
	x = width / 2;
	y = height / 2;
	//Fruit initial spawn
	fruitX = rand() % width;
	fruitY = rand() % height;
	//Score counter
	score = 0;

}
//This function will take care of parameters changes upon deaths
void ResetDeath()
{
	nTail = 0;
	dir = STOP;
	prevUp = prevLeft = prevRight = prevDown = false; //This will allow the player to choose any direction again, even if it is the opposite of the last direction
	x = width / 2;
	y = height / 2;
	lives--;
}
//Game Loop
void Draw()
{
	//Clear background
	system("cls");

	//Here we build the arena for the snake. The walls that limit the arena and the space available. We also print the food and the snake head and body positions
	//Superior wall
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;
	//X, Y loops
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//Right wall
			if (j == 0)
				cout << "#";
			//Snake positions
			if (i == y && j == x)
				cout << "O";
			//Fruit positions
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else
			{
				//Printing the body of the snake
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}
				}
				//Printing blank spaces if there's no body to print
				if (!print)
					cout << " ";
			}
			//Right wall
			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}
	//Bottom Wall
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;
	//Score print
	cout << "Score: " << score << "	Lives: " << lives << endl;
}
//In this function we receive the keys the player presses using <conio.h> library. 
//We change the snake direction according to the keys pressed and in order to avoid the snake from going to an opposite direction 
//respect the current, we use booleans.
//We set the previous key pressed boolean to true in their case and set all the others to false apart from the opposite direction. 
//In the Logic() function we will see why.
void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
			if (!prevUp)
				dir = UP;
			prevUp = true;
			prevLeft = false;
			prevRight = false;
			break;
		case 'a':
			if (!prevLeft)
				dir = LEFT;
			prevUp = false;
			prevLeft = true;
			prevDown = false;
			break;
		case 'd':
			if (!prevRight)
				dir = RIGHT;
			prevUp = false;
			prevRight = true;
			prevDown = false;
			break;
		case 's':
			if (!prevDown)
				dir = DOWN;
			prevLeft = false;
			prevRight = false;
			prevDown = true;
			break;
		case 'x':
			gameOver = true; //x to quit the game
			break;
		case 'r':
			Menu(); //r to go back to the Menu, "reset game"
			break;
		}
	}
}
//In this function is where we calculate all positions according to what the player does in the game.
void Logic()
{
	//Here we take care of the body/tail of the snake
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	//The direction changes depending on the information received in the Input() function
	//If the previous key of any direction has been pressed, it's boolean would be set as "true", meaning that if you press
	//now the opposite key, the snake will continue with the same direction it had. 
	//However, if the player presses any other key than the opposite, the previous boolean will be "false", adapting the direction to the new one.
	//Example given: If you press LEFT (A), the snake goes left due to all booleans being initialised to "false" and the "prevLeft" boolean sets to "true".
	//Now, if you press RIGHT (D), "prevLeft" continues true since it will only change to "false" if you press UP (W) or DOWN (S), meaning that in the following 
	//switch the snake will continue as if you pressed LEFT or Nothing. 
	//"Espero haver-me explicat bé: Arnau Ruiz. xD Recomano mirar l'Input() conjuntament.
	switch (dir)
	{
	case LEFT:
		if (!prevRight)
			x--;
		else if (prevRight)
			x++;
		break;
	case RIGHT:
		if (!prevLeft)
			x++;
		else if (prevLeft)
			x--;
		break;
	case UP:
		if (!prevDown)
			y--;
		else if (prevDown)
			y++;
		break;
	case DOWN:
		if (!prevUp)
			y++;
		else if (prevUp)
			y--;
		break;
	default:
		break;
	}
	//Game ends if you crash with any wall
	if (x > width - 1 || x < 0 || y > height || y < 0)
	{
		if(lives <= 0)
			gameOver = true;
		else
		{
			ResetDeath();
		}
	}
	//Game ends if you crash with your tail or body
	for (int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)
		{
			if (lives <= 0)
				gameOver = true;
			else
			{
				ResetDeath();
			}
		}
	}
	//When the head of the snake touches a fruit, it respawns to a random location of the arena, the player gains points and increases it's tail size.
	if (x == fruitX && y == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}
void Guillem() {
	try {
		SDL_Event e;
		//INIT
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw SDL_GetError();
		const Uint8 imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		if (!(IMG_Init(imgFlags)&imgFlags)) throw IMG_GetError();
		//WINDOW
		const int WIDTH = 900, HEIGHT = 600;
		SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr) throw SDL_GetError();

		//RENDERER
		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		//SPRITES
		SDL_Texture *tileTexture = IMG_LoadTexture(renderer, "../res/gfx/Tile.png");
		SDL_Texture *tailTexture = IMG_LoadTexture(renderer, "../res/gfx/Tail.png");
		SDL_Texture *headTexture = IMG_LoadTexture(renderer, "../res/gfx/Head.png");
		SDL_Texture *bodyTexture = IMG_LoadTexture(renderer, "../res/gfx/Body.png");
		SDL_Texture *appleTexture = IMG_LoadTexture(renderer, "../res/gfx/Apple.png");
		SDL_Texture *wallTexture = IMG_LoadTexture(renderer, "../res/gfx/Wall.png");
		if (tileTexture == nullptr) throw SDL_GetError();
		SDL_Rect tileRect = { 0, 0, 50,50 };
		SDL_Rect tileRect2 = { 50, 0, 50, 50 };
		
		//SDL_Rect playerRect = { (WIDTH >> 1) - 100, (HEIGHT >> 1) - 100, 200, 200 };

		//GAME LOOP
		for (bool isRunning = true; isRunning;) {
			if (!SDL_PollEvent(&e)) if (e.type == SDL_QUIT) isRunning = false;
			//DRAW
			SDL_RenderCopy(renderer, headTexture, nullptr, &tileRect);
			SDL_RenderCopy(renderer, tileTexture, nullptr, &tileRect2);
			SDL_RenderPresent(renderer);
		}
		
		
		//DESTROY
		SDL_DestroyTexture(tileTexture);
		SDL_DestroyTexture(tailTexture);
		SDL_DestroyTexture(headTexture);
		SDL_DestroyTexture(bodyTexture);
		SDL_DestroyTexture(appleTexture);
		SDL_DestroyTexture(wallTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
	catch (const char *msg) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", msg);
	}
	IMG_Quit();
	SDL_Quit();
	
}
//We call here all functions, the order is very important!
int main(int, char*[])
{
	Menu();
	Setup();
	//You can play the game as long as the game isn't over
	while (!gameOver) {
		Draw();
		Input();
		Logic();
		//We use Sleep() function to control the speed of the game
		if (easy)
			Sleep(120);
		else if (medium)
			Sleep(80);
		else if (hard)
			Sleep(40);
	}
	IMG_Quit();
	SDL_Quit();
	
	Guillem();
	return 0;
}
/*
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
		
		for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
		{
				cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
		}
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
}*/