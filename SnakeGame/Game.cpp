#include "Game.h"
#include "Snake.h"
#include "PickUp.h"
#include "Text.h"
#include "Background.h"
#include "Button.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

Game::Game()
{
    m_scoreboard = std::make_shared<Text>(sf::Color::Red, 20, "Scoreboard", sf::Vector2f(550, 10));
    m_timer = std::make_shared<Text>(sf::Color::Red, 20, "Timer", sf::Vector2f(300, 10));
    m_menuMessage = std::make_shared<Text>(sf::Color::Red, 50, "Press Space To Start", sf::Vector2f(100, 300));
    m_winnerMessage = std::make_shared<Text>(sf::Color::Red, 30, "Test", sf::Vector2f(400, 100), true);

    int highscore{ 0 };
	std::ifstream read;
	read.open("HighScore.txt");

	if (!read.fail()) {
		read >> highscore;
	}

	read.close();

	m_highscoreText = std::make_shared<Text>(sf::Color::Red, 30, "Current Highscore: " + std::to_string(highscore), sf::Vector2f(400, 500), true);
    m_gameBackground = std::make_shared<Background>("SnakeGameBG2.jpg");
	m_startButton = std::make_shared<Button>(sf::Vector2f(400, 350), sf::Vector2f(200, 100), sf::Color(255, 255, 255, 255), "Start");
	m_quitButton = std::make_shared<Button>(sf::Vector2f(750, 550), sf::Vector2f(100, 100), sf::Color(255, 255, 255, 255), "Quit");

    if (!m_backgroundMusic.openFromFile("SnakeBGM.wav")) {
        std::cout << "Music Not Found" << std::endl;
    }

    if (!m_eatSound.loadFromFile("eat.wav")) {
        std::cout << "Eat Sound Not Found" << std::endl;
    }

    if (!m_deathSound.loadFromFile("death.wav")) {
        std::cout << "Death Sound Not Found" << std::endl;
    }
}

void Game::Run()
{
    //Sets up window for the game and starts playing the background music
    ScoreBoard();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;
    sf::RenderWindow window(sf::VideoMode(800, 600), "C++ Snake ICA : U0018197", sf::Style::Default, settings);
    m_backgroundMusic.play();
    m_backgroundMusic.setLoop(true);

    // Main loop that continues until we call window.close()
    while (window.isOpen())
    {
        // Handle any pending SFML events
        // These cover keyboard, mouse,joystick etc.
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                //Handles key inputs that control the player snakes, which are always at positions 0 and 1 in the list
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    m_snakes[0]->SetDirection(SnakeDirection::E_UP);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    m_snakes[0]->SetDirection(SnakeDirection::E_DOWN);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    m_snakes[0]->SetDirection(SnakeDirection::E_LEFT);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    m_snakes[0]->SetDirection(SnakeDirection::E_RIGHT);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    m_snakes[1]->SetDirection(SnakeDirection::E_UP);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    m_snakes[1]->SetDirection(SnakeDirection::E_DOWN);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    m_snakes[1]->SetDirection(SnakeDirection::E_LEFT);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    m_snakes[1]->SetDirection(SnakeDirection::E_RIGHT);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (m_gameRunning == false) {
                        //Starts the game if left mouse button is clicked and it is hovering over the start button
						bool isClicked = m_startButton->CheckClick(sf::Vector2f(sf::Mouse::getPosition(window)));
						std::cout << sf::Vector2f(sf::Mouse::getPosition(window)).x << " " << sf::Vector2f(sf::Mouse::getPosition(window)).y << std::endl;
						if (isClicked == true) {
							std::cout << "Button Pressed" << std::endl;
                            //Starting the game involves resetting all of the clocks and spawning in new snakes and pickups
							m_gameRunning = true;
							m_startTime = clock();
							m_currentTime = clock();
							m_updateTime = clock();
                            m_backgroundMusic.stop();

							if (m_snakes.empty() == false) {
								while (m_snakes.empty() == false) {
									m_snakes.pop_back();
								}
							}

							if (m_pickups.empty() == false) {
								while (m_pickups.empty() == false) {
									m_pickups.pop_back();
								}
							}

							for (size_t i = 0; i < 5; i++)
							{
								m_pickups.push_back(Pickup(sf::Vector2f((float)(rand() % 79 + 1) * 10, (float)(rand() % 59 + 1) * 10)));
							}

							m_pickups.push_back(Pickup(sf::Vector2f((float)(rand() % 79 + 1) * 10, (float)(rand() % 59 + 1) * 10), false));

							m_snakes.push_back(std::make_shared<PlayerSnake>(PlayerSnake(sf::Vector2f(500, 500), sf::Color::Blue, 0, m_eatSound, m_deathSound)));
							m_snakes.push_back(std::make_shared<PlayerSnake>(PlayerSnake(sf::Vector2f(200, 200), sf::Color::Yellow, 1, m_eatSound, m_deathSound)));
							m_snakes.push_back(std::make_shared<AISnake>(AISnake(sf::Vector2f(600, 400), sf::Color::Green, 2, m_eatSound, m_deathSound)));
							m_snakes.push_back(std::make_shared<AISnake>(AISnake(sf::Vector2f(200, 500), sf::Color::White, 3, m_eatSound, m_deathSound)));
							m_snakes.push_back(std::make_shared<AISnake>(AISnake(sf::Vector2f(100, 100), sf::Color::Magenta, 4, m_eatSound, m_deathSound)));
							m_snakes.push_back(std::make_shared<AISnake>(AISnake(sf::Vector2f(100, 400), sf::Color::Red, 5, m_eatSound, m_deathSound)));
						}
						else {
							std::cout << "Button Missed" << std::endl;
						}

						isClicked = m_quitButton->CheckClick(sf::Vector2f(sf::Mouse::getPosition(window)));

                        //Close the window if the player clicks on the quit button
						if (isClicked == true) {
							window.close();
						}
						else {
							std::cout << "Button Missed" << std::endl;
						}
					}
                }
                break;
            default:
                break;
            }
        }

        // We must clear the window each time around the loop
        window.clear();

        //Updates the game state if the game is running
        if (m_gameRunning == true) {
            MasterUpdate();
        }
        
        //Displays all active objects on the screen
        MasterRender(window);

        // Get the window to display its contents
        window.display();
    }
}

void Game::MasterUpdate()
{
    //Updates the clocks to keep track of and display the time
	m_currentTime = clock();
	float timeElapsed = float(m_currentTime - m_updateTime) / CLOCKS_PER_SEC;
    float gameTimer = float(m_currentTime - m_startTime) / CLOCKS_PER_SEC;

    std::ostringstream ss;
    ss << gameTimer;

    m_timer->SetText("Time Left: " + ss.str());

    //Ends the match and reveal the menu if the time limit is hit or 1 or less snakes remain
    if (gameTimer >= 90.0f || CheckEarlyGameOver() == true) {
        m_gameRunning = false;
        m_backgroundMusic.play();
        m_backgroundMusic.setLoop(true);
		if (m_winningSnakeIndex == 0 || m_winningSnakeIndex == 1) {
			SaveHighscore(m_winningSnakeScore);
		}
        return;
    }

    //Updates the position of each snake and checks for collisions if 0.2 seconds has elapsed
	if (timeElapsed >= 0.2f) {
        for (std::shared_ptr<Snake>& s : m_snakes) {
            if (s->GetIsDead() == false) {
                s->Update();
                s->CheckCollision(m_pickups, m_snakes);
            }
        }
		m_updateTime = clock();
        m_updatesSinceSpawn++;
        //If 50 or more updates have passed since the last attempt to spawn food, the game will attempt to spawn food
        if (m_updatesSinceSpawn >= 50) {
            SpawnFood();
            m_updatesSinceSpawn = 0;
        }

        //Instantly respawns the electric pickup if it isn't currently active
        //The electric pickup is always in the 6th slot (index 5) of the pickups vector
        if (m_pickups[5].GetActive() == false) {
            m_pickups[5].Spawn(m_pickups, m_snakes);
            m_pickups[5].SetActive(true);
        }

        
        //Updates the scoreboard
        ScoreBoard();
	}
}

void Game::MasterRender(sf::RenderWindow& w)
{
    //Renders the snakes, fruit, scoreboard, timer and the background
    m_gameBackground->Render(w);

    for (std::shared_ptr<Snake>& s : m_snakes) {
        s->Render(w);
    }

    for (Pickup p : m_pickups) {
        p.Render(w);
    }

    m_scoreboard->Render(w);
    m_timer->Render(w);

    //If the game isn't running, the menu ui is rendered
    if (m_gameRunning == false) {
		m_startButton->Render(w);
		m_quitButton->Render(w);
		m_highscoreText->Render(w);
        if (m_winningSnakeScore > 0) {
            std::string winText = "Snake " + std::to_string(m_winningSnakeIndex) + " Won With A Score Of " + std::to_string(m_winningSnakeScore);
            m_winnerMessage->SetText(winText);
            m_winnerMessage->Render(w);
        }
    }
}

void Game::SpawnFood()
{
    bool foodSpawned{ false };
    //Loops until a pickup in the list is found that is inactive
    //Spawns the pickup if an inactive, non-battery pickup is found
    for (Pickup& p : m_pickups) {
        if (p.GetActive() == false && foodSpawned == false && p.GetIsBattery() == false) {
            p.Spawn(m_pickups, m_snakes);
            foodSpawned = true;
            p.SetActive(true);
            break;
        }
    }
}

void Game::ScoreBoard()
{
    //Creates an array of pairs
    //Eack pair contains the index and score of a snake currently in the game
    std::vector<std::pair<int, int>> scores;

    for (std::shared_ptr<Snake>& s : m_snakes) {
        if (s->GetIsDead() == false) {
            scores.push_back(std::make_pair(s->GetLength(), s->GetIndex()));
        }
    }

    //Sorts the list of pairs by score in descending order
    for (size_t i = 0; i < scores.size(); i++)
    {
        for (size_t j = 0; j < scores.size() - i; j++)
        {
            if (j == scores.size() - 1) {
                break;
            }
            else if (scores[j].first <= scores[j + 1].first) {
                std::pair<int, int> temp;
                temp.first = scores[j].first;
                temp.second = scores[j].second;
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    //Creates a string to pass to the Text object for the scoreboard
    //Contains the index and score of each active snake in the correct order
    std::string scoreboardText{ "Scores:\n" };

    for (size_t i = 0; i < scores.size(); i++)
    {
        scoreboardText += "Snake: " + std::to_string(scores[i].second) + " Score: " + std::to_string(scores[i].first) + "\n";
    }

    m_scoreboard->SetText(scoreboardText);

    //Keeps a track of the snake with the highest score along with the score itself
    if (scores.size() > 0) {
        m_winningSnakeIndex = scores[0].second;
        m_winningSnakeScore = scores[0].first;
    }
}

bool Game::CheckEarlyGameOver()
{
    int livingSnakes{ 0 };

    //Loops through the list of snakes and counts how many of them are still alive
    for (size_t i = 0; i < m_snakes.size(); i++)
    {
        if (m_snakes[i]->GetIsDead() == false) {
            livingSnakes++;
        }

        //Stops the loop early if more than one living snake is found, meaning there is no winner yet
        if (livingSnakes > 1) {
            break;
        }
    }

    //If 1 or less snakes are counted, then the round has ended early
    if (livingSnakes <= 1) {
        return true;
    }

    return false;
}

void Game::SaveHighscore(int s)
{
    //Opens up text document containing the highscore
	std::ifstream read;
	int currentHighScore{ 0 };

	read.open("HighScore.txt");

    //Reads in the highscore if the file is found
	if (!read.fail()) {
		read >> currentHighScore;
	}

	read.close();

    //If the score from the last round is larger than the current highscore, the new highscore is written to the file
	if (s > currentHighScore) {
		std::ofstream write;
		write.open("HighScore.txt");

		if (!write.fail()) {
			write << s;
		}

        m_highscoreText->SetText("Current Highscore: " + std::to_string(s));
	}
}
