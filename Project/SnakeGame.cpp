#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

int mouseX, mouseY;
HWND hwnd;
bool menu = true;
bool dificulties, play, exitBool;
bool gameOver;
bool easy, medium, hard;
int arenaX, arenaY;
const int WIDTH = 1200, HEIGHT = 640;
SDL_Rect rect{ (WIDTH - arenaX) / 2, (HEIGHT - arenaY) / 2, arenaX, arenaY };
int x, y, dirAngle, fruitX, fruitY, score, fruitCounter;
int tailX[100], tailY[100], direction[100];
int nTail;
int lives;
int level, messageLevel;
POINT p;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
string slotsS, timeLevelS, speedS, foodS, foodIncreaseS;
int slots, timeLevel, speed, food, foodIncrease, threshold;
int countdown = 0;
double timer;
double start;
int game;

SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

SDL_Texture *playTexture = IMG_LoadTexture(renderer, "../res/gfx/play.png");
SDL_Texture *exitTexture = IMG_LoadTexture(renderer, "../res/gfx/exit.png");
SDL_Texture *snakeTexture = IMG_LoadTexture(renderer, "../res/gfx/snake.png");
SDL_Texture *easyTexture = IMG_LoadTexture(renderer, "../res/gfx/easy.png");
SDL_Texture *mediumTexture = IMG_LoadTexture(renderer, "../res/gfx/medium.png");
SDL_Texture *hardTexture = IMG_LoadTexture(renderer, "../res/gfx/hard.png");

SDL_Texture *snakeLiveTexture = IMG_LoadTexture(renderer, "../res/gfx/snakeLive.png");
SDL_Texture *snakeLive2Texture = IMG_LoadTexture(renderer, "../res/gfx/snakeLive2.png");

SDL_Texture *bgTexture = IMG_LoadTexture(renderer, "../res/gfx/Black.png");

SDL_Texture *tileTexture = IMG_LoadTexture(renderer, "../res/gfx/Tile.png");
SDL_Texture *tailTexture = IMG_LoadTexture(renderer, "../res/gfx/Tail.png");
SDL_Texture *headTexture = IMG_LoadTexture(renderer, "../res/gfx/Head.png");
SDL_Texture *bodyTexture = IMG_LoadTexture(renderer, "../res/gfx/Body2.png");
SDL_Texture *appleTexture = IMG_LoadTexture(renderer, "../res/gfx/Apple.png");
SDL_Texture *wallTexture = IMG_LoadTexture(renderer, "../res/gfx/Wall.png");

SDL_Texture *timeBarTexture = IMG_LoadTexture(renderer, "../res/gfx/TimeBar.png");
SDL_Texture *timeBarTexture2 = IMG_LoadTexture(renderer, "../res/gfx/TimeBar2.png");
SDL_Texture *clockTexture = IMG_LoadTexture(renderer, "../res/gfx/clock.png");
SDL_Texture *levelTexture = IMG_LoadTexture(renderer, "../res/gfx/level.png");
SDL_Texture *levelTexture2 = IMG_LoadTexture(renderer, "../res/gfx/level2.png");

SDL_Rect playRect = { WIDTH / 2-50,HEIGHT / 2,100,100 };
SDL_Rect exitRect = { WIDTH/2-50,HEIGHT /2 +120,100,100 };

SDL_Rect bgRect = {0,0,WIDTH,HEIGHT};

SDL_Rect snakeRect = { WIDTH / 2 - 200, 20,400,100 };
SDL_Rect easyRect = { WIDTH / 2 - 230, HEIGHT/2-300 / 2,125,125 };
SDL_Rect mediumRect = { WIDTH / 2 -65, HEIGHT/2- 300 / 2,125,125 };
SDL_Rect hardRect = { WIDTH / 2 + 100, HEIGHT/2- 300 / 2,125,125 };

SDL_Rect snakeLiveRect1;
SDL_Rect snakeLiveRect2;
SDL_Rect snakeLiveRect3;
SDL_Rect timeBarRect, timeBarRect2, clockRect, levelRect, levelRect2;

SDL_Rect tileRect = { 0, 0, 10, 10 };
Uint32 initiate;
Mix_Music* music;

//Menu Scene
void Menu()
{	
	//Rendering Menu sprites
	SDL_RenderCopy(renderer, playTexture, nullptr, &playRect);
	SDL_RenderCopy(renderer, exitTexture, nullptr, &exitRect);
	SDL_RenderCopy(renderer, snakeTexture, nullptr, &snakeRect);

	rapidxml::xml_document<> doc;
	std::ifstream file("GameScene.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);
	rapidxml::xml_node<> *pRoot = doc.first_node();
	SDL_Event event;

	dificulties, play, exitBool = false;
	easy, medium, hard = false;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		//Click check on mouse position
		case SDL_MOUSEBUTTONDOWN:
			p.x = event.button.x;
			p.y = event.button.y;
			if (menu)
			{	//Play Button click check
				if (p.x <= playRect.x + playRect.w && p.x >= playRect.x && p.y <= playRect.y + playRect.h && p.y >= playRect.y && event.button.button == SDL_BUTTON_LEFT)
				{
					dificulties = true;
					cout << "Play" << endl;
					menu = false;
				}
			}
			if (dificulties)
			{
				SDL_RenderCopy(renderer, easyTexture, nullptr, &easyRect);
				SDL_RenderCopy(renderer, mediumTexture, nullptr, &mediumRect);
				SDL_RenderCopy(renderer, hardTexture, nullptr, &hardRect);
				//Easy Button click check
				if (p.x <= easyRect.x + easyRect.w && p.x >= easyRect.x && p.y <= easyRect.y + easyRect.h && p.y >= easyRect.y)
				{
					cout << "EASY mode selected\n";
					//Copying values from XML file to .cpp variables
					if (rapidxml::xml_node<> *pNode = pRoot->first_node("easy"))
					{
						for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
						{
							string name = pNodeI->name();  //cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
							if (name == "Slots") { slotsS = pNodeI->value(); cout << slotsS << endl; }
							if (name == "Time") { timeLevelS = pNodeI->value(); string::size_type sz;  timeLevel = stoi(timeLevelS, &sz); cout << "Time: " << timeLevel << endl; }
							if (name == "Speed") { speedS = pNodeI->value(); string::size_type sz;  speed = stoi(speedS, &sz); cout << "Speed: " << speed << endl;}
							if (name == "Food") { foodS = pNodeI->value();  string::size_type sz;  food = stoi(foodS, &sz); cout << "Food: " << food << endl;}
							if (name == "FoodIncrease") { foodIncreaseS = pNodeI->value();  string::size_type sz;  foodIncrease = stoi(foodIncreaseS, &sz); cout << "Food increase: " << foodIncrease << endl;}
						}
					}
					gameOver = false;
					easy = true;
					dificulties = false;
				}
				//Medium Button click check
				if (p.x <= mediumRect.x + mediumRect.w && p.x >= mediumRect.x && p.y <= mediumRect.y + mediumRect.h && p.y >= mediumRect.y)
				{	
					cout << "MEDIUM mode selected\n";
					//Copying values from XML file to .cpp variables
					if (rapidxml::xml_node<> *pNode = pRoot->first_node("medium"))
					{

						for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
						{
							string name = pNodeI->name();  //cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
							if (name == "Slots") { slotsS = pNodeI->value(); cout << slotsS << endl; }
							if (name == "Time") { timeLevelS = pNodeI->value(); string::size_type sz;  timeLevel = stoi(timeLevelS, &sz); cout << "Time: " << timeLevel << endl; }
							if (name == "Speed") { speedS = pNodeI->value(); string::size_type sz;  speed = stoi(speedS, &sz); cout << "Speed: " << speed << endl; }
							if (name == "Food") { foodS = pNodeI->value();  string::size_type sz;  food = stoi(foodS, &sz); cout << "Food: " << food << endl; }
							if (name == "FoodIncrease") { foodIncreaseS = pNodeI->value();  string::size_type sz;  foodIncrease = stoi(foodIncreaseS, &sz); cout << "Food increase: " << foodIncrease << endl; }
						}
					}
					gameOver = false;
					medium = true;
					dificulties = false;
				}
				//Hard Button click check
				if (p.x <= hardRect.x + hardRect.w && p.x >= hardRect.x && p.y <= hardRect.y + hardRect.h && p.y >= hardRect.y)
				{
					cout << "HARD mode selected\n";
					//Copying values from XML file to .cpp variables
					if (rapidxml::xml_node<> *pNode = pRoot->first_node("hard"))
					{

						for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI = pNodeI->next_sibling())
						{
							string name = pNodeI->name();  //cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
							if (name == "Slots") { slotsS = pNodeI->value(); cout << slotsS << endl; }
							if (name == "Time") { timeLevelS = pNodeI->value(); string::size_type sz;  timeLevel = stoi(timeLevelS, &sz); cout << "Time: " << timeLevel << endl; }
							if (name == "Speed") { speedS = pNodeI->value(); string::size_type sz;  speed = stoi(speedS, &sz); cout << "Speed: " << speed << endl; }
							if (name == "Food") { foodS = pNodeI->value();  string::size_type sz;  food = stoi(foodS, &sz); cout << "Food: " << food << endl; }
							if (name == "FoodIncrease") { foodIncreaseS = pNodeI->value();  string::size_type sz;  foodIncrease = stoi(foodIncreaseS, &sz); cout << "Food increase: " << foodIncrease << endl; }
						}
					}
					gameOver = false;
					hard = true;
					dificulties = false;
				}
			}
			//Exit Button click check
			if (p.x <= exitRect.x + exitRect.w && p.x >= exitRect.x && p.y <= exitRect.y + exitRect.h && p.y >= exitRect.y)
			{
				cout << "Exit" << endl;
				dificulties = false;
				gameOver = true;
				menu = false;
				game++;
			}
		}
		
	}
	SDL_RenderPresent(renderer);
}

//In this setup we will declare all variables according to the difficulty chosen by the player
void Setup()
{
	arenaX = 220;
	arenaY = 120;
	lives = 3;
	level = 1;
	start = SDL_GetTicks();
	threshold = 0;

	if (easy)
	{
		//Arena size
		arenaX *= 5;
		arenaY *= 5;
		//Initial position of the snake in any map
		x = arenaX / 10 / 3 * 10, y = arenaY / 1.5;
	}
	else if (medium)
	{
		//Arena size
		arenaX *= 2;
		arenaY *= 2;
		//Initial position of the snake in any map
		x = arenaX / 10 / 3 * 10, y = arenaY / 1.5;
	}
	else if (hard)
	{
		//Initial position of the snake in any map
		x = arenaX / 10 / 3 * 10, y = arenaY / 1.5;
	}
	//Initial direction of the snake is 0 = Stop for any diafficulty
	dir = STOP;

	//Fruit initial spawn
	//980 +10
	fruitX = (1 + rand() % ((arenaX-20) / 10)) * 10;
	fruitY = (1 + rand() % ((arenaY-20) / 10)) * 10;
	//Fruit counter score multiplier
	fruitCounter = 1;
	//Score counter
	score = 0;
	//Score & lives print
	cout << "Score: " << score << "	Lives: " << lives << endl;

	//Lives position according to arena size
	snakeLiveRect1 = { arenaX + 20, 20, 50, 50 };
	snakeLiveRect2 = { arenaX + 20, 60, 50, 50 };
	snakeLiveRect3 = { arenaX + 20, 100, 50, 50 };
}
//This function is in charge of changing to the following level
void NextLevel()
{
	//If the number of fruits eaten in one level are equal to the number of food available (considering the food increase as well), advance to next level
	if(fruitCounter == 1+food+threshold) 
	{ 
		nTail = 0;
		dir = STOP;
		dirAngle = STOP;
		x = arenaX / 10 / 3 * 10;
		y = arenaY / 1.5;
		level++;
		start = timer;
		fruitCounter = 1;
		threshold += foodIncrease;
		messageLevel = countdown;
		if (level < 8)
		{
			speed = speed - (level*1.75); //The speed of the game will change according to level
			cout << "Speed increased" << endl;
		}
		else if (level >= 8)
		{
			cout << "Maximum speed!" << endl;
		}
		cout << "Level: " << level << endl;
		if (messageLevel < countdown + 3) {
			levelRect = { arenaX / 2 -100, arenaY / 2 -50, 200, 100 };	//levelRect = { 60 + timeLevel, arenaY + 10, 100, 50 };
			SDL_RenderCopy(renderer, levelTexture, nullptr, &levelRect);
			SDL_RenderPresent(renderer);
			Sleep(1000);
			}
	}

}
//This function will take care of parameters changes upon deaths
void ResetDeath()
{
	if (lives <= 0) { 
		gameOver = true; 
	}
	else 
	{
		nTail = 0;
		dir = STOP;
		dirAngle = STOP;
		x = arenaX / 10 / 3 * 10;
		y = arenaY / 1.5;
		lives--;
		//Score & lives print
		cout << "Score: " << score << "	Lives: " << lives << endl;
	}
}
void Timer()
{
	//countdown = new DATE;
	countdown = timeLevel;
	timer = SDL_GetTicks();
	countdown -= (timer - start)/1000;
	cout << countdown << endl;
	if (countdown <= 0)
	{
		cout << "You run out of time!" << endl;
		start = timer;
		ResetDeath();
	}
}
//Game Loop
void Draw()
{
	//Here we build the arena for the snake. The walls that limit the arena and the space available. We also print the food and the snake head and body positions
	//X, Y loops

	//Lives sprites
	if (lives == 3) { SDL_RenderCopy(renderer, snakeLiveTexture, nullptr, &snakeLiveRect1); SDL_RenderCopy(renderer, snakeLiveTexture, nullptr, &snakeLiveRect2); 
	SDL_RenderCopy(renderer, snakeLiveTexture, nullptr, &snakeLiveRect3);
	}
	else if (lives == 2) {
		SDL_RenderCopy(renderer, snakeLive2Texture, nullptr, &snakeLiveRect1); SDL_RenderCopy(renderer, snakeLiveTexture, nullptr, &snakeLiveRect2);
		SDL_RenderCopy(renderer, snakeLiveTexture, nullptr, &snakeLiveRect3);
	}
	else if (lives == 1) {
		SDL_RenderCopy(renderer, snakeLive2Texture, nullptr, &snakeLiveRect1); SDL_RenderCopy(renderer, snakeLive2Texture, nullptr, &snakeLiveRect2);
		SDL_RenderCopy(renderer, snakeLiveTexture, nullptr, &snakeLiveRect3);
	}
	else if (lives == 0) {
		SDL_RenderCopy(renderer, snakeLive2Texture, nullptr, &snakeLiveRect1); SDL_RenderCopy(renderer, snakeLive2Texture, nullptr, &snakeLiveRect2);
		SDL_RenderCopy(renderer, snakeLive2Texture, nullptr, &snakeLiveRect3);
	}

	//Time bar position according to arena size
	timeBarRect = { 60, arenaY+10, timeLevel, 20 };
	timeBarRect2 = { 60, arenaY+10, countdown, 20 };
	clockRect = { 10, arenaY + 5, 30, 30 };
	SDL_RenderCopy(renderer, timeBarTexture2, nullptr, &timeBarRect);
	SDL_RenderCopy(renderer, timeBarTexture, nullptr, &timeBarRect2);
	SDL_RenderCopy(renderer, clockTexture, nullptr, &clockRect);

	//DRAW
	for (int i = 0; i < arenaY; i += 10) 
	{
		for (int j = 0; j < arenaX; j += 10) 
		{
			//Tiles
			tileRect.x = j;
			tileRect.y = i;
			//Snake
			if (i == y && j == x) { SDL_RenderCopyEx(renderer, headTexture, nullptr, &tileRect, dirAngle, nullptr, SDL_FLIP_NONE);}
			//Apple
			else if (i == fruitY && j == fruitX) { SDL_RenderCopy(renderer, appleTexture, nullptr, &tileRect); }
			else
			{
				//Printing the body of the snake
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						SDL_RenderCopyEx(renderer, bodyTexture, nullptr, &tileRect, dirAngle, nullptr, SDL_FLIP_NONE);
						print = true;
					}
				}
				//Printing blank spaces if there's no body to print
				if (!print) {
					SDL_RenderCopy(renderer, tileTexture, nullptr, &tileRect);
					//Printing walls
					if (i == 0) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else if (j == 0) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else if (j == arenaX - 10) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					else if (i == arenaY - 10) { SDL_RenderCopy(renderer, wallTexture, nullptr, &tileRect); }
					//else { SDL_RenderCopy(renderer, tileTexture, nullptr, &tileRect); }
				}
			}
		}
	}	
	SDL_RenderPresent(renderer);
}
//In this function we receive the keys the player presses
//We change the snake direction according to the keys pressed. In order to avoid the snake from going to an opposite direction 
//the current direction will be used as a condition. The game can be ended at any time while playing by pressing the "Esc" key.
void Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			/* Look for a keypress */
		case SDL_KEYDOWN:
			/* Check the SDLKey values and move change the coords */
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				if (dir != RIGHT)
					dir = LEFT;
				break;
			case SDLK_RIGHT:
				if (dir != LEFT)
					dir = RIGHT;
				break;
			case SDLK_UP:
				if (dir != DOWN)
					dir = UP;
				break;
			case SDLK_DOWN:
				if (dir != UP)
					dir = DOWN;
				break;
			case SDLK_ESCAPE:
				gameOver = true;
				break;
			default:
				break;
			}
		}
	}
}
//In this function is where we calculate all positions according to what the player does in the game.
void Logic()
{
	//Here we take care of the body/tail of the snake
	int prevX = tailX[0];
	int prevY = tailY[0];
	//int prevDirection = direction[0];
	int prev2X, prev2Y, prev2Direction;
	tailX[0] = x;
	tailY[0] = y;
	direction[0] = dirAngle;

	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		//prev2Direction = direction[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		//direction[i] = prevDirection;
		prevX = prev2X;
		prevY = prev2Y;
		//prevDirection = prev2Direction;
	}
	//The direction changes depending on the information received in the Input() function
	switch (dir)
	{
	case LEFT:
			x -= 10;
			dirAngle = 270;
		break;
	case RIGHT:
			x += 10;
			dirAngle = 90;
		break;
	case UP:
			y -= 10;
			dirAngle = 0;
		break;
	case DOWN:
			y += 10;
			dirAngle = 180;
		break;
	default:
		break;
	}
	//Game ends if you crash with any wall
	if (x >= arenaX - 10 || x < 10 || y >= arenaY - 10 || y < 10)
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
	if (x == fruitX && y == fruitY)	//if(x <= fruitX + tileRect.w && x >= fruitX && y <= fruitY + tileRect.h && y >= fruitY)	
	{
		score += 100 * fruitCounter;
		fruitX = (1 + rand() % ((arenaX - 20) / 10)) * 10;
		fruitY = (1 + rand() % ((arenaY - 20) / 10)) * 10;
		fruitCounter++;
		nTail++;
		//Score & lives print
		cout << "Score: " << score << "	Lives: " << lives << endl;
	}
}
/*
std::string Encript(std::string myString) {
	string::iterator it;
	std::string text = ("");
	int loop = 0, num = 0;
	for (it = myString.begin(); it < myString.end(); ++it, loop++) {
		switch (myString[loop]) {//converts the char into a decimal
		case '0':
			num = 0;
			break;
		case '1':
			num = 1;
			break;
		case '2':
			num = 2;
			break;
		case '3':
			num = 3;
			break;
		case '4':
			num = 4;
			break;
		case '5':
			num = 5;
			break;
		case '6':
			num = 6;
			break;
		case '7':
			num = 7;
			break;
		case '8':
			num = 8;
			break;
		case '9':
			num = 9;
			break;
		case 'A':
			num = 10;
			break;
		case 'B':
			num = 11;
			break;
		case 'C':
			num = 12;
			break;
		case 'D':
			num = 13;
			break;
		case 'E':
			num = 14;
			break;
		case 'F':
			num = 15;
			break;
		case 'G':
			num = 16;
			break;
		case 'H':
			num = 17;
			break;
		case 'I':
			num = 18;
			break;
		case 'J':
			num = 19;
			break;
		case 'K':
			num = 20;
			break;
		case 'L':
			num = 21;
			break;
		case 'M':
			num = 22;
			break;
		case 'N':
			num = 23;
			break;
		case 'O':
			num = 24;
			break;
		case 'P':
			num = 25;
			break;
		case 'Q':
			num = 26;
			break;
		case 'R':
			num = 27;
			break;
		case 'S':
			num = 28;
			break;
		case 'T':
			num = 29;
			break;
		case 'U':
			num = 30;
			break;
		case 'V':
			num = 31;
			break;
		case 'W':
			num = 32;
			break;
		case 'X':
			num = 33;
			break;
		case 'Y':
			num = 34;
			break;
		case 'Z':
			num = 35;
			break;
		}
		for (int i = 0; i < 6; i++) {//converts the decimal into a 6 pack binary number
			if (num == 1) {
				text = "1" + text;
				num = 0;
			}
			else if (num == 0) {
				text = "0" + text;
			}
			else {
				if (num % 2 == 1) {
					text = "1" + text;
				}
				else if (num % 2 == 0) {
					text = "0" + text;
				}
				num = num / 2;
			}
		}
	}
	return text;
}
std::string Desencript(std::string myString) {
	std::string aux, text;
	int num;
	string::iterator it;
	int i = 0;
	for (it = myString.begin(); it < myString.end(); it += 6, i += 6) {//gets a pack 6 digit binary number
		aux = "";
		num = 0;
		for (int j = i; j < i + 6; j++) {
			aux = aux + myString[j];
		}
		num = std::stoi(aux, nullptr, 2);//converts the binary into decimal number
		switch (num) {//converts the decimal into a char and adds it to the return text
		case 0:
			text = '0' + text;
			break;
		case 1:
			text = '1' + text;
			break;
		case 2:
			text = '2' + text;
			break;
		case 3:
			text = '3' + text;
			break;
		case 4:
			text = '4' + text;
			break;
		case 5:
			text = '5' + text;
			break;
		case 6:
			text = '6' + text;
			break;
		case 7:
			text = '7' + text;
			break;
		case 8:
			text = '8' + text;
			break;
		case 9:
			text = '9' + text;
			break;
		case 10:
			text = 'A' + text;
			break;
		case 11:
			text = 'B' + text;
			break;
		case 12:
			text = 'C' + text;
			break;
		case 13:
			text = 'D' + text;
			break;
		case 14:
			text = 'E' + text;
			break;
		case 15:
			text = 'F' + text;
			break;
		case 16:
			text = 'G' + text;
			break;
		case 17:
			text = 'H' + text;
			break;
		case 18:
			text = 'I' + text;
			break;
		case 19:
			text = 'J' + text;
			break;
		case 20:
			text = 'K' + text;
			break;
		case 21:
			text = 'L' + text;
			break;
		case 22:
			text = 'M' + text;
			break;
		case 23:
			text = 'N' + text;
			break;
		case 24:
			text = 'O' + text;
			break;
		case 25:
			text = 'P' + text;
			break;
		case 26:
			text = 'Q' + text;
			break;
		case 27:
			text = 'R' + text;
			break;
		case 28:
			text = 'S' + text;
			break;
		case 29:
			text = 'T' + text;
			break;
		case 30:
			text = 'U' + text;
			break;
		case 31:
			text = 'V' + text;
			break;
		case 32:
			text = 'W' + text;
			break;
		case 33:
			text = 'X' + text;
			break;
		case 34:
			text = 'Y' + text;
			break;
		case 35:
			text = 'Z' + text;
			break;
		}
	}
	return text;
}

void Ranking() {

	struct Persona {
		std::string name;
		string points;
	};
	Persona player;
	player.points = std::to_string(score);

	cout << "\nIntroduce your name:\n";
	cin >> player.name; //demanar nom del jugador.

	Persona Ranking[10];//ranking of the game
	std::string aux[20];//auxiliar to read the file
	int counter = 0;
	std::ifstream inputFile("ranking.dat");
	std::string line;
	while (getline(inputFile, line)) {//gets the info from the file
		if (!line.length() || line[0] == '#')
			continue;
		std::istringstream iss(line);
		iss >> aux[counter];
		counter++;
	}
	inputFile.close();
	counter = 0;
	bool highscore = false;
	int high;
	for (int i = 0; i < 19; i += 2) {//puts the info of the file in the ranking 
		Ranking[counter].name = Desencript(aux[i]);
		Ranking[counter].points = Desencript(aux[i + 1]);
		if (Ranking[counter].points < player.points && highscore == false) { highscore = true; high = counter; cout << counter << endl; }//checks if there is a high score
		counter++;
	}
	if (highscore == true) {//reorders the high score
		for (int i = 8; i >= high; i--) {
			Ranking[i + 1].name = Ranking[i].name;
			Ranking[i + 1].points = Ranking[i].points;
		}
		Ranking[high].name = player.name;
		Ranking[high].points = player.points;

		ofstream file("Ranking.dat");
		file.clear();
		for (int i = 0; i < 10; i++) {
			file << Encript(Ranking[i].name) << endl;
			file << Encript(Ranking[i].points) << endl;
		}
		file.close();
	}

	counter = 1;
	cout << "The ranking is: \n";//prints the ranking
	for (int i = 0; i < 10; i++) {
		cout << counter << "- " << Ranking[i].name << " " << Ranking[i].points << " points." << endl;
		counter++;

	}
}*/
void RankingEasy() {
	struct Persona {
		std::string name;
		int points;
	};
	Persona player;
	player.points = score;

	bool highscore = false;
	int high;

	cout<< "\nIntroduce your name:\n";
	cin >> player.name; //demanar nom del jugador.

	Persona Ranking[10];//ranking of the game
	std::string aux[20];//auxiliar to read the file
	int counter = 0;
	std::ifstream inputFile("RankingEasy.dat", ios::binary);

	// READ //

	for (int i = 0; i < 10; i++) {
		getline(inputFile, Ranking[i].name, '\0'); // Get player name (only if null ternimated in binary)
		inputFile.read(reinterpret_cast<char*>(&Ranking[i].points), sizeof(Ranking[i].points)); // Read int bytes
		if (Ranking[counter].points < player.points && highscore == false) { highscore = true; high = counter; cout << counter << endl; }
		counter++;
	}

	// HIGH SCORE //
	if (highscore == true) {//reorders the high score
		for (int i = 8; i >= high; i--) {
			Ranking[i + 1].name = Ranking[i].name;
			Ranking[i + 1].points = Ranking[i].points;
		}
		Ranking[high].name = player.name;
		Ranking[high].points = player.points;
		// WRITE //
		ofstream outputfile("RankingEasy.dat", ios::binary);
		outputfile.clear();
		for (int i = 0; i < 10; i++) {
			outputfile.write(Ranking[i].name.c_str(), Ranking[i].name.size()); // Write string to binary file
			outputfile.write("\0\n", sizeof(char)); // Add null end string for easier reading
			outputfile.write(reinterpret_cast<char *>(&Ranking[i].points), sizeof(Ranking[i].points));
		}
		outputfile.close();
	}
	cout << "The EASY ranking is: \n";
	for (int i = 0; i < 10; i++) {
		cout << i+1 << ") " << Ranking[i].name << ": " << Ranking[i].points << endl;
	}

	// SI PETA PROVAR AIXO AL MAIN //

	/*struct auxiliar {
		string aux1;
		int aux2;
	};
	auxiliar Auxiliar = { "0",0 };
	ofstream outputfile("Ranking.dat", ios::binary);
	for (int i = 0; i < 10; i++) {
		outputfile.write(Auxiliar.aux1.c_str(), Auxiliar.aux1.size()); // Write string to binary file
		outputfile.write("\0\n", sizeof(char)); // Add null end string for easier reading
		outputfile.write(reinterpret_cast<char *>(&Auxiliar.aux2), sizeof(Auxiliar.aux2));
	}
	outputfile.close();*/
}

void RankingMedium() {
	struct Persona {
		std::string name;
		int points;
	};
	Persona player;
	player.points = score;

	bool highscore = false;
	int high;

	cout << "\nIntroduce your name:\n";
	cin >> player.name; //demanar nom del jugador.

	Persona Ranking[10];//ranking of the game
	std::string aux[20];//auxiliar to read the file
	int counter = 0;
	std::ifstream inputFile("RankingMedium.dat", ios::binary);

	// READ //

	for (int i = 0; i < 10; i++) {
		getline(inputFile, Ranking[i].name, '\0'); // Get player name (only if null ternimated in binary)
		inputFile.read(reinterpret_cast<char*>(&Ranking[i].points), sizeof(Ranking[i].points)); // Read int bytes
		if (Ranking[counter].points < player.points && highscore == false) { highscore = true; high = counter; cout << counter << endl; }
		counter++;
	}

	// HIGH SCORE //
	if (highscore == true) {//reorders the high score
		for (int i = 8; i >= high; i--) {
			Ranking[i + 1].name = Ranking[i].name;
			Ranking[i + 1].points = Ranking[i].points;
		}
		Ranking[high].name = player.name;
		Ranking[high].points = player.points;
		// WRITE //
		ofstream outputfile("RankingMedium.dat", ios::binary);
		outputfile.clear();
		for (int i = 0; i < 10; i++) {
			outputfile.write(Ranking[i].name.c_str(), Ranking[i].name.size()); // Write string to binary file
			outputfile.write("\0\n", sizeof(char)); // Add null end string for easier reading
			outputfile.write(reinterpret_cast<char *>(&Ranking[i].points), sizeof(Ranking[i].points));
		}
		outputfile.close();
	}
	cout << "The MEDIUM ranking is: \n";
	for (int i = 0; i < 10; i++) {
		cout << i + 1 << ") " << Ranking[i].name << ": " << Ranking[i].points << endl;
	}

	// SI PETA PROVAR AIXO AL MAIN //

	/*struct auxiliar {
	string aux1;
	int aux2;
	};
	auxiliar Auxiliar = { "0",0 };
	ofstream outputfile("Ranking.dat", ios::binary);
	for (int i = 0; i < 10; i++) {
	outputfile.write(Auxiliar.aux1.c_str(), Auxiliar.aux1.size()); // Write string to binary file
	outputfile.write("\0\n", sizeof(char)); // Add null end string for easier reading
	outputfile.write(reinterpret_cast<char *>(&Auxiliar.aux2), sizeof(Auxiliar.aux2));
	}
	outputfile.close();*/
}

void RankingHard() {
	struct Persona {
		std::string name;
		int points;
	};
	Persona player;
	player.points = score;

	bool highscore = false;
	int high;

	cout << "\nIntroduce your name:\n";
	cin >> player.name; //demanar nom del jugador.

	Persona Ranking[10];//ranking of the game
	std::string aux[20];//auxiliar to read the file
	int counter = 0;
	std::ifstream inputFile("RankingHard.dat", ios::binary);

	// READ //

	for (int i = 0; i < 10; i++) {
		getline(inputFile, Ranking[i].name, '\0'); // Get player name (only if null ternimated in binary)
		inputFile.read(reinterpret_cast<char*>(&Ranking[i].points), sizeof(Ranking[i].points)); // Read int bytes
		if (Ranking[counter].points < player.points && highscore == false) { highscore = true; high = counter; cout << counter << endl; }
		counter++;
	}

	// HIGH SCORE //
	if (highscore == true) {//reorders the high score
		for (int i = 8; i >= high; i--) {
			Ranking[i + 1].name = Ranking[i].name;
			Ranking[i + 1].points = Ranking[i].points;
		}
		Ranking[high].name = player.name;
		Ranking[high].points = player.points;
		// WRITE //
		ofstream outputfile("RankingHard.dat", ios::binary);
		outputfile.clear();
		for (int i = 0; i < 10; i++) {
			outputfile.write(Ranking[i].name.c_str(), Ranking[i].name.size()); // Write string to binary file
			outputfile.write("\0\n", sizeof(char)); // Add null end string for easier reading
			outputfile.write(reinterpret_cast<char *>(&Ranking[i].points), sizeof(Ranking[i].points));
		}
		outputfile.close();
	}
	cout << "The HARD ranking is: \n";
	for (int i = 0; i < 10; i++) {
		cout << i + 1 << ") " << Ranking[i].name << ": " << Ranking[i].points << endl;
	}
}

//This function destroys textures, renderer, window and quits SDL
void KillBill()
{
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
	SDL_DestroyTexture(timeBarTexture);
	SDL_DestroyTexture(timeBarTexture2);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

//Here there are the funtions that encript and desencript from the text


//We call here all functions, the order is very important!
int main(int, char*[])
{
	//Loading and playing the background music for the game
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("../res/sfx/music.wav");
	Mix_PlayMusic(music, -1); //-1 plays the music forever

	for (game =0;game <1;game++){
		//First Menu
		while (menu || dificulties)
		{
			Menu();
		}
		//Setting up a few things according to game dificulty
		Setup();
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
		SDL_RenderPresent(renderer);
		//Main game loop
		while (!gameOver) //(play && !gameOver)
		{
			Input();
			Draw();
			Logic();
			NextLevel();
			Timer();
			//We use Sleep() function to control the speed of the game
			if (easy)
				Sleep(speed);
			else if (medium)
				Sleep(speed);
			else if (hard)
				Sleep(speed);
		}
		//Ranking
		if (easy) { RankingEasy(); }
		else if (medium) { RankingMedium(); }
		else if (hard) { RankingHard(); }
		//Back to menu
		easy = false;
		medium = false;
		hard = false;
		gameOver = false;
		menu = true;
		
		game--;
		SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
		SDL_RenderPresent(renderer);
	}
	
	/*int num=0;
	ofstream outputfile("Ranking.dat", ios::out | ios::binary);
	for (int i = 0; i < 20; i++) {
		outputfile.write(reinterpret_cast<char *>(&num), sizeof num);
	}
	outputfile.close();*/
	return 0;
}