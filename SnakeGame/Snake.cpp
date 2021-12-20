#include "Snake.h"
#include "Game.h"
#include "PickUp.h"
#include "LinkedList.h"

#include <iostream>



Snake::Snake(sf::Vector2f p, sf::Color c, int i, sf::SoundBuffer& eatSound, sf::SoundBuffer& deathSound)
{
	m_color = c;
	m_headPosition = p;
	Segment front(this, 1, m_headPosition, true);
	m_tail.PushFront(front);
	m_direction = SnakeDirection::E_UP;
	m_backwardsDirection = SnakeDirection::E_DOWN;
	m_snakeIndex = i;
	m_eatSound.setBuffer(eatSound);
	m_deathSound.setBuffer(deathSound);
}

void Snake::Render(sf::RenderWindow& w)
{
	//Will not render if the snake is dead
	if (m_isDead == true) {
		return;
	}

	if (m_tail.Size() > 0) {
		//Creates a rectangle and renders it in the position of each tail segment
		for (int i = 0; i < m_tail.Size(); i++) {
			std::shared_ptr<Segment> s = m_tail.GetNode(i);
			sf::RectangleShape rectangle(m_size);
			rectangle.setPosition(s->GetPosition());
			rectangle.setFillColor(m_color);
			rectangle.setOrigin(rectangle.getLocalBounds().width / 2, rectangle.getLocalBounds().height / 2);

			if (m_isElectric == true) {
				rectangle.setOutlineColor(sf::Color::Cyan);
				rectangle.setOutlineThickness(2.0f);
			}

			w.draw(rectangle);
		}
	}
}

void Snake::SetDirection(SnakeDirection d)
{
	//Sets the direction the snake is moving in providing d isn't the same as the snakes current or reverse direction
	if (m_direction == d) {
		return;
	}
	else if (m_backwardsDirection == d) {
		return;
	}
	else {
		m_direction = d;
	}
}

LinkedList& Snake::GetTail()
{
	return m_tail;
}

int Snake::GetLength() const
{
	return m_tail.Size();
}

void Snake::CheckCollision(std::vector<Pickup>& pickups, std::vector<std::shared_ptr<Snake>>& snakes)
{
	//Kills the snake if the head segment reaches the edge of the screen
	if (m_headPosition.x >= 800 || m_headPosition.x <= 0 || m_headPosition.y >= 600 || m_headPosition.y <= 0) {
		m_isDead = true;
		m_deathSound.play();
		return;
	}

	//Compares the head position with the position of every pickup
	//If a match is found and the pickup provides food, the amount of segments to add will go up by the value held by the pickup
	//If the pickup is the electricity power up, the counter for how long the electricity will last is set and the bool indicating the electricity is on is set to true
	for (Pickup& p : pickups) {
		if (p.GetPosition() == m_headPosition && p.GetActive() == true) {
			m_eatSound.play();
			if (p.GetIsBattery() == false) {
				m_segmentsToAdd += p.GetValue();
				p.SetActive(false);
				std::cout << "Fruit hit" << std::endl;
			}
			else {
				m_isElectric = true;
				m_electricTicks = rand() % 5 + 5;
				p.SetActive(false);
				std::cout << "Shocking!" << std::endl;
			}
		}
	}

	//Compares the position of the head with the position of every other snake segment
	//If there is a match, the snake is killed
	//If the collision was with the head of another snake, the other snake is killed too
	for (std::shared_ptr<Snake>& s : snakes) {
		if (s->GetIsDead() == false) {
			//Segment& t : s->GetTail()
			for (int i = 0; i < s->GetTail().Size(); i++) {
				std::shared_ptr<Segment> t = s->GetTail().GetNode(i);

				if (m_headPosition == t->GetPosition() && t->GetIsHead() == false) {
					m_isDead = true;
					m_deathSound.play();
					break;
				}
				else if (m_headPosition == t->GetPosition() && t->GetIsHead() == true && s->GetIndex() != m_snakeIndex) {
					m_isDead = true;
					std::cout << "Head collision" << std::endl;
					s->SetIsDead(true);
					m_deathSound.play();
					break;
				}
			}
		}

		//Ends the loop early if a collision was detected
		if (m_isDead == true) {
			break;
		}
	}

	//Checks every segment within the range of every segment in this snake
	//If the snake has the powerup and a segment is in range, then DeleteTail is called on the snake that owns it
	//This will delete the tail segment and every segment after it (including the original segment)
	if (m_isElectric == true) {
		for (std::shared_ptr<Snake>& s : snakes) {
			int targetIndex{ 999999999 };

			if (s->GetIsDead() == false && s->GetIndex() != m_snakeIndex) {
				for (int i = 0; i < s->GetTail().Size(); i++) {
					std::shared_ptr<Segment> t = s->GetTail().GetNode(i);
					for (int j = 0; j < m_tail.Size(); j++) {
						std::shared_ptr<Segment> myt = m_tail.GetNode(j);
						sf::Vector2f magVector;
						magVector.x = myt->GetPosition().x - t->GetPosition().x;
						magVector.y = myt->GetPosition().y - t->GetPosition().y;

						if (magVector.x <= 50 && magVector.y <= 50 && magVector.x >= -50 && magVector.y >= -50) {
							std::cout << "Target Shocked" << std::endl;
							if (targetIndex > t->GetIndex()) {
								targetIndex = t->GetIndex();
							}
						}
					}
				}
			}
			
			if (targetIndex != 999999999) {
				std::cout << targetIndex << std::endl;
				s->DeleteTail(targetIndex);
			}
		}
	}
}

void Snake::DeleteTail(int i)
{
	std::cout << "Deleting to " << i << std::endl;

	bool targetSegmentHit{ false };

	//Deletes a segment from the back until the target segment is deleted
	while (targetSegmentHit == false) {
		if (m_tail.GetBackIndex() != i) {
			m_tail.PopBack();
		}
		else
		{
			m_tail.PopBack();
			targetSegmentHit = true;
		}
	}

	//If this function leaves the tail list empty, the snake is killed
	if (m_tail.Size() == 0) {
		m_isDead = true;
		m_deathSound.play();
	}
}

void Snake::SetIsDead(bool d)
{
	m_isDead = d;
}

Segment::Segment(Snake* s, int i, sf::Vector2f p, bool h)
{
	m_position = p;
	m_isHead = h;
	m_owner = s;
	m_index = i;
}

void Segment::SetIsHead(bool head)
{
	m_isHead = head;
}

void PlayerSnake::Update()
{
	//Will not update if the snake is dead
	if (m_isDead == true) {
		return;
	}

	//If no segments need to be added, the last tail segment is deleted and a new one is placed where the head should be based on the direction the snake is moving
	//If a segment needs to be added, the same thing happens, except the backmost segment is not deleted
	if (m_segmentsToAdd <= 0) {
		switch (m_direction)
		{
		case SnakeDirection::E_UP:
			m_headPosition.y -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_DOWN;
			break;
		case SnakeDirection::E_DOWN:
			m_headPosition.y += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_UP;
			break;
		case SnakeDirection::E_LEFT:
			m_headPosition.x -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_RIGHT;
			break;
		case SnakeDirection::E_RIGHT:
			m_headPosition.x += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_LEFT;
			break;
		default:
			break;
		}
	}
	else {
		switch (m_direction)
		{
		case SnakeDirection::E_UP:
			m_headPosition.y -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_DOWN;
			break;
		case SnakeDirection::E_DOWN:
			m_headPosition.y += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_UP;
			break;
		case SnakeDirection::E_LEFT:
			m_headPosition.x -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_RIGHT;
			break;
		case SnakeDirection::E_RIGHT:
			m_headPosition.x += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_LEFT;
			break;
		default:
			break;
		}
	}

	//Reduces the counter for the powerup by one and disables it if the counter hits 0
	if (m_electricTicks > 0) {
		m_electricTicks--;
	}
	else if (m_electricTicks == 0 && m_isElectric == true) {
		m_isElectric = false;
	}
}

AISnake::AISnake(sf::Vector2f p, sf::Color c, int i, sf::SoundBuffer& eatSound, sf::SoundBuffer& deathSound) : Snake(p, c, i, eatSound, deathSound)
{
	//Target destination is temporarily set to a random position on the map
	m_target = sf::Vector2f((float)(rand() % 79 + 1) * 10, (float)(rand() % 59 + 1) * 10);
}

void AISnake::Update()
{
	if (m_isDead == true) {
		return;
	}

	//Pushes the snakes current direction to the path stack if an error occurs and the stack ends up empty
	if (m_path.empty() == false) {
		m_direction = m_path.front();
		m_path.pop_front();
	}

	//If no segments need to be added, the last tail segment is deleted and a new one is placed where the head should be based on the direction the snake is moving
	//If a segment needs to be added, the same thing happens, except the backmost segment is not deleted
	if (m_segmentsToAdd <= 0) {
		switch (m_direction)
		{
		case SnakeDirection::E_UP:
			m_headPosition.y -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_DOWN;
			break;
		case SnakeDirection::E_DOWN:
			m_headPosition.y += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_UP;
			break;
		case SnakeDirection::E_LEFT:
			m_headPosition.x -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_RIGHT;
			break;
		case SnakeDirection::E_RIGHT:
			m_headPosition.x += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PopBack();
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_backwardsDirection = SnakeDirection::E_LEFT;
			break;
		default:
			break;
		}
	}
	else {
		switch (m_direction)
		{
		case SnakeDirection::E_UP:
			m_headPosition.y -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_DOWN;
			break;
		case SnakeDirection::E_DOWN:
			m_headPosition.y += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_UP;
			break;
		case SnakeDirection::E_LEFT:
			m_headPosition.x -= 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_RIGHT;
			break;
		case SnakeDirection::E_RIGHT:
			m_headPosition.x += 10;
			m_tail.GetNode(0)->SetIsHead(false);
			m_tail.PushFront(Segment(this, m_tail.Size() + 1, m_headPosition, true));
			m_segmentsToAdd--;
			m_backwardsDirection = SnakeDirection::E_LEFT;
			break;
		default:
			break;
		}
	}

	//Creates a temporary target position to avoid errors
	if (m_headPosition == m_target) {
		m_target = sf::Vector2f((float)(rand() % 78 + 1) * 10, (float)(rand() % 58 + 1) * 10);
	}

	//Reduces the counter for the powerup by one and disables it if the counter hits 0
	if (m_electricTicks > 0) {
		m_electricTicks--;
	}
	else if (m_electricTicks == 0 && m_isElectric == true) {
		m_isElectric = false;
	}
}

void AISnake::CheckCollision(std::vector<Pickup>& pickups, std::vector<std::shared_ptr<Snake>>& snakes)
{
	//Kills the snake if the head segment reaches the edge of the screen
	if (m_headPosition.x >= 800 || m_headPosition.x <= 0 || m_headPosition.y >= 600 || m_headPosition.y <= 0) {
		m_isDead = true;
		m_deathSound.play();
		std::cout << "I hit a wall" << std::endl;
		return;
	}

	//Compares the head position with the position of every pickup
	//If a match is found and the pickup provides food, the amount of segments to add will go up by the value held by the pickup
	//If the pickup is the electricity power up, the counter for how long the electricity will last is set and the bool indicating the electricity is on is set to true
	for (Pickup& p : pickups) {
		if (p.GetPosition() == m_headPosition && p.GetActive() == true) {
			m_eatSound.play();
			if (p.GetIsBattery() == false) {
				m_segmentsToAdd += p.GetValue();
				p.SetActive(false);
				std::cout << "Fruit hit" << std::endl;
			}
			else {
				m_isElectric = true;
				m_electricTicks = rand() % 5 + 5;
				p.SetActive(false);
				std::cout << "Shocking!" << std::endl;
			}
		}
	}

	//Compares the position of the head with the position of every other snake segment
	//If there is a match, the snake is killed
	//If the collision was with the head of another snake, the other snake is killed too
	for (std::shared_ptr<Snake>& s : snakes) {
		if (s->GetIsDead() == false) {
			for (int i = 0; i < s->GetTail().Size(); i++) {
				std::shared_ptr<Segment> t = s->GetTail().GetNode(i);
				if (m_headPosition == t->GetPosition() && t->GetIsHead() == false) {
					m_deathSound.play();
					std::cout << "My name is Snake " << m_snakeIndex << std::endl;
					std::cout << "I hit a snake tail" << std::endl;
					std::cout << "It belongs to Snake " << t->GetOwner()->GetIndex() << std::endl;
					switch (m_direction)
					{
					case SnakeDirection::E_UP:
						std::cout << "I was moving up" << std::endl;
						break;
					case SnakeDirection::E_DOWN:
						std::cout << "I was moving down" << std::endl;
						break;
					case SnakeDirection::E_LEFT:
						std::cout << "I was moving left" << std::endl;
						break;
					case SnakeDirection::E_RIGHT:
						std::cout << "I was moving right" << std::endl;
						break;
					default:
						break;
					}
					m_isDead = true;
					break;
				}
				else if (m_headPosition == t->GetPosition() && t->GetIsHead() == true && s->GetIndex() != m_snakeIndex) {
					m_deathSound.play();
					m_isDead = true;
					std::cout << "Head collision" << std::endl;
					s->SetIsDead(true);
					break;
				}
			}
		}

		if (m_isDead == true) {
			break;
		}
	}

	//Checks every segment within the range of every segment in this snake
	//If the snake has the powerup and a segment is in range, then DeleteTail is called on the snake that owns it
	//This will delete the tail segment and every segment after it (including the original segment)
	if (m_isElectric == true) {
		for (std::shared_ptr<Snake>& s : snakes) {
			int targetIndex{ 999999999 };

			if (s->GetIsDead() == false && s->GetIndex() != m_snakeIndex) {
				for (int i = 0; i < s->GetTail().Size(); i++) {
					std::shared_ptr<Segment> t = s->GetTail().GetNode(i);
					for (int j = 0; j < m_tail.Size(); j++) {
						std::shared_ptr<Segment> myt = m_tail.GetNode(j);
						sf::Vector2f magVector;
						magVector.x = myt->GetPosition().x - t->GetPosition().x;
						magVector.y = myt->GetPosition().y - t->GetPosition().y;

						//std::cout << "X: " << magVector.x << " Y: " << magVector.y << std::endl;

						if (magVector.x <= 20 && magVector.y <= 20 && magVector.x >= -20 && magVector.y >= -20) {
							std::cout << "Target Shocked" << std::endl;
							if (targetIndex > t->GetIndex()) {
								targetIndex = t->GetIndex();
							}
						}
					}
				}
			}

			if (targetIndex != 999999999) {
				std::cout << targetIndex << std::endl;
				s->DeleteTail(targetIndex);
			}
		}
	}

	//These two functions must be called in CheckCollision so they can have access to the list of snakes and pickups
	ChooseTarget(pickups);

	CalculatePath(snakes);
}

void AISnake::ChooseTarget(std::vector<Pickup>& pickups)
{
	bool targetExists{ false };
	bool allPickupsInactive{ true };

	//Checks to see if the current target still exists or if none of the food segments are active
	for (Pickup& p : pickups) {
		if (p.GetActive() == true) {
			allPickupsInactive = false;
		}

		if (p.GetActive() == true && p.GetPosition() == m_target) {
			targetExists = true;
			break;
		}
	}

	//If none of the foods are active, the function ends
	if (allPickupsInactive == true) {
		return;
	}

	//If the current target doesn't exist, a new target is chosen
	if (targetExists == false) {
		bool targetFound{ false };

		//The game generates a random number and checks that position in the food list
		//If the food in the given position is active, it becomes the new target and the loop ends
		while (targetFound == false) {
			int rng = rand() % pickups.size();

			if (pickups[rng].GetActive() == true) {
				m_target = pickups[rng].GetPosition();
				targetFound = true;
			}
		}
	}
}

void AISnake::CalculatePath(std::vector<std::shared_ptr<Snake>>& snakes)
{
	if (m_isDead == true) {
		std::cout << "I'm Dead" << std::endl;
		return;
	}

	//This array represents each position on the map
	//Every 10 pixel square in the game world is one position in the map array
	bool map[60][80];
	//Initialises every position in the map array to false
	memset(map, 0, 60 * 80 * sizeof(bool));

	//Declare where obstacles are on the map
	for (std::shared_ptr<Snake>& s : snakes) {
		for (int i = 0; i < s->GetTail().Size(); i++) {
			std::shared_ptr<Segment> t = s->GetTail().GetNode(i);
			int x = (int)t->GetPosition().x / 10;
			int y = (int)t->GetPosition().y / 10;
			map[y][x] = true;
			//Makes each square around a snake segment a obstacle as well
			if (s->GetIndex() != m_snakeIndex) {
				if (x + 1 < 80) {
					map[y][x + 1] = true;
				}

				if (x - 1 > 0) {
					map[y][x - 1] = true;
				}

				if (y + 1 < 60) {
					map[y + 1][x] = true;
				}

				if (y - 1 > 0) {
					map[y - 1][x] = true;
				}

				if (x + 1 < 80 && y + 1 < 60) {
					map[y + 1][x + 1] = true;
				}

				if (x - 1 > 0 && y + 1 < 60) {
					map[y + 1][x - 1] = true;
				}

				if (x + 1 < 80 && y - 1 > 0) {
					map[y - 1][x + 1] = true;
				}

				if (x - 1 > 0 && y - 1 > 0) {
					map[y - 1][x - 1] = true;
				}
			}
		}
	}

	int targetX = (int)m_target.x / 10;
	int targetY = (int)m_target.y / 10;

	//Stops the process if the target tile is an obstacle
	//Will then clear the path stack and look for a safe direction for the snake to move in to add to the stack
	//The function will then end
	if (map[targetY][targetX] == true) {
		m_path.clear();
		std::cout << "My path is blocked" << std::endl;

		int snakeX = (int)m_headPosition.x / 10;
		int snakeY = (int)m_headPosition.y / 10;

		std::cout << "X: " << snakeX << "Y: " << snakeY << std::endl;

		if (snakeY + 1 < 60) {
			if (map[snakeY + 1][snakeX] == false && m_direction != SnakeDirection::E_UP) {
				m_path.push_back(SnakeDirection::E_DOWN);
				std::cout << "I will go down" << std::endl;
				return;
			}
			else {
				std::cout << "Down is blocked" << std::endl;
			}
		}

		if (snakeY - 1 > 0) {
			if (map[snakeY - 1][snakeX] == false && m_direction != SnakeDirection::E_DOWN) {
				m_path.push_back(SnakeDirection::E_UP);
				std::cout << "I will go up" << std::endl;
				return;
			}
			else {
				std::cout << "Up is blocked" << std::endl;
			}
		}

		if (snakeX + 1 < 80) {
			if (map[snakeY][snakeX + 1] == false && m_direction != SnakeDirection::E_LEFT) {
				m_path.push_back(SnakeDirection::E_RIGHT);
				std::cout << "I will go right" << std::endl;
				return;
			}
			else {
				std::cout << "Right is blocked" << std::endl;
			}
		}

		if (snakeX - 1 > 0) {
			if (map[snakeY][snakeX - 1] == false && m_direction != SnakeDirection::E_RIGHT) {
				m_path.push_back(SnakeDirection::E_LEFT);
				std::cout << "I will go left" << std::endl;
				return;
			}
			else {
				std::cout << "Left is blocked" << std::endl;
			}
		}

		std::cout << "I have nowhere to go" << std::endl;
		return;
	}

	std::pair<int, int> targetPair;
	targetPair.first = (int)m_target.y;
	targetPair.second = (int)m_target.x;

	std::vector<Node> openList;
	std::vector<Node> closedList;

	//Creates a node of the snakes current position
	Node start;
	start.m_y = (int)m_headPosition.y;
	start.m_x = (int)m_headPosition.x;
	start.m_g = 0;
	start.m_h = 0;
	start.m_f = 0;

	bool endFound{ false };

	openList.push_back(start);

	while (openList.empty() == false && endFound == false) {
		Node current;
		int currentF{ 999999999 };
		int currentIndex{ 0 };
		//Loops through the open list to find the cheapest node
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i].m_f < currentF) {
				currentIndex = (int)i;
				currentF = openList[i].m_f;
			}
		}

		//Removes the cheapest node from the open list and locks it in by adding it to the closed list
		current = openList[currentIndex];
		closedList.push_back(openList[currentIndex]);
		openList.erase(openList.begin() + currentIndex);

		//Ends the loop if the current node has the same coordinates as the target position
		if (current.m_x == targetPair.second && current.m_y == targetPair.first) {
			endFound = true;
			break;
		}

		//Fills a std::vector with all the nodes adjacent to the current one
		std::vector<Node> adjacentTiles;

		if (current.m_y + 10 < 600) {
			Node temp = current;
			temp.m_y += 10;
			adjacentTiles.push_back(temp);
		}

		if (current.m_y - 10 > 0) {
			Node temp = current;
			temp.m_y -= 10;
			adjacentTiles.push_back(temp);
		}

		if (current.m_x + 10 < 800) {
			Node temp = current;
			temp.m_x += 10;
			adjacentTiles.push_back(temp);
		}

		if (current.m_x - 10 > 0) {
			Node temp = current;
			temp.m_x -= 10;
			adjacentTiles.push_back(temp);
		}

		//Makes the parent of each new node the current node
		//Calculates the g, h, and f values for each new node
		for (size_t i = 0; i < adjacentTiles.size(); i++)
		{
			adjacentTiles[i].m_parent = &current;
			adjacentTiles[i].m_g = adjacentTiles[i].m_parent->m_g + 1;
			adjacentTiles[i].m_h = ((targetPair.second - adjacentTiles[i].m_x) * (targetPair.second - adjacentTiles[i].m_x) + (targetPair.first - adjacentTiles[i].m_y) * (targetPair.first - adjacentTiles[i].m_y));
			adjacentTiles[i].m_f = adjacentTiles[i].m_g + adjacentTiles[i].m_h;
		}

		//Loops through each adjacent tile
		for (size_t i = 0; i < adjacentTiles.size(); i++)
		{
			//Divides the x and y coordinates by 10 so they can be passed into the map array
			int tempX = (adjacentTiles[i].m_x / 10);
			int tempY = (adjacentTiles[i].m_y / 10);

			//Moves on if this coordinate is blocked or if the coordinate contains any negatives
			if (tempX < 0 || tempY < 0) {
				continue;
			}

			if (map[tempY][tempX] == true) {
				continue;
			}

			//Checks through the whole closed list and moves on if the current node is already there

			bool isInClosedList{ false };

			for (size_t j = 0; j < closedList.size(); j++) {
				if (closedList[j].m_x == adjacentTiles[i].m_x && closedList[j].m_y == adjacentTiles[i].m_y) {
					isInClosedList = true;
					break;
				}
			}

			if (isInClosedList == true) {
				continue;
			}

			int nodeIndex;
			bool nodePresent{ false };

			//Checks through the whole open list to see if this node is already there

			for (size_t j = 0; j < openList.size(); j++) {
				if (openList[j].m_x == adjacentTiles[i].m_x && openList[j].m_y == adjacentTiles[i].m_y) {
					nodeIndex = (int)j;
					nodePresent = true;
					break;
				}
			}

			//If the node is in the open list and the g (distance from the start node) of the current node is smaller, it replaces the node in the open list
			//If the node isn't in the open list, it is added
			if (nodePresent == true) {
				if (adjacentTiles[i].m_g < openList[nodeIndex].m_g) {
					openList[nodeIndex].m_g = current.m_g + 1;
					openList[nodeIndex].m_parent = &current;
					openList[nodeIndex].m_h = ((targetPair.second - openList[nodeIndex].m_x) * (targetPair.second - openList[nodeIndex].m_x) + (targetPair.first - openList[nodeIndex].m_y) * (targetPair.first - openList[nodeIndex].m_y));
					openList[nodeIndex].m_f = openList[nodeIndex].m_g + openList[nodeIndex].m_h;
				}
			}
			else {
				openList.push_back(adjacentTiles[i]);
			}
		}
	}

	//Runs if the path isn't found
	//Will then clear the path stack and look for a safe direction for the snake to move in to add to the stack
	//The function will then end
	if (endFound == false) {
		std::cout << "Target Unreachable" << std::endl;

		m_path.clear();
		std::cout << "My path is blocked" << std::endl;

		int snakeX = (int)m_headPosition.x / 10;
		int snakeY = (int)m_headPosition.y / 10;

		std::cout << "X: " << snakeX << "Y: " << snakeY << std::endl;

		if (snakeY + 1 < 60) {
			if (map[snakeY + 1][snakeX] == false && m_direction != SnakeDirection::E_UP) {
				m_path.push_back(SnakeDirection::E_DOWN);
				std::cout << "I will go down" << std::endl;
				return;
			}
			else {
				std::cout << "Down is blocked" << std::endl;
			}
		}

		if (snakeY - 1 > 0) {
			if (map[snakeY - 1][snakeX] == false && m_direction != SnakeDirection::E_DOWN) {
				m_path.push_back(SnakeDirection::E_UP);
				std::cout << "I will go up" << std::endl;
				return;
			}
			else {
				std::cout << "Up is blocked" << std::endl;
			}
		}

		if (snakeX + 1 < 80) {
			if (map[snakeY][snakeX + 1] == false && m_direction != SnakeDirection::E_LEFT) {
				m_path.push_back(SnakeDirection::E_RIGHT);
				std::cout << "I will go right" << std::endl;
				return;
			}
			else {
				std::cout << "Right is blocked" << std::endl;
			}
		}

		if (snakeX - 1 > 0) {
			if (map[snakeY][snakeX - 1] == false && m_direction != SnakeDirection::E_RIGHT) {
				m_path.push_back(SnakeDirection::E_LEFT);
				std::cout << "I will go left" << std::endl;
				return;
			}
			else {
				std::cout << "Left is blocked" << std::endl;
			}
		}

		std::cout << "I have nowhere to go" << std::endl;
		return;
	}

	//If a path is found, the algorithm will run through each node in the closed list
	//It will then figure out which direction the snake must travel from the last node to get to the next node. This will be pushed onto the path stack
	m_path.clear();
	Node lastNode = closedList[0];

	for (size_t i = 1; i < closedList.size(); i++)
	{
		if (closedList[i].m_y < lastNode.m_y) {
			m_path.push_back(SnakeDirection::E_UP);
		}
		else if (closedList[i].m_y > lastNode.m_y) {
			m_path.push_back(SnakeDirection::E_DOWN);
		}
		else if (closedList[i].m_x < lastNode.m_x) {
			m_path.push_back(SnakeDirection::E_LEFT);
		}
		else if (closedList[i].m_x > lastNode.m_x) {
			m_path.push_back(SnakeDirection::E_RIGHT);
		}
	}
}
