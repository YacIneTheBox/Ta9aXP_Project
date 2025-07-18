// Ta9aXP_Project.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <raylib.h>
#include <cmath>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"  // see what we can do with this library
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

typedef enum GameScene {
	Desktop = 0,
	Paint,
	Calculator,
}GameScene;

typedef struct App {
	string name;
	GameScene scene;
	//Image icon;// for now we put color instead of icon
	Color iconColor = LIGHTGRAY;
	Rectangle posSize = {0,0,60 ,60 };

}App;

typedef struct Brick {
	Rectangle rect = {0,0,0,0};
	bool isoccupied = false;
	bool isSelected = false; // Pour savoir si le bloc est sélectionné
	App app; // l'application qui occupe le bloc
	float lastClickTime = 0.0f;
}Brick;



void CollisionSelectingApp(GameScene* currentScene, Vector2 mousePos, Brick& bloc);
void InitializeDesktopScene(App* AllApps, int nombreApp, Brick* blocks, int N_BLOCKS_HORIZONTAL, int N_BLOCKS_VERTICAL);
void GoBack(GameScene& currentScene);
void MovingApps(App* AllApps, int N_BLOCKS_VERTICAL, int N_BLOCKS_HORIZONTAL, Brick* blocks);
int ClosestPoint(Brick* block, int N_BLOCK_HORIZONTAL, int N_BLOCK_VERTICAL, Vector2 appPos);
bool ClickDroitGestion(Brick* bricks, int N_BLOCK_HORIZONTAL, int N_BLOCK_VERTICAL, int& idxClickDroitedApp);

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

	int nombreApp =9;
	App *AllApps = new App[nombreApp]{
		{"Desktop", Desktop, RED},
		{"Paint", Paint, BLUE},
		{"Calculator", Calculator, YELLOW},
		{ "Desktop", Desktop, RED },
		{"Paint", Paint, BLUE},
		{"Calculator", Calculator, YELLOW},
		{ "Desktop", Desktop, RED },
		{"Paint", Paint, BLUE},
		{"Calculator", Calculator, YELLOW},
	};

	Brick* blocks = new Brick[N_BLOCKS_HORIZONTAL * N_BLOCKS_VERTICAL];
	for (int row = 0 ; row < N_BLOCKS_VERTICAL; row++) {
		for (int col = 0; col < N_BLOCKS_HORIZONTAL; col++) {
			blocks[row * N_BLOCKS_HORIZONTAL + col].rect = {
				(float)(col * BLOCK_SIZE),
				(float)(row * BLOCK_SIZE),
				(float)BLOCK_SIZE,
				(float)BLOCK_SIZE
			};
			blocks[row * N_BLOCKS_HORIZONTAL + col].isoccupied = false;
		}
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ta9a XP");

	SetTargetFPS(60);

	for (int i = 0; i < nombreApp; i++) {
		int col = i / N_BLOCKS_VERTICAL;
		int row = i % N_BLOCKS_VERTICAL;
		int idx = row * N_BLOCKS_HORIZONTAL + col;

		AllApps[i].posSize = blocks[idx].rect;
		blocks[idx].app = AllApps[i];
		blocks[idx].isoccupied = true;
	}
	int idxClickDroitedApp = -1;
	bool rightClick = false;
	while (!WindowShouldClose()) {
		time_t now = time(nullptr);
		struct tm tstruct;
		localtime_s(&tstruct, &now); // Version sécurisée pour Windows/Visual Studio

		// Formater l'heure dans une string C++
		std::ostringstream oss;
		oss << std::put_time(&tstruct, "%H:%M:%S");
		std::string heure = oss.str();
		switch (currentScene) {
			case Desktop:
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
					rightClick = false; // Reset right click state on left click
				}
				MovingApps(AllApps, N_BLOCKS_VERTICAL, N_BLOCKS_HORIZONTAL, blocks);
				
				for (int i = 0; i < N_BLOCKS_VERTICAL* N_BLOCKS_HORIZONTAL; i++) {
					CollisionSelectingApp(&currentScene, GetMousePosition(), blocks[i]);
				}
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && ClickDroitGestion(blocks, N_BLOCKS_HORIZONTAL, N_BLOCKS_VERTICAL,idxClickDroitedApp)) {
					rightClick = true;
				}
				

				break;
			}
			case Paint: {
				GoBack(currentScene);
				MovingApps(AllApps, N_BLOCKS_VERTICAL, N_BLOCKS_HORIZONTAL, blocks);
				break;
			}
			case Calculator: {
				GoBack(currentScene);
				MovingApps(AllApps, N_BLOCKS_VERTICAL, N_BLOCKS_HORIZONTAL, blocks);
				break;
			}
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		// Draws that always appear
		DrawRectangleRec(taskBarPos, TaskbarColor);
		DrawText(heure.c_str(), SCREEN_WIDTH - SCREEN_WIDTH/14, SCREEN_HEIGHT - SCREEN_HEIGHT/25,20, WHITE);

		switch (currentScene) {
			case Desktop: {
				ClearBackground(DARKGREEN);
				InitializeDesktopScene(AllApps, nombreApp, blocks, N_BLOCKS_HORIZONTAL, N_BLOCKS_VERTICAL);
				
				if (rightClick) {
					float x = blocks[idxClickDroitedApp].rect.x + blocks[idxClickDroitedApp].rect.width / 2;
					float y = blocks[idxClickDroitedApp].rect.y + blocks[idxClickDroitedApp].rect.height / 2;
					DrawRectangle(x, y, 150, 30, DARKGRAY);
					DrawText("Right-clicked on app", x + 5, y + 5, 10, WHITE);
					cout << x << " " << y << endl;
				}
				break;
			}
			case Paint: {
				cout << "you are in Paint mode!" << endl;
				ClearBackground(DARKBLUE);
				DrawText("Paint Mode", 10, 10, 20, WHITE);
				break;
			}
			case Calculator: {
				ClearBackground(GRAY);
				DrawText("Calculator Mode", 10, 10, 20, BLACK);
				break;
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
void InitializeDesktopScene(App* AllApps, int nombreApp, Brick* blocks, int N_BLOCKS_HORIZONTAL, int N_BLOCKS_VERTICAL) {
	for (int i = 0; i < N_BLOCKS_HORIZONTAL * N_BLOCKS_VERTICAL; i++) {
		if (blocks[i].isoccupied) {
			DrawRectangleRec(blocks[i].app.posSize, blocks[i].app.iconColor);
		}
	}
}

void MovingApps(App* AllApps, int N_BLOCKS_VERTICAL, int N_BLOCKS_HORIZONTAL, Brick* blocks) {
	static int selectedBlockIndex = -1;
	Vector2 mousePos = GetMousePosition();

	if (selectedBlockIndex == -1) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			for (int i = 0; i < N_BLOCKS_VERTICAL * N_BLOCKS_HORIZONTAL; i++) {
				if (blocks[i].isoccupied && CheckCollisionCircleRec(mousePos,10 ,blocks[i].app.posSize)) {
					selectedBlockIndex = i;
					break;
				}
			}
		}
	}
	if (selectedBlockIndex != -1) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			blocks[selectedBlockIndex].app.posSize.x = mousePos.x - blocks[selectedBlockIndex].app.posSize.width / 2;
			blocks[selectedBlockIndex].app.posSize.y = mousePos.y - blocks[selectedBlockIndex].app.posSize.height / 2;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			Vector2 appCenter = {
				blocks[selectedBlockIndex].app.posSize.x + blocks[selectedBlockIndex].app.posSize.width / 2,
				blocks[selectedBlockIndex].app.posSize.y + blocks[selectedBlockIndex].app.posSize.height / 2
			};
			int closestIdx = ClosestPoint(blocks, N_BLOCKS_HORIZONTAL, N_BLOCKS_VERTICAL, appCenter);

			if (closestIdx == -1 || closestIdx == selectedBlockIndex) {
				// Pas de bloc cible valide ou on relâche sur le même bloc
				blocks[selectedBlockIndex].app.posSize = blocks[selectedBlockIndex].rect; // Snap back
			}
			else if (blocks[closestIdx].isoccupied) {
				// Bloc cible occupé, refuse le move
				blocks[selectedBlockIndex].app.posSize = blocks[selectedBlockIndex].rect; // Snap back
			}
			else {
				// Déplacement autorisé
				blocks[closestIdx].app = blocks[selectedBlockIndex].app;
				blocks[closestIdx].app.posSize = blocks[closestIdx].rect; // Snap à la grille du nouveau bloc
				blocks[closestIdx].isoccupied = true;

				blocks[selectedBlockIndex].isoccupied = false;
			}
			blocks[selectedBlockIndex].isSelected = false;
			selectedBlockIndex = -1;
		}
	}
}

void CollisionSelectingApp(GameScene *currentScene,Vector2 mousePos,Brick& bloc) {
	float currentTime = GetTime();
	if (CheckCollisionPointRec(mousePos,bloc.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bloc.isoccupied) {
		
		if (currentTime - bloc.lastClickTime < 0.25f) {
			// Double click detected
			*currentScene = bloc.app.scene;
			bloc.lastClickTime = 0.0f; // Reset last click time

		}
		else {
			bloc.lastClickTime = currentTime; // Update last click time
		}
	}
}

void GoBack(GameScene& currentScene) {
	if (IsKeyPressed(KEY_SPACE)) {
		currentScene = Desktop;
	}
}

int ClosestPoint(Brick* blocks, int N_BLOCK_HORIZONTAL, int N_BLOCK_VERTICAL, Vector2 appPos) {
	// On cherche le bloc dont le rect contient le centre de l'app
	for (int i = 0; i < N_BLOCK_HORIZONTAL * N_BLOCK_VERTICAL; i++) {
		if (CheckCollisionPointRec(appPos, blocks[i].rect)) {
			return i;
		}
	}
	// Si aucun bloc ne contient la position, retourne -1
	return -1;
}

bool ClickDroitGestion(Brick* bricks,int N_BLOCK_HORIZONTAL, int N_BLOCK_VERTICAL ,int& idxClickDroitedApp) {

	Vector2 mousePos = GetMousePosition();
	for (int i = 0; i < N_BLOCK_HORIZONTAL * N_BLOCK_VERTICAL; i++) {
		if (CheckCollisionPointRec(mousePos, bricks[i].rect) && bricks[i].isoccupied) {
			// Ici, on peut afficher un menu contextuel ou effectuer une action
			//DrawRectangleRec(bricks[i].rect, Fade(LIGHTGRAY, 0.5f)); // Juste pour visualiser le clic droit
			DrawRectangle(mousePos.x, mousePos.y, 150, 30, DARKGRAY);
			idxClickDroitedApp = i; // Stocke l'index de l'application cliquée
			cout << "Right-clicked on app: " << bricks[i].app.name << endl;
			// On pourrait aussi ajouter des options pour fermer l'application, etc.
			return true;
		}
	}
	return false;
}

