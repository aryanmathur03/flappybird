#pragma once
#include <SFML/Graphics.hpp>

class Pipe
{
public:
    Pipe(int y_pos);
    sf::Sprite sprite_up, sprite_down;
    static void loadTextures();
    void update(sf::Time dt);
    float getRightBound();

private:
    static sf::Texture texture_down, texture_up;
    static int pipe_distance, move_speed;
};