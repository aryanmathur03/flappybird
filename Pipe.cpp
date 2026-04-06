#include "Pipe.h"
#include "Globals.h"

Pipe::Pipe(int y_pos) :
    sprite_up(texture_up),
    sprite_down(texture_down)
{
    // Reset texture rect to ensure full texture is shown
    sprite_down.setTexture(texture_down, true);
    sprite_up.setTexture(texture_up, true);

    sprite_down.setScale({ SCALE_FACTOR, SCALE_FACTOR });
    sprite_up.setScale({ SCALE_FACTOR, SCALE_FACTOR });

    sprite_up.setPosition({ (float)WIN_WIDTH, (float)y_pos });
    float upHeight = sprite_up.getGlobalBounds().size.y;
    sprite_down.setPosition({ (float)WIN_WIDTH, (float)y_pos - pipe_distance - upHeight });
}

void Pipe::update(sf::Time dt)
{
    sprite_up.move({ -move_speed * dt.asSeconds(), 0.f });
    sprite_down.move({ -move_speed * dt.asSeconds(), 0.f });
}

void Pipe::loadTextures()
{
    texture_down.loadFromFile("assets/pipedown.png");
    texture_up.loadFromFile("assets/pipe.png");
}

float Pipe::getRightBound()
{
    auto b = sprite_down.getGlobalBounds();
    return b.position.x + b.size.x;
}

sf::Texture Pipe::texture_down, Pipe::texture_up;
int Pipe::pipe_distance = 170, Pipe::move_speed = 400;