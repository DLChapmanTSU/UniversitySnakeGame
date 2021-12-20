#pragma once

#include <SFML/Graphics.hpp>

class Background {
private:
	const sf::Vector2f m_size{ sf::Vector2f(800,600) };
	sf::Texture m_texture;
public:
	Background(std::string t);
	void Render(sf::RenderWindow& w);
};
