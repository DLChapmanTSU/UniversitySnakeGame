#pragma once

#include <SFML/Graphics.hpp>

class Text;

class Button {
private:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::Color m_color;
	std::shared_ptr<Text> m_text;
public:
	Button(sf::Vector2f p, sf::Vector2f s, sf::Color c = sf::Color(255,255,255,255), std::string t = "Error");
	void Render(sf::RenderWindow& w);
	bool CheckClick(sf::Vector2f m);
};