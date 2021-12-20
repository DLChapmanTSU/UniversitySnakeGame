#include "Button.h"
#include "Text.h"

Button::Button(sf::Vector2f p, sf::Vector2f s, sf::Color c, std::string t)
{
	m_position = p;
	m_size = s;
	m_color = c;
	//Uses a pointer to a Text object so the button can have text over it
	m_text = std::make_shared<Text>(sf::Color::Red, 20, t, p, true);
}

void Button::Render(sf::RenderWindow& w)
{
	//Renders the button with the values passed in
	sf::RectangleShape rectangle(m_size);

	rectangle.setPosition(m_position);
	rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height / 2);
	rectangle.setFillColor(m_color);

	w.draw(rectangle);
	m_text->Render(w);
}

//The value m should only ever be the mouse position
//Returns true if the x and y values of m are within the bounds of the button
bool Button::CheckClick(sf::Vector2f m)
{
	if (m.x > (m_position.x - (m_size.x / 2)) && m.x < (m_position.x + (m_size.x / 2))) {
		if (m.y > (m_position.y - (m_size.y / 2)) && m.y < (m_position.y + (m_size.y / 2))) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
