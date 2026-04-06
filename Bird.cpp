#include "Bird.h"
#include "Globals.h"

Bird::Bird() :
    gravity(800.f),
    flap_speed(300.f),
    velocity_y(0.f),
    anim_counter(0),
    texture_switch(1),
    should_fly(false),
    bird_sprite(textures[0])  // textures[0] is empty here, fixed below
{
    // Load textures first
    textures[0].loadFromFile("assets/birddown.png");
    textures[1].loadFromFile("assets/birdup.png");

    // Now assign the loaded texture to the sprite
    bird_sprite.setTexture(textures[0], true);
    bird_sprite.setScale({ SCALE_FACTOR, SCALE_FACTOR });
    resetBirdPosition();
}

void Bird::update(sf::Time dt)
{
    if (bird_sprite.getGlobalBounds().position.y < 548.f && should_fly)
    {
        if (anim_counter == 5)
        {
            bird_sprite.setTexture(textures[texture_switch], true);
            texture_switch = texture_switch ? 0 : 1;
            anim_counter = 0;
        }
        anim_counter++;

        velocity_y += gravity * dt.asSeconds();
        bird_sprite.move({ 0.f, velocity_y * dt.asSeconds() });

        if (bird_sprite.getGlobalBounds().position.y < 0.f)
            bird_sprite.setPosition({ 100.f, 0.f });
    }
}

void Bird::flapBird()
{
    velocity_y = -flap_speed;
}

float Bird::getRightBound()
{
    auto b = bird_sprite.getGlobalBounds();
    return b.position.x + b.size.x;
}

void Bird::resetBirdPosition()
{
    bird_sprite.setPosition({ 100.f, 50.f });
    velocity_y = 0.f;
}

void Bird::setShouldFly(bool val)
{
    should_fly = val;
}