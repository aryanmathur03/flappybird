#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <memory>
#include "Bird.h"
#include "Pipe.h"

class Game
{
public:
    Game(sf::RenderWindow&);
    void startGameLoop();
    sf::RenderWindow& win;

private:
    sf::Texture bg_texture, ground_texture;
    sf::Sprite bg_sprite, ground_sprite1, ground_sprite2;

    Bird bird;

    bool is_enter_pressed, run_game, start_monitoring;
    const int move_speed = 270;

    void draw();
    void moveGround(sf::Time dt);
    void doProcessing(sf::Time dt);
    void checkCollisions();
    void restartGame();
    void checkScore();
    std::string toString(int);

    int pipe_counter, pipe_spawn_time, score;
    std::vector<Pipe> pipes;

    std::random_device rd;
    std::uniform_int_distribution<int> dist{ 250, 550 };

    sf::Font font;
    std::unique_ptr<sf::Text> restart_text;
    std::unique_ptr<sf::Text> score_text;
};