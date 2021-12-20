#include "Text.h"

#include <iostream>

Text::Text(sf::Color c, unsigned int s, std::string t, sf::Vector2f p, bool m)
{
	m_color = c;
	m_size = s;
	m_text = t;
	m_position = p;
	m_snapToMiddle = m;
	
	//Loads the font and prints an error message if it fails
	if (!m_font.loadFromFile("VCR_OSD_MONO_1.001.ttf")) {
		std::cout << "Error loading font" << std::endl;
	}
}

void Text::SetText(std::string t)
{
	m_text = t;
}

void Text::Render(sf::RenderWindow& w)
{
	//Renders the text onto the screen based on the values passed in
	sf::Text t;
	t.setFont(m_font);
	t.setFillColor(m_color);
	t.setPosition(m_position);
	t.setCharacterSize(m_size);
	t.setString(m_text);

	if (m_snapToMiddle == true) {
		t.setOrigin(t.getLocalBounds().width / 2, t.getLocalBounds().height / 2);
	}

	w.draw(t);
}
