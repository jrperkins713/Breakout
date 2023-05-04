#include <iostream>
#include <string>
#include "model.hpp"
#include <vector>
#include <SFML/Audio.hpp>

using namespace std;
void drawScore(sf::RenderWindow& window, int score, const sf::Font& font);
void drawLives(sf::RenderWindow& window, int lives, const sf::Font& font);
std::vector<cs3520::Block> resetBlocks();
void drawGameOver(sf::RenderWindow& window, int score, sf::Font font);
void drawCountDown(sf::RenderWindow& window, int count, const sf::Font& font);

int main() {
    cs3520::Paddle player;
    cs3520::Ball ball;
    sf::Clock gameClock;
    const int DELAY_TIME = 3;
	sf::RenderWindow window;
    window.create(sf::VideoMode(500, 500), "Super Fun Game!", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i(200,200));
    bool left = false;
    bool right = false;
    bool gameOver = false;
    bool muteSound = false;
    int score = 0;
    int lives = 3;
    sf::Font arial;
    sf::Music music;
    sf::SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("sounds/bounce.wav");
    sf::Sound bounceSound(hitBuffer);
    if(!arial.loadFromFile("arial.ttf")) {
        cout << "no font" << endl;
    }
    if(!music.openFromFile("sounds/synth_loop.wav")) {
        cout << "no music" << endl;
    }
    music.setLoop(true);
    music.setVolume(50);
    music.play();

    std::vector<cs3520::Block> blocks = resetBlocks();
	while (window.isOpen()) {
		// Event processing
		sf::Event event;
		while (window.pollEvent(event)) {
            
			// Request for closing the window
			switch (event.type) {
				// window closed
                
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case 71:
                            left = true;
                            break;
                        case 72:
                            right = true;
                            break;
                        case 17:
                            gameOver = false;
                            lives = 3;
                            score = 0;
                            blocks = resetBlocks();
                            ball = cs3520::Ball();
                            player = cs3520::Paddle();
                            gameClock.restart();
                            if(!muteSound) {
                                music.play();
                            }
                            
                            
                            break;
                        case 12:
                            muteSound = !muteSound;
                            muteSound ? music.stop(): music.play();
                            
                            break;
                        default:
                            break;
                    }

					break;

                case sf::Event::KeyReleased:
                    switch (event.key.code)
                    {
                        case 71:
                            left = false;
                            break;
                        case 72:
                            right = false;
                            break;
                        default:
                            break;
                    }
					break;
				default:
                    break;
			}
		}
        window.clear();
        // update game
        if(!gameOver && gameClock.getElapsedTime().asSeconds() >= DELAY_TIME) {
            bool ballHit;
            player.move(left, right);
            
            ballHit = ball.move(player);
            if(ballHit) {
                bounceSound.play();
            }
            for(auto iter = blocks.begin(); iter != blocks.cend();) {
                
                if(ball.collision(*iter)) {
                    ball.handleBlockCollision(*iter);
                    blocks.erase(iter);
                    score += iter->getScore();
                    bounceSound.play();
                    
                } else {
                    ++iter;
                }
                
            }
            if(blocks.size() == 0) {
                gameClock.restart();
                blocks = resetBlocks();
                ball = cs3520::Ball();
                player = cs3520::Paddle(player.getWidth() * .75);
            }
            if(ball.getPos().second > 550) {
                gameClock.restart();
                --lives;
                ball = cs3520::Ball();
                if (lives <= 0) {
                    gameOver = true;
                    music.stop();
                }
            }
        }

        // display game
        if(!gameOver) {
            if (gameClock.getElapsedTime().asSeconds() < DELAY_TIME) {
                drawCountDown(window, DELAY_TIME - gameClock.getElapsedTime().asSeconds() + 1, arial);
            }
            drawScore(window, score, arial);
            drawLives(window, lives, arial);
            for(auto iter = blocks.begin(); iter != blocks.cend(); ++iter) {
                iter->draw(window);           
            }
            player.draw(window);
            ball.draw(window);
            
        } else {
            drawGameOver(window, score, arial);
        }
        window.display();

	}
	return 0;
}

void drawScore(sf::RenderWindow& window, int score, const sf::Font& font) {
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::Red);
    window.draw(scoreText);
}

void drawCountDown(sf::RenderWindow& window, int count, const sf::Font& font) {
    sf::Text countDown;
    countDown.setFont(font);
    countDown.setString(std::to_string(count));
    countDown.setCharacterSize(50);
    countDown.setFillColor(sf::Color::Red);
    countDown.setPosition(241, 200);
    window.draw(countDown);

    sf::Text muteText;
    muteText.setFont(font);
    muteText.setString("Press M to mute music");
    muteText.setCharacterSize(20);
    muteText.setFillColor(sf::Color::Red);
    muteText.setPosition(150, 250);
    window.draw(muteText);
}

void drawLives(sf::RenderWindow& window, int lives, const sf::Font& font) {
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Lives: " + std::to_string(lives));
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(400, 0);
    window.draw(scoreText);
    
}

std::vector<cs3520::Block> resetBlocks() {
    std::vector<cs3520::Block> blocks;
    sf::Color colorLevels[5] = {sf::Color::Magenta, sf::Color(255, 95, 31), 
                                sf::Color::Yellow, sf::Color::Green,
                                sf::Color::Cyan};
    for(int j = 0; j < 5; j++) {
        for(int i = 0; i < 7; i++) {
            blocks.push_back(cs3520::Block(25, 66, 5+i*71, 30*j + 50, 50 - 10*j, colorLevels[j]));
        }
    }
    return blocks;
}

void drawGameOver(sf::RenderWindow& window, int score, sf::Font font) {
    sf::Text gameOver;
    gameOver.setFont(font);
    gameOver.setString("Game Over");
    gameOver.setCharacterSize(50);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition(120, 200);
    window.draw(gameOver);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(150, 250);
    window.draw(scoreText);

    sf::Text resetText;
    resetText.setFont(font);
    resetText.setString("Press R to play again");
    resetText.setCharacterSize(30);
    resetText.setFillColor(sf::Color::Red);
    resetText.setPosition(115, 300);
    window.draw(resetText);
}