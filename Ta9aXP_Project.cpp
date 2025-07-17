// Ta9aXP_Project.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <raylib.h>

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
	Rectangle rect;
	bool isoccupied = false;
	bool isSelected = false; // Pour savoir si le bloc est sélectionné
	App app; // l'application qui occupe le bloc
	float lastClickTime = 0.0f;
}Brick;

void CollisionSelectingApp(GameScene* currentScene, Vector2 mousePos, Brick& bloc);
void InitializeDesktopScene(App* AllApps, int nombreApp, Brick* blocks, int N_BLOCKS_HORIZONTAL, int N_BLOCKS_VERTICAL);
void GoBack(GameScene& currentScene);
void MovingApps(App* AllApps, int N_BLOCKS_VERTICAL, int N_BLOCKS_HORIZONTAL, Brick* blocks);

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

	while (!WindowShouldClose()) {

		switch (currentScene) {
			case Desktop:
			{
				for (int row = 0; row < N_BLOCKS_VERTICAL; row++) {
					for (int col = 0; col < N_BLOCKS_HORIZONTAL; col++) {
						DrawRectangleLinesEx(blocks[row * N_BLOCKS_HORIZONTAL + col].rect,1, BLACK);
						MovingApps(AllApps,N_BLOCKS_VERTICAL,N_BLOCKS_HORIZONTAL, blocks);
					}
				}
				
				for (int i = 0; i < N_BLOCKS_VERTICAL* N_BLOCKS_HORIZONTAL; i++) {
					CollisionSelectingApp(&currentScene, GetMousePosition(), blocks[i]);
				}
				
				break;
			}
			case Paint: {
				GoBack(currentScene);
				break;
			}
			case Calculator: {
				GoBack(currentScene);
				break;
			}
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangleRec(taskBarPos, TaskbarColor);
		switch (currentScene) {
			case Desktop: {
				ClearBackground(DARKGREEN);
				InitializeDesktopScene(AllApps, nombreApp, blocks, N_BLOCKS_HORIZONTAL, N_BLOCKS_VERTICAL);
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
	for (int i = 0; i < nombreApp; i++) {
		// Calculer la colonne et la ligne
		int col = i / N_BLOCKS_VERTICAL; // colonne courante
		int row = i % N_BLOCKS_VERTICAL; // ligne courante

		int idx = row * N_BLOCKS_HORIZONTAL + col;

		AllApps[i].posSize = blocks[idx].rect;
		blocks[idx].app = AllApps[i];
		blocks[idx].isoccupied = true;

		DrawRectangleRec(blocks[idx].rect, AllApps[i].iconColor);
	}
}

void MovingApps(App* AllApps,int N_BLOCKS_VERTICAL, int N_BLOCKS_HORIZONTAL,Brick* blocks) {
	static int selectedBlockIndex = -1; // Index du bloc sélectionné
	Vector2 mousePos = GetMousePosition();

	if (selectedBlockIndex == -1) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			for (int i = 0; i < N_BLOCKS_VERTICAL * N_BLOCKS_HORIZONTAL; i++) {
				if (CheckCollisionPointRec(mousePos, AllApps[i].posSize)) {
					selectedBlockIndex = i; // Marquer le bloc comme sélectionné
					break;
				}
			}
		}
	}
	if (selectedBlockIndex != -1) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			AllApps[selectedBlockIndex].posSize.x = mousePos.x - blocks[selectedBlockIndex].rect.width / 2;
			AllApps[selectedBlockIndex].posSize.y = mousePos.y - blocks[selectedBlockIndex].rect.height / 2;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			blocks[selectedBlockIndex].isSelected = false; // Désélectionner le bloc
			selectedBlockIndex = -1; // Réinitialiser l'index du bloc sélectionné
			cout << "bro dropped it" << endl;
		}
	}
	// gerer le deplacement des application a travers les blocs
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

Vector2 ClosestPoint(Brick block, Vector2 rect) {
	Vector2 closestPoint = { 0, 0 };
	closestPoint.x = block.rect.width / 2  ;

	return closestPoint;
}

