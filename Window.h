#pragma once
#define WINDOW_H	
#include "raylib.h"
#include <string>
using namespace std;
class Window
{
public:
	Window();
	Vector2 Draw(float x, float y);
	void init(Rectangle rect, Color color, string title, bool isOpen);
private:
	Rectangle rect; // Position and size of the window
	Color color; // Color of the window
	string title; // Title of the window
	bool isOpen; // Whether the window is open or not
};

