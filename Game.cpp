#include "Game.h"
#include "Globals.h"
#include <sstream>

Game::Game(sf::RenderWindow& window) :
    win(window),
    is_enter_pressed(false),
    run_game(true),
    start_monitoring(false),
    pipe_counter(71),
    pipe_spawn_time(70),
    score(0),
    bg_sprite(bg_texture),
    ground_sprite1(ground_texture),
    ground_sprite2(ground_texture)
{
    win.setFramerateLimit(60);

    // Load bg texture then re-assign to sprite with resetRect=true
    bg_texture.loadFromFile("assets/bg.png");
    bg_sprite.setTexture(bg_texture, true);
    bg_sprite.setScale({ SCALE_FACTOR, SCALE_FACTOR });
    bg_sprite.setPosition({ 0.f, -250.f });

    // Load ground texture then re-assign
    ground_texture.loadFromFile("assets/ground.png");
    ground_sprite1.setTexture(ground_texture, true);
    ground_sprite2.setTexture(ground_texture, true);
    ground_sprite1.setScale({ SCALE_FACTOR, SCALE_FACTOR });
    ground_sprite2.setScale({ SCALE_FACTOR, SCALE_FACTOR });
    ground_sprite1.setPosition({ 0.f, 578.f });
    ground_sprite2.setPosition({ ground_sprite1.getGlobalBounds().size.x, 578.f });

    // Load font and construct Text objects after
    font.openFromFile("assets/arial.ttf");

    restart_text = std::make_unique<sf::Text>(font, "Restart Game!!", 48);
    restart_text->setFillColor(sf::Color::Black);
    restart_text->setPosition({ 150.f, 650.f });

    score_text = std::make_unique<sf::Text>(font, "Score: 0", 24);
    score_text->setFillColor(sf::Color::Black);
    score_text->setPosition({ 15.f, 15.f });

    Pipe::loadTextures();
}

void Game::doProcessing(sf::Time dt)
{
    if (is_enter_pressed)
    {
        moveGround(dt);

        if (pipe_counter > pipe_spawn_time)
        {
            pipes.push_back(Pipe(dist(rd)));
            pipe_counter = 0;
        }
        pipe_counter++;

        for (int i = 0; i < (int)pipes.size(); i++)
        {
            pipes[i].update(dt);
            if (pipes[i].getRightBound() < 0)
            {
                pipes.erase(pipes.begin() + i);
                i--;
            }
        }

        checkCollisions();
        checkScore();
    }

    bird.update(dt);
}

void Game::startGameLoop()
{
    sf::Clock clock;

    while (win.isOpen())
    {
        sf::Time dt = clock.restart();

        while (const std::optional<sf::Event> event = win.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                win.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (run_game)
                {
                    if (keyPressed->code == sf::Keyboard::Key::Enter && !is_enter_pressed)
                    {
                        is_enter_pressed = true;
                        bird.setShouldFly(true);
                    }
                    if (keyPressed->code == sf::Keyboard::Key::Space && is_enter_pressed)
                        bird.flapBird();
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left && !run_game)
                {
                    sf::Vector2f mousePos(
                        static_cast<float>(mousePressed->position.x),
                        static_cast<float>(mousePressed->position.y)
                    );
                    if (restart_text->getGlobalBounds().contains(mousePos))
                        restartGame();
                }
            }
        }

        doProcessing(dt);
        draw();
        win.display();
    }
}

void Game::checkCollisions()
{
    for (auto& pipe : pipes)
    {
        if (pipe.sprite_down.getGlobalBounds().findIntersection(bird.bird_sprite.getGlobalBounds()) ||
            pipe.sprite_up.getGlobalBounds().findIntersection(bird.bird_sprite.getGlobalBounds()))
        {
            is_enter_pressed = false;
            run_game = false;
            return;
        }
    }
    if (bird.bird_sprite.getGlobalBounds().position.y >= 540.f)
    {
        is_enter_pressed = false;
        run_game = false;
    }
}

void Game::checkScore()
{
    if (!pipes.empty())
    {
        float birdLeft  = bird.bird_sprite.getGlobalBounds().position.x;
        float pipeLeft  = pipes[0].sprite_down.getGlobalBounds().position.x;
        float pipeRight = pipes[0].getRightBound();

        if (!start_monitoring)
        {
            if (birdLeft > pipeLeft && bird.getRightBound() < pipeRight)
                start_monitoring = true;
        }
        else
        {
            if (birdLeft > pipeRight)
            {
                score++;
                score_text->setString("Score: " + toString(score));
                start_monitoring = false;
            }
        }
    }
}

void Game::draw()
{
    win.clear();
    win.draw(bg_sprite);

    for (Pipe& pipe : pipes)
    {
        win.draw(pipe.sprite_down);
        win.draw(pipe.sprite_up);
    }

    win.draw(ground_sprite1);
    win.draw(ground_sprite2);
    win.draw(bird.bird_sprite);
    win.draw(*score_text);

    if (!run_game)
        win.draw(*restart_text);
}

void Game::moveGround(sf::Time dt)
{
    ground_sprite1.move({ -move_speed * dt.asSeconds(), 0.f });
    ground_sprite2.move({ -move_speed * dt.asSeconds(), 0.f });

    auto b1 = ground_sprite1.getGlobalBounds();
    auto b2 = ground_sprite2.getGlobalBounds();

    if (b1.position.x + b1.size.x < 0)
        ground_sprite1.setPosition({ b2.position.x + b2.size.x, 578.f });
    if (b2.position.x + b2.size.x < 0)
        ground_sprite2.setPosition({ b1.position.x + b1.size.x, 578.f });
}

void Game::restartGame()
{
    bird.resetBirdPosition();
    bird.setShouldFly(false);
    run_game = true;
    is_enter_pressed = false;
    start_monitoring = false;
    pipe_counter = 71;
    pipes.clear();
    score = 0;
    score_text->setString("Score: 0");
}

std::string Game::toString(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}