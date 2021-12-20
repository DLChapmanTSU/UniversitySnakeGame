#include "PickUp.h"
#include "Snake.h"

#include <iostream>

Pickup::Pickup(sf::Vector2f p, bool isFood)
{
	m_position = p;
	
	if (isFood == true) {
		//Randomizes the value of the food and changes the colour of it accordingly
		m_color = sf::Color::Red;
		m_value = rand() % 4 + 1;
		m_isBattery = false;

		switch (m_value)
		{
		case 1:
			m_color = sf::Color::Red;
			break;
		case 2:
			m_color = sf::Color::Green;
			break;
		case 3:
			m_color = sf::Color::Blue;
			break;
		case 4:
			m_color = sf::Color::Magenta;
			break;
		case 5:
			m_color = sf::Color::White;
			break;
		default:
			break;
		}
	}
	else {
		m_color = sf::Color::Cyan;
		m_value = 0;
		m_isBattery = true;
	}
}

void Pickup::Render(sf::RenderWindow& w)
{
	//Will not render if the pickup is inactive
	if (m_isActive == true) {
		//Renders the pickup based on the the values passed into it
		sf::RectangleShape rectangle(m_size);
		rectangle.setPosition(m_position);
		rectangle.setFillColor(m_color);
		rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height / 2);

		w.draw(rectangle);
	}
}

void Pickup::Spawn(std::vector<Pickup>& pickups, std::vector<std::shared_ptr<Snake>>& snakes)
{
	srand((unsigned int)time(NULL));

	bool spawnPointFound{ true };
	sf::Vector2f spawnCandidate;

	do
	{
		//Generates a random grid position
		spawnPointFound = true;
		spawnCandidate = sf::Vector2f((float)(rand() % 78 + 1) * 10, (float)(rand() % 58 + 1) * 10);

		//Loops through every pickup to see if any of them are in the generated position
		for (Pickup& p : pickups) {
			if (p.GetPosition() == spawnCandidate) {
				spawnPointFound = false;
				break;
			}
		}

		//Loops through every snake segment to see if any of them are in the generated position
		if (spawnPointFound == true) {
			for (std::shared_ptr<Snake>& s : snakes) {
				for (int i = 0; i < s->GetTail().Size(); i++) {
					std::shared_ptr<Segment> t = s->GetTail().GetNode(i);
					if (t->GetPosition() == spawnCandidate) {
						spawnPointFound = false;
						std::cout << "Occupied by player" << std::endl;
						break;
					}
				}
			}
			
		}
		//The loop continues until an unoccupied position is generated
	} while (spawnPointFound == false);

	m_position = spawnCandidate;

	if (m_isBattery == false) {
		//Gives the food a new value and colour
		m_value = rand() % 4 + 1;

		switch (m_value)
		{
		case 1:
			m_color = sf::Color::Red;
			break;
		case 2:
			m_color = sf::Color::Green;
			break;
		case 3:
			m_color = sf::Color::Blue;
			break;
		case 4:
			m_color = sf::Color::Magenta;
			break;
		case 5:
			m_color = sf::Color::White;
			break;
		default:
			break;
		}
	}
	
	std::cout << m_position.x << " " << m_position.y << std::endl;
}

void Pickup::SetActive(bool active)
{
	m_isActive = active;
	std::cout << "SetActive called" << std::endl;
	if (m_isActive == true) {
		std::cout << "Active" << std::endl;
	}
	else {
		std::cout << "Inactive" << std::endl;
	}
	std::cout << m_isActive << std::endl;
}
