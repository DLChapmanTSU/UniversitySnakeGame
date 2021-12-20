#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Text {
private:
	std::string m_text;
	sf::Color m_color;
	unsigned int m_size;
	sf::Font m_font;
	sf::Vector2f m_position;
	bool m_snapToMiddle;
public:
	Text(sf::Color c = sf::Color::White, unsigned int s = 10, std::string t = "Test", sf::Vector2f p = sf::Vector2f(400,300), bool m = false);
	void SetText(std::string t = "Test");
	void Render(sf::RenderWindow& w);
};
