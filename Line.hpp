#pragma once
#include "include.hpp"

class Line : public sf::Drawable {
	float length_ = 0.f;
	float angle_ = 0.f;
	mutable float width_ = 2.f;
	sf::Vector2f coords1_ = { 0.f, 0.f };
	sf::Vector2f coords2_ = { 0.f, 0.f };
	mutable sf::Color color_ = sf::Color::Black;
	Line* copyThis_ = this;

	float calculateLength() const {
		return std::sqrtf(std::powf(coords2_.x - coords1_.x, 2) + std::powf(coords2_.y - coords1_.y, 2));
		// The length of a vector AB is equal to √[(x2+x1)²+(y2-y1)²]
	}

	float calculateAngle() const {
		/*		   C____ B
				   |	/
				   |   /
				   |  /
				   | /
				   |/
				   A

				   AB is the vector, ABC is a right-angled triangle in C, so the Â = arcsin(CB/BA)
				   C has as coordinates (Xa, Yb)
		*/
		const sf::Vector2f point = { coords1_.x, coords2_.y };
		const float CB = std::sqrtf(std::powf(coords2_.x - point.x, 2) + std::powf(coords2_.y - point.y, 2));	// CB
		const float sine = CB / length_;
		const float positiveAngle = angleConvert::radToDegree(std::asinf(sine));	// std::asin returns a result in radians
		if (coords2_.x < coords1_.x) {	// we must inverse the sign of the value
			return -positiveAngle;
		}
		return positiveAngle;
	}

	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const {
		sf::RectangleShape line;
		line.setFillColor(color_);
		line.setSize({ std::sqrtf(std::powf(coords2_.x - coords1_.x, 2) + std::powf(coords2_.y - coords1_.y, 2)), width_ });
		line.setRotation(90.f - angle_);
		line.move(coords1_);
		window.draw(line);

		/*
		A________________________D
		|\
		| \
		|__\
		C   B

		angle_ = CÂB
		originally, the line is oriented as [AD], so we turn it of BÂD, which is 90 (= CÂD) - BÂD

		*/

	}

public:

	Line() {
		elements::lines.push_back(copyThis_);
	}

	Line(sf::Vector2f const& coords1 = { 0.f, 0.f }, sf::Vector2f const& coords2 = { 0.f, 0.f }, float width = 2.f, sf::Color color = sf::Color::Black) {
		elements::lines.push_back(copyThis_);
		try {
			verifyCoords(coords1);
		}
		catch (std::invalid_argument& except) {
			std::cerr << except.what() << std::endl;
			return;
		}
		try {
			verifyCoords(coords2);
		}
		catch (std::invalid_argument& except) {
			std::cerr << except.what() << std::endl;
			return;
		}
		if (width < 0.f) {
			std::cerr << "Error while constructing a Line instance, width was negative (" << width << ") was given." << std::endl;
			return;
		}
		coords1_ = coords1;
		coords2_ = coords2;
		width_ = width;
		length_ = calculateLength();
		angle_ = calculateAngle();
		color_ = color;
	}

	~Line() {
		elements::lines = vectorWithoutValues(elements::lines, copyThis_);
	}

	float getLength() const {
		return length_;
	}

	float getAngle() const {
		return angle_;
	}

	sf::Color getColor() const {
		return color_;
	}

	void moveStart(sf::Vector2f const& newCoords) {
		coords1_ = newCoords;
		length_ = calculateLength();
		angle_ = calculateAngle();
	}

	void moveEnd(sf::Vector2f const& newCoords) {
		coords2_ = newCoords;
		length_ = calculateLength();
		angle_ = calculateAngle();
	}

	void setWidth(float newWidth) const {
		width_ = newWidth;
	}

	void setColor(sf::Color newColor) const {
		color_ = newColor;
	}
};