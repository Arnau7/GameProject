#include <Windows.h>
//Now, it doesn't work, ty for my effort :)

/*
//Declaro el procés de Windows
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//Fer que el nom de la classe sigui una variable global
char szClassName[] = "Snake Game";

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nFunsterStill)
{
	HWND hwnd;		//Operador de la finestra
	MSG messages;	//Aquí es guarden els missatge a l'aplicació
	WNDCLASSEX wincl; //Estructura de dades per el windowclass

					  //Estructura de la finestra
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure; //Funció cridada per windows
	wincl.style = CS_DBLCLKS;	//Agafa els doble clicks
	wincl.cbSize = sizeof(WNDCLASSEX);

	//

	//Fer servir el color per defecte de Windows per la finestra
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	//Regitrar la classe de la finestra, i si falla surt del programa
	if (!RegisterClassEx(&wincl))
		return 0;

	//La classe està registrada, creem el programa
	hwnd = CreateWindowEx(
		0,					//Possibilitats de variació
		szClassName,		//Nom de la classe
		"Snake Game",		//Text del títol
		WS_SYSMENU | WS_MINIMIZEBOX,//Finestra per defecte
		CW_USEDEFAULT,		//Windows decideix la posició
		CW_USEDEFAULT,		//A on acaba la finestra
		640,				//Amplada del programa
		480,				//Altura del programa
		HWND_DESKTOP,		//La finestra és una finestra filla de l'escriptori
		NULL,				//No Menu
		hThisInstance,		//Instancia de l'operador del programa
		NULL				//Cap creació de dades de la finestra
	);

	//Fer la finestra visible a la pantalla
	ShowWindow(hwnd, nFunsterStill);

	//Fer anar el bucle del missatge. Funcionarà fins que GetMessage() retorni 0
	while (GetMessage(&messages, NULL, 0, 0))
	{
		//Traduir missatge amb clau virtual a caracters
		TranslateMessage(&messages);
		//Enviar missatge a WindowsProcedure
		DispatchMessage(&messages);
	}
	//El valor de retorn del programa es 0 - El valor the PostQuitMessage() donat
	return messages.wParam;
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)		//Operar els missatges
	{
	case WM_CREATE: {
		HMENU hMenubar = CreateMenu();
		HMENU hPlay = CreateMenu();
		HMENU hOption = CreateMenu();

		AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hOption, "Play");

		AppendMenu(hMenubar, MF_STRING, NULL, "Easy");
		AppendMenu(hMenubar, MF_STRING, NULL, "Medium");
		AppendMenu(hMenubar, MF_STRING, NULL, "Hard");

		SetMenu(hwnd, hMenubar);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);		//Envia un WM_QUIT a la cua del missatge
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}*/