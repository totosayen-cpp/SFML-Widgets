#include "TextBox.hpp"
#include "processWindow.hpp"
#include "CheckBox.hpp"

#ifdef _WIN32	// also defined for Win64 computers
#define WINDOWS
#include <Windows.h>
#endif

int main() {
#ifdef WINDOWS
	SetConsoleOutputCP(1252);
	SetConsoleCP(1252);
#endif
	global::arial.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");
	TextBox box;
	elements::textBoxes[0]->setBackgroundColor(sf::Color::Red);
	while (global::window.isOpen()) {
		while (global::window.pollEvent(global::event)) {
			processEvents::processAll();
		}
		drawAll();
	}
	return 0;
}