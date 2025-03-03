#include "game_object.h"
#include "projectile.h"
#include "alien.h"
#include <SFML/Audio.hpp>
#include <iostream>

// Sound buffers
sf::Sound s;
sf::SoundBuffer sb;

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
    : Position(pos), Size(size), Color(color), enabled(true) {
    Velocity = glm::vec2(0.0f);
}

GameObject::~GameObject() {}

void GameObject::Draw(Sprite& renderer)
{
    std::lock_guard<std::mutex> lock(renderMutex);
    if (!enabled) return;
    renderer.DrawSprite(this->Position, this->Size, this->Color);
}

void GameObject::Move(float dt)
{ 
    std::lock_guard<std::mutex> lock(updateMutex);
    if (!enabled) return;
    Position += Velocity * dt;

    // Clamp At Bounds of Screen... Ideally I would pass in the screen width and height as params to the game object but I will just use literals here to save time
    if (Position.x >= 800 - Size.x) Position.x = 800 - Size.x;
    if (Position.x <= 0) Position.x = 0;
}

void GameObject::Die()
{
    PlaySound("tankHitSound.wav");
    enabled = false;
}

void GameObject::PlaySound(std::string soundFile)
{
    if (!sb.loadFromFile(soundFile))
    {
        std::cerr << "ERROR: Sound File Failed Loading!" << std::endl;
    }
    s.setPosition(sf::Vector3f());
    s.setVolume(20.0f);
    s.setBuffer(sb);
    s.play();
}