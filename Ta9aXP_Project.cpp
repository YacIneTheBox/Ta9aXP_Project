// Ta9aXP_Project.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <raylib.h>

using namespace std;
int main()
{
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 800;
    
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ta9aXP");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {


		BeginDrawing();
		ClearBackground(RAYWHITE);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

