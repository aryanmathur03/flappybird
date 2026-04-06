#pragma once
#include <SFML/Graphics.hpp>

class Bird
{
private:
    sf::Texture textures[2];
    const float gravity;
    const float flap_speed;
    float velocity_y;
    int anim_counter, texture_switch;
    bool should_fly;

public:
    sf::Sprite bird_sprite;

    Bird();
    void setShouldFly(bool);
    void flapBird();
    void resetBirdPosition();
    void update(sf::Time dt);
    float getRightBound();
};