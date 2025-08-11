#pragma once
#define WINDOW_H	
#include "raylib.h"
#include <string>
using namespace std;
class Window
{
public:
	Window();
	Vector2 Draw(float x, float y){
		return { x, y };
	}
	void init(Rectangle rect, Color color, string title, bool isOpen);
	Rectangle rect; // Position and size of the window
	Color color; // Color of the window
	string title; // Title of the window
	bool isOpen; // Whether the window is open or not
};
class ChangeBgWindow : public Window {
public:
	Vector2 Draw(float x, float y){
		float width = 500;
		float height = 500;

		Rectangle closingBtn = { x + width - 50, y, 50, 50}; // Bouton de fermeture

		// fond fenetre
		DrawRectangle(x, y, width, height, Fade(GRAY, 0.9f)); // Fond semi-transparent
		// barre de titre de la fenetre
		float barreTitleHeight = 50;
		DrawRectangle(x, y, width, barreTitleHeight, DARKBLUE); // Barre de titre
		// Texte de la barre de titre
		DrawText("Change Background", x + 10, y + 10, 20, WHITE);
		// interaction btns
		DrawRectangle(x + width - 100, y, 50, 50, WHITE); // Bouton Full screen
		DrawRectangleRec(closingBtn,RED); // Bouton Close

		bool isDragged = false;
		//comportement de la fenetre 
		if (CheckCollisionPointRec(GetMousePosition(), { x,y,width,barreTitleHeight }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			isDragged = true;
		}
		if (isDragged && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();
			x = mousePos.x - width / 2; // Centrer la fenêtre sur le curseur
			y = mousePos.y - barreTitleHeight / 2; // Ajuster la position verticale
			return { x, y }; // Retourner la nouvelle position de la fenêtre
		}
		else {
			isDragged = false; // Réinitialiser le drapeau de glissement
			return { x, y }; // Retourner la position actuelle de la fenêtre
		}
		if (CheckCollisionPointRec(GetMousePosition(), closingBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			// Close the window
			isOpen = false;// Indicate that the window should be closed
		}
		return { x, y }; // Return the current position of the window
	}
};

