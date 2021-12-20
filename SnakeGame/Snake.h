#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>

#include "LinkedList.h"

class Segment;
enum class SnakeDirection;
class Game;
class Pickup;

class Snake {
protected:
	const sf::Vector2f m_size{ sf::Vector2f(10,10) };
	sf::Vector2f m_headPosition;
	sf::Color m_color;
	SnakeDirection m_direction;
	SnakeDirection m_backwardsDirection;
	LinkedList m_tail;
	int m_segmentsToAdd{ 5 };
	bool m_isDead{ false };
	bool m_isElectric{ false };
	int m_snakeIndex;
	int m_electricTicks{ 0 };
	sf::Sound m_eatSound;
	sf::Sound m_deathSound;
public:
	Snake(sf::Vector2f p, sf::Color c, int i, sf::SoundBuffer& eatSound, sf::SoundBuffer& deathSound);
	void Render(sf::RenderWindow& w);
	void SetDirection(SnakeDirection d);
	LinkedList& GetTail();
	int GetIndex() const { return m_snakeIndex; };
	int GetLength() const;
	bool GetIsDead() const { return m_isDead; };
	virtual void CheckCollision(std::vector<Pickup>& pickups, std::vector<std::shared_ptr<Snake>>& snakes);
	void DeleteTail(int i);
	void SetIsDead(bool d = false);
	virtual void Update() = 0;
};

class PlayerSnake : public Snake {
public:
	PlayerSnake(sf::Vector2f p, sf::Color c, int i, sf::SoundBuffer& eatSound, sf::SoundBuffer& deathSound) : Snake(p, c, i, eatSound, deathSound) { };
	void Update() override;
};

class AISnake : public Snake {
private:
	sf::Vector2f m_target;
	std::list<SnakeDirection> m_path;
public:
	AISnake(sf::Vector2f p, sf::Color c, int i, sf::SoundBuffer& eatSound, sf::SoundBuffer& deathSound);
	void Update() override;
	void CheckCollision(std::vector<Pickup>& pickups, std::vector<std::shared_ptr<Snake>>& snakes) override;
private:
	void ChooseTarget(std::vector<Pickup>& pickups);
	void CalculatePath(std::vector<std::shared_ptr<Snake>>& snakes);
};

//Class for the individual parts of the snakes tail. These are what the snake renders on the screen
class Segment {
private:
	bool m_isHead;
	sf::Vector2f m_position;
	Snake* m_owner;
	int m_index;
public:
	Segment(Snake* s, int i, sf::Vector2f p, bool h = false);
	bool GetIsHead() { return m_isHead; };
	sf::Vector2f GetPosition() { return m_position; };
	Snake* GetOwner() { return m_owner; };
	int GetIndex() { return m_index; };
	void SetIsHead(bool head);
};

enum class SnakeDirection
{
	E_UP,
	E_DOWN,
	E_LEFT,
	E_RIGHT
};

//Used during the AISnake pathfinding algorithm CalculatePath()
//Will allow the algorithm to compare different positions on the map to calculate the best path
struct Node {
	int m_x{ 0 }; //The X coordinate of the position
	int m_y{ 0 }; //The Y coordinate of the position
	int m_g{ 0 }; //The distance between this node and the start node
	int m_h{ 0 }; //The estimated distance between this node and the target position
	int m_f{ 0 }; //The total cost of the node. Calculated by g + h. This is what will decide if this node is in the quickest path
	Node* m_parent{ nullptr }; //A pointer to the parent node
};
