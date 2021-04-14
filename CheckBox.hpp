#pragma once
#include "Line.hpp"

class CheckBox : public sf::Drawable {
	mutable bool clicked_ = false;
	float size_ = 25.f;
	sf::Color backgroundColor_ = sf::Color::White;
	sf::Color linesColor_ = sf::Color::Black;
	sf::Color outlineColor_ = sf::Color::Black;
	sf::Vector2f topLeftCornerCoords_ = { 100.f, 100.f };
	sf::Text title_ = sf::Text("CheckBox " + std::to_string(elements::checkBoxes.size() + 1), global::arial);
	CheckBox* copyThis_ = this;

	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const {
		sf::Text title = title_;
		title.setCharacterSize(static_cast<unsigned int>(size_));
		title.move({ topLeftCornerCoords_.x + size_ + 10.f, topLeftCornerCoords_.y });	// x + 10 to leave a little space between box and text
		title.setFillColor(linesColor_);
		window.draw(title);
		sf::RectangleShape box{ {size_, size_} };
		box.move(topLeftCornerCoords_);
		box.setFillColor(backgroundColor_);
		box.setOutlineColor(outlineColor_);
		box.setOutlineThickness(2.f);
		window.draw(box);
		if (clicked_) {
			Line one{ topLeftCornerCoords_, {topLeftCornerCoords_.x + size_, topLeftCornerCoords_.y + size_} };
			one.setColor(linesColor_);
			window.draw(one);

			Line two{ {topLeftCornerCoords_.x + size_, topLeftCornerCoords_.y}, {topLeftCornerCoords_.x , topLeftCornerCoords_.y + size_} };
			two.setColor(linesColor_);
			window.draw(two);
		}
	}

public:

	CheckBox() {
		elements::checkBoxes.push_back(copyThis_);
	}

	CheckBox(std::string const& title, sf::Vector2f const& topLeftCornerCoords, float size, sf::Color backgroundColor, sf::Color linesColor, sf::Color outlineColor, bool clicked) {
		elements::checkBoxes.push_back(copyThis_);
		title_ = sf::Text(title, global::arial);
		try {
			verifyCoords(topLeftCornerCoords);
		}
		catch (std::invalid_argument& except) {
			std::cerr << except.what() << std::endl;
			return;
		}
		topLeftCornerCoords_ = topLeftCornerCoords;
		if (size < 0.f) {
			std::cerr << "Error while constructing a Checkbox instance, size was negative (" << size << ") was given." << std::endl;
			return;
		}
		backgroundColor_ = backgroundColor;
		linesColor_ = linesColor;
		outlineColor_ = outlineColor;
		clicked_ = clicked;
	}

	~CheckBox() {
		elements::checkBoxes = vectorWithoutValues(elements::checkBoxes, copyThis_);
	}

	bool clicked() const {
		return clicked_;
	}

	void click(sf::Vector2i const& mousePos = sf::Mouse::getPosition(global::window)) {
		sf::FloatRect bounds = sf::FloatRect(topLeftCornerCoords_, { size_, size_ });
		if (bounds.contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
			clicked_ = !clicked_;
		}
	}

	void move(sf::Vector2f const& newCoords) {
		topLeftCornerCoords_ = newCoords;
	}

};