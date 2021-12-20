#include "Background.h"

#include <iostream>

Background::Background(std::string t)
{
	//Prints an error message if the texture failed to load
	if (!m_texture.loadFromFile(t)) {
		std::cout << "Texture not found" << std::endl;
	}
}

void Background::Render(sf::RenderWindow& w)
{
	//Renders the background with the values passed in
	sf::RectangleShape rectangle(m_size);

	rectangle.setTexture(&m_texture);
	rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height / 2);
	rectangle.setPosition(sf::Vector2f(400, 300));
	rectangle.setFillColor(sf::Color(255, 255, 255, 100));

	w.draw(rectangle);
}
