#pragma once
#include "include.hpp"

class Button : public sf::Drawable {
	sf::Text title_ = sf::Text("Button " + std::to_string(elements::buttons.size() + 1), global::arial);
	sf::Vector2f coords_ = { 100.f, 100.f };
	sf::Vector2f size_ = { 100.f, 25.f };
	sf::Color backgroundColor_ = sf::Color::White;
	sf::Color outlineColor_ = sf::Color::Black;
	Button* copyThis_ = this;	// this pointer will change, so I store it at the construction

	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const {
		sf::RectangleShape button{ size_ };
		button.move(coords_);
		button.setFillColor(backgroundColor_);
		button.setOutlineThickness(2.f);
		button.setOutlineColor(outlineColor_);
		window.draw(button);
	}

public:

	Button() {
		elements::buttons.push_back(const_cast<Button*>(copyThis_));
	}

	Button(sf::Text title, sf::Vector2f const& coords, sf::Vector2f const& size, sf::Color backgroundColor, sf::Color outlineColor) {
		elements::buttons.push_back(const_cast<Button*>(copyThis_));
		title_ = title;
		try {
			verifyCoords(coords);
		}
		catch (std::invalid_argument& except) {
			std::cerr << except.what() << std::endl;
			return;
		}
		coords_ = coords;
		try {
			verifyCoords(size);
		}
		catch (std::invalid_argument& except) {
			std::cerr << except.what() << std::endl;
			return;
		}
		size_ = size;
		backgroundColor_ = backgroundColor;
		outlineColor_ = outlineColor;
	}

	~Button() {
		elements::buttons = vectorWithoutValues(elements::buttons, copyThis_);
	}

	bool clicked(sf::Vector2i const& mousePos = sf::Mouse::getPosition(global::window)) const {
		sf::FloatRect bounds = sf::FloatRect(coords_, size_);
		return bounds.contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
	}

	template<typename RETURN, typename... ARGS>
	RETURN doFunction(std::function<RETURN(ARGS...)> function, ARGS... args) const {
		if (clicked()) {
			return function(args);
		}
		throw ButtonNotClickedError("'" + title_.getString() + "' wasn't clicked !");
	}

	template<typename RETURN>
	RETURN doFunction(std::function<RETURN()> function) const {
		if (clicked()) {
			return function();
		}
		throw ButtonNotClickedError("'" + title_.getString() + "' wasn't clicked !");
	}

	template<typename... ARGS>
	void doFunction(std::function<void(ARGS...)> function, ARGS... args) const {
		if (clicked()) {
			return function(args);
		}
		throw ButtonNotClickedError("'" + title_.getString() + "' wasn't clicked !");
	}

	void setBackgroundColor(sf::Color newBackgroundColor) {
		backgroundColor_ = newBackgroundColor;
	}

	void setOutlineColor(sf::Color newOutlineColor) {
		outlineColor_ = newOutlineColor;
	}

	void move(sf::Vector2f const& newCoords) {
		coords_ = newCoords;
	}

	void setTitle(sf::Text newTitle) {
		title_ = newTitle;
	}

	void setSize(sf::Vector2f const& newSize) {
		size_ = newSize;
	}

};