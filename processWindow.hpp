#include "TextBox.hpp"
#include "CheckBox.hpp"

namespace processEvents {
	void processCheckBoxes() {
		for (CheckBox* checkBox : elements::checkBoxes) {
			if (global::event.type == sf::Event::MouseButtonPressed) {
				checkBox->click();
			}
		}
	}

	void processTextBoxes() {
		for (TextBox* textBox : elements::textBoxes) {
			if (global::event.type == sf::Event::MouseButtonPressed) {
				textBox->click();
			}
			else if (global::event.type == sf::Event::TextEntered) {
				textBox->processKeyboardInput();
			}
			else {
				return;
			}
		}
	}

	void processAll() {
		if (global::event.type == sf::Event::Closed) {
			global::window.close();
			return;
		}
		processCheckBoxes();
		processTextBoxes();
	}
}

void drawAll(sf::Color clearWindow = sf::Color::White) {
	global::window.clear(clearWindow);
	if (elements::buttons.size() > 0) {
		for (const Button* button : elements::buttons) {
			global::window.draw(*button);
		}
	}
	if (elements::lines.size() > 0) {
		for (const Line* line : elements::lines) {
			global::window.draw(*line);
		}
	}
	if (elements::checkBoxes.size() > 0) {
		for (const CheckBox* checkBox : elements::checkBoxes) {
			global::window.draw(*checkBox);
		}
	}
	if (elements::textBoxes.size() > 0) {
		for (const TextBox* textBox : elements::textBoxes) {
			global::window.draw(*textBox);
		}
	}
	global::window.display();
}