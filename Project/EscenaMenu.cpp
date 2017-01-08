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

int mouseX, mouseY;
SDL_Rect rect;
HWND hwnd;
bool menu = true;
bool dificulties, play = false;
bool gameOver;
bool easy, medium, hard = false;
bool prevUp, prevLeft, prevRight, prevDown = false;
int arenaX = 150, arenaY = 100;
int WIDTH = 900, HEIGHT = 600;
int x, y, DIR,fruitX, fruitY, score;
int tailX[100], tailY[100], direction[100];
int nTail;
int lives = 3;
POINT p;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;


SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

SDL_Texture *playTexture = IMG_LoadTexture(renderer, "../res/gfx/play.png");
SDL_Texture *exitTexture = IMG_LoadTexture(renderer, "../res/gfx/exit.png");
SDL_Texture *snakeTexture = IMG_LoadTexture(renderer, "../res/gfx/snake.png");
SDL_Texture *easyTexture = IMG_LoadTexture(renderer, "../res/gfx/easy.png");
SDL_Texture *mediumTexture = IMG_LoadTexture(renderer, "../res/gfx/medium.png");
SDL_Texture *hardTexture = IMG_LoadTexture(renderer, "../res/gfx/hard.png");

SDL_Texture *tileTexture = IMG_LoadTexture(renderer, "../res/gfx/Tile.png");
SDL_Texture *tailTexture = IMG_LoadTexture(renderer, "../res/gfx/Tail.png");
SDL_Texture *headTexture = IMG_LoadTexture(renderer, "../res/gfx/Head.png");
SDL_Texture *bodyTexture = IMG_LoadTexture(renderer, "../res/gfx/Body.png");
SDL_Texture *appleTexture = IMG_LoadTexture(renderer, "../res/gfx/Apple.png");
SDL_Texture *wallTexture = IMG_LoadTexture(renderer, "../res/gfx/Wall.png");

SDL_Rect playRect = { WIDTH / 2-50,HEIGHT / 2-50,100,100 };
SDL_Rect exitRect = { WIDTH/2-50,HEIGHT /2-50 +120,100,100 };

SDL_Rect snakeRect = { WIDTH / 2 - 200, 20,400,100 };
SDL_Rect easyRect = { WIDTH / 2 - 200, HEIGHT/2-300 / 2,100,100 };
SDL_Rect mediumRect = { WIDTH / 2 -50, HEIGHT/2- 300 / 2,100,100 };
SDL_Rect hardRect = { WIDTH / 2 + 100, HEIGHT/2- 300 / 2,100,100 };

SDL_Rect tileRect = { 0, 0, 50, 50 };
SDL_Rect wallRect = { 0, 0, 50, 50 };
SDL_Rect headRect = { 0, 0, 50, 50 };
SDL_Rect bodyRect = { 0, 0, 50, 50 };
SDL_Rect tailRect = { 0, 0, 50, 50 };
SDL_Rect appleRect = { 0, 0, 50, 50 };

//Menu Scene
void Menu()
{
	if (playTexture == NULL) cout << SDL_GetError();
	if (exitTexture == NULL) cout << SDL_GetError();
	
	rapidxml::xml_document<> doc;
	std::ifstream file("EscenaGame.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);
	rapidxml::xml_node<> *pRoot = doc.first_node();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		SDL_RenderCopy(renderer, playTexture, nullptr, &playRect);
		SDL_RenderCopy(renderer, exitTexture, nullptr, &exitRect);
		SDL_RenderCopy(renderer, snakeTexture, nullptr, &snakeRect);

		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			p.x = event.button.x;
			p.y = event.button.y;
			if (menu)
			{
				if (p.x <= playRect.x + playRect.w && p.x >= playRect.x && p.y <= playRect.y + playRect.h && p.y >= playRect.y && event.button.button == SDL_BUTTON_LEFT)
				{
					dificulties = true;
					cout << "Play" << endl;
					menu = false;
				}
				if (p.x <= exitRect.x + exitRect.w && p.x >= exitRect.x && p.y <= exitRect.y + exitRect.h && p.y >= exitRect.y)
				{
					cout << "Exit" << endl;
					gameOver = true;
				}
			}
			if (dificulties)
			{
				SDL_RenderCopy(renderer, easyTexture, nullptr, &easyRect);
				SDL_RenderCopy(renderer, mediumTexture, nullptr, &mediumRect);
				SDL_RenderCopy(renderer, hardTexture, nullptr, &hardRect);

				if (p.x <= easyRect.x + easyRect.w && p.x >= easyRect.x && p.y <= easyRect.y + easyRect.h && p.y >= easyRect.y)
				{
					cout << "EASY mode selected\n";
					if (rapidxml::xml_node<> *pNode = pRoot->first_node("easy"))
					{

						for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
						{
							cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
						}
					}
					gameOver = false;
					easy = true;
					dificulties = false;
				}
				if (p.x <= mediumRect.x + mediumRect.w && p.x >= mediumRect.x && p.y <= mediumRect.y + mediumRect.h && p.y >= mediumRect.y)
				{
					cout << "MEDIUM mode selected\n";
					if (rapidxml::xml_node<> *pNode = pRoot->first_node("medium"))
					{

						for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
						{
							cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
						}
					}
					gameOver = false;
					medium = true;
					dificulties = false;
				}
				if (p.x <= hardRect.x + hardRect.w && p.x >= hardRect.x && p.y <= hardRect.y + hardRect.h && p.y >= hardRect.y)
				{
					cout << "HARD mode selected\n";
					if (rapidxml::xml_node<> *pNode = pRoot->first_node("hard"))
					{

						for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
						{
							cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
						}
					}
					gameOver = false;
					hard = true;
					dificulties = false;
				}
			}
		}
		
	}
	SDL_RenderPresent(renderer);

	/*
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
	}*/
}

//In this setup we will declare all variables according to the difficulty chosen by the player
void Setup()
{

	tileRect = { 0, 0, 10, 10 };


	if (easy)
	{
		arenaX *= 5;
		arenaY *= 5;
	}
	else if (medium)
	{
		arenaX *= 2;
		arenaY *= 2;
	}
	else if (hard)
	{

	}
	//Initial direction of the snake is 0 = Stop for any diafficulty
	dir = STOP;
	//Initial position of the snake in any map
	x = 50;
	y = 50;
	//Fruit initial spawn
	fruitX = (1 + rand() % (arenaX/10)) * 10;
	fruitY = (1 + rand() % (arenaY/10)) * 10;
	//Score counter
	score = 0;
	//Score & lives print
	cout << "Score: " << score << "	Lives: " << lives << endl;

	rect.x = WIDTH / 2;
	rect.y = HEIGHT / 2;
	rect.w = arenaX;
	rect.h = arenaY;
}

//This function will take care of parameters changes upon deaths
void ResetDeath()
{
	nTail = 0;
	dir = STOP;
	prevUp = prevLeft = prevRight = prevDown = false; //This will allow the player to choose any direction again, even if it is the opposite of the last direction
	x = 50;
	y = 50;
	lives--;
	//Score & lives print
	cout << "Score: " << score << "	Lives: " << lives << endl;
}
//Game Loop
void Draw()
{
	//Here we build the arena for the snake. The walls that limit the arena and the space available. We also print the food and the snake head and body positions
	//X, Y loops
	
			//DRAW
			for (int i = 0; i < arenaY; i += 10) {
				for (int j = 0; j < arenaX; j += 10) {
					tileRect.x = j;
					tileRect.y = i;
					//Tiles
					if (i == 0) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else if (j == 0) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else if (j == arenaX - 10) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else if (i == arenaY - 10) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else { SDL_RenderCopy(renderer, tileTexture, nullptr, &tileRect); }
					//Snake
					if (i==y && j == x){ SDL_RenderCopy(renderer, headTexture, nullptr, &tileRect); }
					//apple //if(dir == UP) etc..
					if (i == fruitY && j == fruitX) { SDL_RenderCopyEx(renderer, appleTexture, nullptr, &tileRect, 90, nullptr, SDL_FLIP_NONE); }
				}
			}
			SDL_RenderPresent(renderer);
			//cout << x << " , " << y << endl;
			/*
			tileRect.x = j;
			tileRect.y = i;


			//TopWall
			if(i==0){SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect);}
			//Left wall
			if (j == 0){ SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
			//Right walls
			if (j == arenaX - 50) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
			//Bottom Wall
			if (i == arenaY-50) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
			//Snake positions
			if (i == y && j == x){ SDL_RenderCopy(renderer, headTexture, nullptr, &tileRect); }
				
			//Fruit positions
			else if (i == fruitY && j == fruitX){ SDL_RenderCopy(renderer, appleTexture, nullptr, &tileRect); }
				
			else
			{
				//Printing the body of the snake
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						SDL_RenderCopy(renderer, bodyTexture, nullptr, &tileRect);
						print = true;
					}
				}
				//Printing blank spaces if there's no body to print
				if (!print){ SDL_RenderCopy(renderer, tileTexture, nullptr, &tileRect); }
					
			}*/
	
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
		}
	}
}
//In this function is where we calculate all positions according to what the player does in the game.
void Logic()
{
	//Here we take care of the body/tail of the snake
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prevDirection = direction[0];
	int prev2X, prev2Y, prev2Direction;
	tailX[0] = x;
	tailY[0] = y;
	direction[0] = DIR;

	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		prev2Direction = direction[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		direction[i] = prevDirection;
		prevX = prev2X;
		prevY = prev2Y;
		prevDirection = prev2Direction;
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
		if (!prevRight){
			x -= 10;
			DIR = 270;
		}
			
		else if (prevRight){ 
			x += 10; 
			DIR = 90;
		}
		break;
	case RIGHT:
		if (!prevLeft){
			x += 10;
			DIR = 90;
		}
			
		else if (prevLeft){
			x -= 10;
			DIR = 270;
		}
		break;
	case UP:
		if (!prevDown){
			y -= 10;
			DIR = 0;
		}
		else if (prevDown){
			y += 10;
			DIR = 180;
		}
		break;
	case DOWN:
		if (!prevUp) {
			y += 10;
			DIR = 180;
		}
		else if (prevUp) {
			y -= 10;
			DIR = 0;
		}
		break;
	default:
		break;
	}
	//Game ends if you crash with any wall
	if (x > arenaX - 10 || x < 0 || y > arenaY -10 || y < 0)
	{
		if (lives <= 0)
			gameOver = true;
		else
		{
			ResetDeath();
		}
	}
	//Game ends if you crash with your tail or body
	for (int i = 0; i < nTail; i++)
	{
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
		fruitX = (1 + rand() % (arenaX / 10)) * 10;
		fruitY = (1 + rand() % (arenaY / 10)) * 10;
		nTail++;
		//Score & lives print
		cout << "Score: " << score << "	Lives: " << lives << endl;
	}
}
/*
void Sprites() {
	try {
		SDL_Event e;
		//INIT
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw SDL_GetError();
		const Uint8 imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		if (!(IMG_Init(imgFlags)&imgFlags)) throw IMG_GetError();
		//WINDOW
		 int WIDTH = 900, HEIGHT = 600;
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
		//if (tileTexture == nullptr) throw SDL_GetError();
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
	
}*/

//We call here all functions, the order is very important!
int main(int, char*[])
{
	while (((menu || dificulties) && !gameOver))
	{
		Menu();
	}
	Setup();
	//You can play the game as long as the game isn't over
	while (!gameOver) { //(play && !gameOver)
		Draw();
		Input();
		Logic();
		//We use Sleep() function to control the speed of the game
	/*	if (easy)
			Sleep(120);
		else if (medium)
			Sleep(80);
		else if (hard)
			Sleep(40);*/
	}
	SDL_DestroyTexture(playTexture);
	SDL_DestroyTexture(exitTexture);
	SDL_DestroyTexture(easyTexture);
	SDL_DestroyTexture(mediumTexture);
	SDL_DestroyTexture(hardTexture);
	SDL_DestroyTexture(snakeTexture);
	SDL_DestroyTexture(tileTexture);
	SDL_DestroyTexture(tailTexture);
	SDL_DestroyTexture(headTexture);
	SDL_DestroyTexture(bodyTexture);
	SDL_DestroyTexture(appleTexture);
	SDL_DestroyTexture(wallTexture);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	
	return 0;
}