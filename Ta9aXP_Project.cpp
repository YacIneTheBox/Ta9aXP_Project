// Ta9aXP_Project.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <raylib.h>

using namespace std;

typedef enum GameScene {
	Desktop,
	Paint,
	Calculator,
}GameScene;

typedef struct App {
	string name;
	GameScene scene;
	//Image icon;// pour l'instant on mets color
	Color iconColor = LIGHTGRAY;
	Rectangle posSize = {0,0,60 ,60 };

}App;

void CollisionSelectingApp(GameScene* currentScene, Vector2 pos1, App app);
void InitializeDesktopScene(App* apps, int appCount);

int main()
{
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 890;
	const int ICON_SIZE = 60;
	const int BLOCK_SIZE = 120;
	const int N_BLOCKS_HORIZONTAL = SCREEN_WIDTH / BLOCK_SIZE;
	const int N_BLOCKS_VERTICAL = SCREEN_HEIGHT / BLOCK_SIZE;

	GameScene currentScene = Desktop;

	Rectangle taskBarPos = { 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50 };
	Color TaskbarColor = DARKGRAY;

	int nombreApp = 3;
	App *AllApps = new App[nombreApp]{
		{"Desktop", Desktop, RED, {SCREEN_WIDTH / 16, SCREEN_HEIGHT / 12,60 ,60}},
		{"Paint", Paint, BLUE, {SCREEN_WIDTH / 16 * 2, SCREEN_HEIGHT / 12,60 ,60}},
		{"Calculator", Calculator, YELLOW, {SCREEN_WIDTH / 16 * 3, SCREEN_HEIGHT / 12,60 ,60}}
	};




	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ta9aXP");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {

		switch (currentScene) {
			case Desktop:
			{
				for (int i = 0; i < nombreApp; i++) {
					CollisionSelectingApp(&currentScene, GetMousePosition(), AllApps[i]);
				}
				
			}
			case Paint: {

			}
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangleRec(taskBarPos, TaskbarColor);
		switch (currentScene) {
			case Desktop: {
				ClearBackground(DARKGREEN);
				for (int i = 0; i < nombreApp; i++) {
					DrawRectangleRec(AllApps[i].posSize, AllApps[i].iconColor);
					cout << "App: " << AllApps[i].name << " at position: " << AllApps[i].posSize.x << ", " << AllApps[i].posSize.y << endl;
				}
				
				break;
			}
			case Paint: {
				cout << "you are in Paint mode!" << endl;
				ClearBackground(DARKBLUE);
				break;
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
void InitializeDesktopScene(App *apps, int appCount) {
	for (int i = 0; i < appCount; i++) {
		DrawRectangleRec(apps[i].posSize, apps[i].iconColor);
	}
}

void CollisionSelectingApp(GameScene *currentScene,Vector2 pos1,App app) {
	if (CheckCollisionPointRec(pos1,app.posSize) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			*currentScene = app.scene;	
	}
	else {
		cout << "No collision." << endl;
	}
}

