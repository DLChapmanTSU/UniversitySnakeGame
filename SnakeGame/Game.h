#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>

class Snake;
class Pickup;
class Text;
class Background;
class Button;

//Class which acts as a central hub to manage all aspects of the game
class Game {
private:
	std::vector<std::shared_ptr<Snake>> m_snakes;
	std::vector<Pickup> m_pickups;
	clock_t m_startTime;
	clock_t m_currentTime;
	clock_t m_updateTime;
	int m_updatesSinceSpawn{ 0 };
	std::shared_ptr<Text> m_scoreboard;
	std::shared_ptr<Text> m_timer;
	std::shared_ptr<Text> m_menuMessage;
	std::shared_ptr<Text> m_winnerMessage;
	std::shared_ptr<Text> m_highscoreText;
	bool m_gameRunning{ false };
	int m_winningSnakeIndex{ 0 };
	int m_winningSnakeScore{ 0 };
	std::shared_ptr<Background> m_gameBackground;
	std::shared_ptr<Button> m_startButton;
	std::shared_ptr<Button> m_quitButton;
	sf::Music m_backgroundMusic;
	sf::SoundBuffer m_eatSound;
	sf::SoundBuffer m_deathSound;
public:
	Game();
	void Run();
private:
	void MasterUpdate();
	void MasterRender(sf::RenderWindow& w);
	void SpawnFood();
	void ScoreBoard();
	bool CheckEarlyGameOver();
	void SaveHighscore(int s);
};
