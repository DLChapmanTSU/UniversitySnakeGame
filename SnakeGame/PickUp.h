#pragma once
#include <SFML/Graphics.hpp>

class Snake;

//Class for both the food and battery power-up
class Pickup {
private:
	sf::Vector2f m_position;
	const sf::Vector2f m_size{ sf::Vector2f(10,10) };
	sf::Color m_color;
	bool m_isActive{ true };
	int m_value;
	bool m_isBattery;
public:
	Pickup(sf::Vector2f p, bool isFood = true);
	void Render(sf::RenderWindow& w);
	sf::Vector2f GetPosition() { return m_position; };
	bool GetActive() const { return m_isActive; };
	int GetValue() const { return m_value; };
	bool GetIsBattery() const { return m_isBattery; };
	void Spawn(std::vector<Pickup>& pickups, std::vector<std::shared_ptr<Snake>>& snakes);
	void SetActive(bool active);
};