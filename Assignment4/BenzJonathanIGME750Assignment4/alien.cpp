#include "alien.h"

// Static variables
glm::vec2 Alien::Velocity = glm::vec2(100.0f, 0.0f);
float Alien::yPos = 60.0f;

Alien::Alien(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
    : GameObject(pos, size, color) { }

Alien::~Alien() {}

void Alien::Draw(Sprite& renderer)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    if (!enabled) return;
    renderer.DrawSprite(this->Position, this->Size, this->Color, true);
}

void Alien::Move(float dt)
{
    std::lock_guard<std::mutex> lock(updateMutex);
    if (!enabled) return;
    Position = glm::vec2(Position.x, Alien::yPos) + Alien::Velocity * dt;

    // Move down and then revert direction if at Bounds of Screen... 
    // Ideally I would pass in the screen width and height as params to the game object but I will just use literals here to save time
    if (Position.x >= 800 - Size.x || Position.x <= 0)
    {
        Alien::yPos += 30.0f;
        Velocity *= -1;
        PlaySound("Boop.wav");
    }
}

void Alien::Die()
{
    // If an alien dies, every other alien's speed goes up (possible due to static Velocity variable)
    Alien::Velocity *= 1.5f;
    enabled = false;
}