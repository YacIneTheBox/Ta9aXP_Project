#include "Window.h"
#include <iostream>
#include "raygui.h"

using namespace std;
Window::Window() {
	Rectangle rect = { 0, 0, 100, 100 }; // Default rectangle size
	Color color = DARKBLUE; // Default color
	string title = "Default Window"; // Default title
	bool isOpen = false;
}

Vector2 Window::Draw(float x, float y) {
	float width = 500;
	float height = 500;
	// fond fenetre
	DrawRectangle(x, y, width, height, Fade(GRAY, 0.9f)); // Fond semi-transparent
	// barre de titre de la fenetre
	float barreTitleHeight = 50;
	DrawRectangle(x, y, width, barreTitleHeight, DARKBLUE); // Barre de titre
	// Texte de la barre de titre
	DrawText("Change Background", x + 10, y + 10, 20, WHITE);
	// interaction btns
	DrawRectangle(x + width - 100, y, 50, 50, WHITE); // Bouton Full screen
	DrawRectangle(x + width - 50, y, 50, 50, RED); // Bouton Close

	bool isDragged = false;
	//comportement de la fenetre 
	if (CheckCollisionPointRec(GetMousePosition(), { x,y,width,barreTitleHeight }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		isDragged = true;
	}
	if (isDragged && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		cout << "Dragging the window" << endl;
		Vector2 mousePos = GetMousePosition();
		x = mousePos.x - width / 2; // Centrer la fenêtre sur le curseur
		y = mousePos.y - barreTitleHeight / 2; // Ajuster la position verticale
		return { x, y }; // Retourner la nouvelle position de la fenêtre
	}
	else {
		isDragged = false; // Réinitialiser le drapeau de glissement
		return { x, y }; // Retourner la position actuelle de la fenêtre
	}
}

void Window::init(Rectangle rect, Color color, string title, bool isOpen) {
	this->rect = rect;
	this->color = color;
	this->title = title;
	this->isOpen = isOpen;
	cout << "Window initialized with title: " << title << endl;
}