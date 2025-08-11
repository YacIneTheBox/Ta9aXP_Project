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

void Window::init(Rectangle rect, Color color, string title, bool isOpen) {
	this->rect = rect;
	this->color = color;
	this->title = title;
	this->isOpen = isOpen;
	cout << "Window initialized with title: " << title << endl;
}
