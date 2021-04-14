#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <functional>
#include <random>
#include <chrono>
#include <array>
#include <memory>
#include <type_traits> // enable_if, conjuction

class CheckBox;
class TextBox;
class Line;
class Button;

namespace unicodeSpecialValues {
	constexpr short unsigned int backspace = 8u;
	constexpr short unsigned int ctrlC = 3u;
	constexpr short unsigned int ctrlX = 24u;
	constexpr short unsigned int ctrlV = 22u;
	constexpr short unsigned int ctrlZ = 26u;
	constexpr short unsigned int ctrlY = 25u;
}

class ButtonNotClickedError : public std::exception {
	std::string message_ = "";

public:
	ButtonNotClickedError(std::string const& message) : message_(message) {}

	virtual const char* what() const noexcept {
		return message_.c_str();
	}
};

namespace TextBoxValue {
	sf::String textInMemory = "";
}

namespace global {
	sf::RenderWindow window{ sf::VideoMode(1920, 1080), "Class Generator" };
	sf::Font arial;
	sf::Event event;
}

namespace elements {
	std::vector<CheckBox*> checkBoxes;
	std::vector<TextBox*> textBoxes;
	std::vector<Button*> buttons;
	std::vector<Line*> lines;
}

namespace angleConvert {
	float radToDegree(float radValue) {
		constexpr float pi = 3.14159265358979323846f;
		return static_cast<float>(radValue * 180.f / pi);
	}

	float DegreeToRad(float degreeValue) {
		constexpr float pi = 3.14159265358979323846f;
		return static_cast<float>(degreeValue / 180 / pi);
	}
}

template<typename T>
void verifyCoords(sf::Vector2<T> const& coords) {
	if (coords.x < 0.f || coords.y < 0.f) {
		throw std::invalid_argument("Coordinates are bad : " + ((coords.x < 0.f) ? ("X-coordinate is negative (= " + std::to_string(coords.x) + ")") : "") + " " + ((coords.y < 0.f) ? ("Y-coordinate is negative (= " + std::to_string(coords.y) + ")") : ""));
	}
}

template<unsigned int SIZE, typename KEY, typename VALUE>
std::array<VALUE, SIZE> mapValues(std::map<KEY, VALUE> const& map) {
	std::array<VALUE, SIZE> values;
	int i = 0;
	for (std::pair<KEY, VALUE> const& pair : map) {
		values[i++] = pair.second;
	}
	return values;
}

template<typename KEY, typename VALUE>
std::vector<VALUE> mapValues(std::map<KEY, VALUE> const& map) {
	std::vector<VALUE> values;
	values.reserve(map.size());
	for (std::pair<KEY, VALUE> const& pair : map) {
		values.push_back(pair.second);
	}
	return values;
}

template<typename T>
T randomNumber(T min, T max) {
	if (min == max) {
		return min;
	}
	if (max < min) {
		std::swap(min, max);
	}
	std::mt19937 generator{ static_cast<std::mt19937::result_type>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
	std::uniform_real_distribution<T> distribution(min, max);
	return (static_cast<long long int>(distribution(generator)) % static_cast<long long int>(max - min)) + min;
}

template<typename T, typename... ARGS>
using are_same = std::conjunction<std::is_same<T, ARGS>...>;

template<typename T>
std::vector<T> vectorWithoutValues(const std::vector<T>& vector, T valueToDelete) {
	std::vector<T> result;
	result.reserve(vector.size());
	for (const T& val : vector) {
		if (val != valueToDelete) {
			result.push_back(const_cast<T&>(val));
		}
	}
	return result;
}

template<typename T, typename... ARGS, typename = std::enable_if_t<are_same<T, ARGS...>::value, void>>
std::vector<T> vectorWithoutValues(const std::vector<T>& vector, ARGS... args) {
	std::vector<T> result;
	return vectorWithoutValues(args...);
}