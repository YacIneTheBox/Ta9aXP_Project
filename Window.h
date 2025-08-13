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
    bool isDragged = false;
    Vector2 dragOffset = { 0, 0 };

    void setRect(Rectangle newRect) {
		rect.x = newRect.x;
		rect.y = newRect.y;
		rect.width = newRect.width;
		rect.height = newRect.height;
    }
	void setIsOpen(bool open) {
		isOpen = open;
	}

    Vector2 ClosingWindow(Rectangle closingBtn) {
        if (CheckCollisionPointRec(GetMousePosition(), closingBtn) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            setIsOpen(false);
            isDragged = false; // stop dragging when closed
            return { rect.x, rect.y };
        }
    }

    Rectangle DragingWindow(Rectangle dragingZone) {
        // --- Start dragging ---
        if (CheckCollisionPointRec(GetMousePosition(), { dragingZone.x, dragingZone.y, dragingZone.width, dragingZone.height }) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            isDragged = true;
            dragOffset.x = GetMousePosition().x - dragingZone.x;
            dragOffset.y = GetMousePosition().y - dragingZone.y;
        }

        // --- Continue dragging ---
        if (isDragged && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            dragingZone.x = GetMousePosition().x - dragOffset.x;
            dragingZone.y = GetMousePosition().y - dragOffset.y;
        }

        // --- Stop dragging ---
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragged = false;
        }
        return { dragingZone.x ,dragingZone.y,dragingZone.width,dragingZone.height };
    }
};
class ChangeBgWindow : public Window {
private:
    Rectangle rect = { 0,0,500,500 }; // Position and size of the window
    void ColorSetter(Rectangle recColor, Color color, Color* DesktopColor) {
        if (CheckCollisionPointRec(GetMousePosition(), recColor) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *DesktopColor = color; // Change the desktop color
        }
	}
public:
    Rectangle Draw(float x, float y, float width,float height,Color* DesktopColor) {

        float barreTitleHeight = 50;


        Rectangle closingBtn = { x + width - 50, y, 50, 50 };
		Rectangle max_reducBtn = { x + width - 100, y, 50, 50 };

        // --- Draw window background ---
        DrawRectangle(x, y, width, height, Fade(GRAY, 0.9f));
        DrawRectangle(x, y, width, barreTitleHeight, DARKBLUE);
        DrawText("Change Background", x + 10, y + 10, 20, WHITE);

        // --- Draw color picker ---
        Color color1 = ORANGE;
        Color color2 = PURPLE;
        Color color3 = GREEN;
        Color color4 = PINK;

        Rectangle recColor1 = { x + width / 10, y + height / 8, width * 0.3f, height * 0.3f };
        DrawRectangleRec(recColor1, color1);
        ColorSetter(recColor1, color1, DesktopColor);
        Rectangle recColor2 = { x + width / 10 + width * 0.5f, y + height / 8, width * 0.3f, height * 0.3f };
        DrawRectangleRec(recColor2, color2);
        ColorSetter(recColor2, color2, DesktopColor);
        Rectangle recColor3 = { x + width / 10, y + height / 8 + height * 0.4f, width * 0.3f, height * 0.3f };
        DrawRectangleRec(recColor3, color3);
        ColorSetter(recColor3, color3, DesktopColor);
        Rectangle recColor4 = { x + width / 10 + width * 0.5f, y + height / 8 + height * 0.4f, width * 0.3f, height * 0.3f };
        DrawRectangleRec(recColor4, color4);
        ColorSetter(recColor4, color4, DesktopColor);

        // --- Buttons ---
        DrawRectangleRec(max_reducBtn, WHITE); // Fullscreen btn
        DrawRectangleRec(closingBtn, RED); // Close btn

        // --- Close button click ---
		ClosingWindow(closingBtn);

        // --- Start dragging ---
        Rectangle newRect;
        newRect = DragingWindow({ x,y,width,barreTitleHeight });
		x = newRect.x;
		y = newRect.y;


		// --- reduc/maximize button ---
        if (CheckCollisionPointRec(GetMousePosition(), max_reducBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Toggle between maximized and normal size
            if (width == 500 && height == 500) {
                width = GetScreenWidth();
                height = GetScreenHeight() - barreTitleHeight;
				x = 0;
				y = 0;
            } else {
                width = 500;
                height = 500;
            }
		}
		rect.width = width;
		rect.height = height;
        rect.x = x;
        rect.y = y;
        return { x, y,width,height };
    }
};


