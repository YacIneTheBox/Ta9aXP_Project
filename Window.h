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
	void setIsOpen(bool open) {
		isOpen = open;
	}
};
class ChangeBgWindow : public Window {
private:
    bool isDragged = false;
    Vector2 dragOffset = { 0, 0 };
public:
    Vector2 Draw(float x, float y) {
        float width = 500;
        float height = 500;
        float barreTitleHeight = 50;

        Rectangle closingBtn = { x + width - 50, y, 50, 50 };

        // --- Draw window background ---
        DrawRectangle(x, y, width, height, Fade(GRAY, 0.9f));
        DrawRectangle(x, y, width, barreTitleHeight, DARKBLUE);
        DrawText("Change Background", x + 10, y + 10, 20, WHITE);

        // --- Buttons ---
        DrawRectangle(x + width - 100, y, 50, 50, WHITE); // Fullscreen btn
        DrawRectangleRec(closingBtn, RED); // Close btn

        // --- Close button click ---
        if (CheckCollisionPointRec(GetMousePosition(), closingBtn) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            setIsOpen(false);
            isDragged = false; // stop dragging when closed
            return { x, y };
        }

        // --- Start dragging ---
        if (CheckCollisionPointRec(GetMousePosition(), { x, y, width, barreTitleHeight }) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            isDragged = true;
            dragOffset.x = GetMousePosition().x - x;
            dragOffset.y = GetMousePosition().y - y;
        }

        // --- Continue dragging ---
        if (isDragged && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            x = GetMousePosition().x - dragOffset.x;
            y = GetMousePosition().y - dragOffset.y;
        }

        // --- Stop dragging ---
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragged = false;
        }

        return { x, y };
    }
};


