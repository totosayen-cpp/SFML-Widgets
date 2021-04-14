#pragma once
#include "Button.hpp"

class TextBox : public sf::Drawable {
	Button area_ = Button(sf::Text(), coords_, size_, backgroundColor_, outlineColor_);
	mutable bool selected_ = false;
	sf::Text title_ = sf::Text("TextBox " + std::to_string(elements::textBoxes.size() + 1), global::arial);
	sf::Vector2f coords_ = { 100.f, 100.f };
	sf::Vector2f size_ = { 250.f, 100.f };
	sf::Color backgroundColor_ = sf::Color::White;
	sf::Color outlineColor_ = sf::Color::Black;
	mutable sf::Text textEntered_ = sf::Text("", global::arial);
	mutable std::array<sf::String, 50u> history_ = {};
	TextBox* copyThis_ = this;

	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const {
		Button copyArea = area_;
		copyArea.setBackgroundColor(backgroundColor_);
		copyArea.setOutlineColor(outlineColor_);
		copyArea.setSize(size_);
		copyArea.move(coords_);
		window.draw(copyArea);
		sf::Text copyTitle = title_;
		copyTitle.setCharacterSize(25u);
		copyTitle.move({ coords_.x, coords_.y - 30.f });
		copyTitle.setFillColor(outlineColor_);
		window.draw(copyTitle);
		const short unsigned int characterSize = findBestCharacterSize();
		if (characterSize != 0u) {	// don't write if it's not necessary
			sf::Text textEntered = textEntered_;
			textEntered.setCharacterSize(characterSize);
			textEntered.setFillColor(outlineColor_);
			textEntered.move(findBestTextPosition(characterSize));
			window.draw(textEntered);
		}
	}

	short unsigned int findBestCharacterSize() const {
		const float widthAllowed = size_.x - 2.f * 5.f;		// leaves 5 pixels free at the left and at the right
		const float heightAllowed = size_.y - 2.f * 5.f;	// leaves 5 pixels free at the top and at the bottom
		const unsigned int numbersOfCharacters = textEntered_.getString().getSize();
		if (numbersOfCharacters == 0u) {
			return static_cast<short unsigned int>(std::min(heightAllowed, widthAllowed));
		}
		const float sizeAccordingToWidthOnly = widthAllowed / numbersOfCharacters;
		enum class charSize { NARROW, WIDE };
		std::map<charSize, std::initializer_list<char>> table = {
			{charSize::NARROW,
			{'.', '\'', '"', '1', '{', '(', '[', '|', '`', '^', ')', ']' , '}', '°', '!', '§', ':', ';', ',', 'i', 'I', 'l', 'j', '*', '¤', '²', '³', 'f', 't', ' '}
			},
			{charSize::WIDE,
			{'m', 'M', 'w', 'W', '\t'}
			}
		};
		// with a char size of 30, narrow letters (ex : 'l') measure 1mm, normal letters (ex : 'a') 3 mm, wide letters (ex : 'm') 4mm
		// => a bigger coef means a bigger size, so values above are reversed, wide at the beginning ==> 25% biger than normal ==> 25% smaller than normal; narrow at the beginning ==> 3 times smaller than normal ==> 3 times bigger than normal
		float coef = 0.f;
		for (char c : textEntered_.getString()) {
			if (std::find(table[charSize::NARROW].begin(), table[charSize::NARROW].end(), c) != table[charSize::NARROW].end()) {
				coef += 3.f;
			}
			else if (std::find(table[charSize::WIDE].begin(), table[charSize::WIDE].end(), c) != table[charSize::WIDE].end()) {
				coef += 0.75f;
			}
			else {
				coef++;
			}
		}
		coef /= numbersOfCharacters;	// average size
		if (sizeAccordingToWidthOnly * coef > heightAllowed) {	// cannot be bigger than the height
			return static_cast<short unsigned int>(heightAllowed);
		}
		if (static_cast<float>(sizeAccordingToWidthOnly) * coef < 1.f) {
			return 0;	// too small, so if size is 1 the text will protrude from the box
		}
		return static_cast<short unsigned int>(sizeAccordingToWidthOnly * coef);
	}

	sf::Vector2f findBestTextPosition(short unsigned int characterSize) const {
		return { coords_.x + 5.f,								// leaves 5 pixels free at the beginning
			coords_.y + (size_.y - characterSize) / 2.f };		// (size_.y - characterSize) is the height remaining (top + bottom), so we divide by 2 to have only the top one
	}

public:
	TextBox() {
		elements::textBoxes.push_back(copyThis_);
	}

	TextBox(sf::Text title, sf::Vector2f const& coords, sf::Vector2f const& size, sf::Color backgroundColor, sf::Color outlineColor, sf::Text defaultTextEntered = sf::Text("", global::arial)) {
		elements::textBoxes.push_back(copyThis_);
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
		textEntered_ = defaultTextEntered;
	}

	~TextBox() {
		elements::textBoxes = vectorWithoutValues(elements::textBoxes, copyThis_);
	}

	void processKeyboardInput() const {
		if (selected_) {
			const unsigned int textEnteredLength = textEntered_.getString().getSize();
			switch (global::event.text.unicode) {
			case unicodeSpecialValues::backspace:
				if (textEnteredLength != 0) {	// erases the last character
					textEntered_ = sf::Text(textEntered_.getString().substring(0, textEnteredLength - 1), global::arial);
				}
				break;
			case unicodeSpecialValues::ctrlC:	// copies the text in memory
				TextBoxValue::textInMemory = textEntered_.getString();
				break;
			case unicodeSpecialValues::ctrlX:
				TextBoxValue::textInMemory = textEntered_.getString();
				textEntered_ = sf::Text("", global::arial);
				break;
			case unicodeSpecialValues::ctrlV:
				textEntered_ = sf::Text(textEntered_.getString() + TextBoxValue::textInMemory, global::arial);
				break;
			case unicodeSpecialValues::ctrlZ:
				// implement
				break;
			case unicodeSpecialValues::ctrlY:
				// implement
			default:
				textEntered_ = sf::Text(textEntered_.getString() + sf::Text(global::event.text.unicode, global::arial).getString(), global::arial);
				break;
			}
		}
	}

	void click() const {
		selected_ = clicked();
	}

	bool clicked(sf::Vector2i const& mousePos = sf::Mouse::getPosition(global::window)) const {
		sf::FloatRect bounds = sf::FloatRect(coords_, size_);
		return bounds.contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
	}

	void setBackgroundColor(sf::Color newBackgroundColor) {
		backgroundColor_ = newBackgroundColor;
	}

	void setOutlineColor(sf::Color newOutlineColor) {
		outlineColor_ = newOutlineColor;
	}
};