#include "game.h"
#include "sprite.h"
#include "game_object.h"
#include "projectile.h"

#include <SFML/Audio.hpp>

Sprite* Renderer;

const glm::vec2 TANK_SIZE(100.0f, 100.0f);
const glm::vec2 BARREL_SIZE(50.0f, 25.0f);
const glm::vec2 TANK1_POS(600.0f, 370.0f);
const glm::vec2 TANK2_POS(100.0f, 370.0f);
const glm::vec2 BARREL1_POS(TANK1_POS.x - BARREL_SIZE.x, TANK1_POS.y + (TANK_SIZE.y / 2.0f) - (BARREL_SIZE.y / 2.0f));
const glm::vec2 BARREL2_POS(TANK2_POS.x + TANK_SIZE.x, TANK2_POS.y + (TANK_SIZE.y / 2.0f) - (BARREL_SIZE.y / 2.0f));
const float rotateClampPos = 60.0f;
const float rotateClampNeg = 0.0f;

GameObject* GroundPlane;
GameObject* Tank1;
GameObject* Tank1Barrel;
GameObject* Tank2;
GameObject* Tank2Barrel;
GameObject* PowerMeter;

bool player1Turn = true;
bool collisionDetected = false;

const float PROJECTILE_RADIUS = 15.0f;
Projectile* projectile;
float power;
const float MAX_POWER = 2000.0f;

sf::Sound sound;
sf::SoundBuffer sound_buffer;

Game::Game() : Keys() { }

Game::~Game()
{
    delete Renderer;
    delete GroundPlane;
    delete Tank1;
    delete Tank2;
    delete Tank1Barrel;
    delete Tank2Barrel;
    delete PowerMeter;
    delete projectile;
}

void Game::Init()
{
    Renderer = new Sprite();
    GroundPlane = new GameObject(glm::vec2(0.0f, 450.0f), glm::vec2(800.0f, 150.0f), glm::vec3(0.3f, 0.5f, 0.3f));
    Tank1 = new GameObject(TANK1_POS, TANK_SIZE, glm::vec3(0.8f, 0.0f, 0.0f));
    Tank2 = new GameObject(TANK2_POS, TANK_SIZE, glm::vec3(0.0f, 0.0f, 0.8f));
    Tank1Barrel = new GameObject(BARREL1_POS, BARREL_SIZE, glm::vec3(0.8f, 0.0f, 0.0f));
    Tank2Barrel = new GameObject(BARREL2_POS, BARREL_SIZE, glm::vec3(0.0f, 0.0f, 0.8f));
    PowerMeter = new GameObject(glm::vec2(300.0f, 100.0f), glm::vec2(0.0f, 25.0f), glm::vec3(1.0f, 1.0f, 0.0f));
}

void Game::Update(float dt)
{
    // Update projectile physics
    if (projectile) projectile->Move(dt);

    // Check for collisions
    if (projectile && !collisionDetected)
    {
        if (CheckCollision(*projectile, *Tank2))
        {
            // Play tank hit sound and have Tank 2 take damage
            PlaySound("tankHitSound.wav");
            collisionDetected = true;
        }
        else if (CheckCollision(*projectile, *Tank1))
        {
            // Play tank hit sound and have Tank 1 take damage
            PlaySound("tankHitSound.wav");
            collisionDetected = true;
        }
        else if (CheckCollision(*projectile, *GroundPlane))
        {
            // Play ground hit sound
            PlaySound("groundHitSound.wav");
            collisionDetected = true;
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (player1Turn)
    {
        if (Keys[GLFW_KEY_SPACE])
        {
            power += 500.0f * dt;
            if (power >= MAX_POWER) power = MAX_POWER;
            PowerMeter->Size = glm::vec2((power / MAX_POWER) * 200.0f, 25.0f);
        }
        else if (Keys[GLFW_KEY_UP])
        {
            Tank1Barrel->Rotation += 100.0f * dt;
            if (Tank1Barrel->Rotation >= rotateClampPos) Tank1Barrel->Rotation = rotateClampPos;
        }
        else if (Keys[GLFW_KEY_DOWN])
        {
            Tank1Barrel->Rotation -= 100.0f * dt;
            if (Tank1Barrel->Rotation <= rotateClampNeg) Tank1Barrel->Rotation = rotateClampNeg;
        }
    }
    else
    {
        if (Keys[GLFW_KEY_SPACE])
        {
            power += 500.0f * dt;
            if (power >= MAX_POWER) power = MAX_POWER;
            PowerMeter->Size = glm::vec2((power / MAX_POWER) * 200.0f, 25.0f);
        }
        else if (Keys[GLFW_KEY_UP])
        {
            Tank2Barrel->Rotation -= 100.0f * dt;
            if (Tank2Barrel->Rotation <= -rotateClampPos) Tank2Barrel->Rotation = -rotateClampPos;
        }
        else if (Keys[GLFW_KEY_DOWN])
        {
            Tank2Barrel->Rotation += 100.0f * dt;
            if (Tank2Barrel->Rotation >= -rotateClampNeg) Tank2Barrel->Rotation = -rotateClampNeg;
        }
    }
}

void Game::SpawnProjectile()
{
    if (player1Turn)
    {
        float theta = glm::radians(180.0f + Tank1Barrel->Rotation);
        projectile = new Projectile(glm::vec2(BARREL1_POS.x - PROJECTILE_RADIUS, BARREL1_POS.y), PROJECTILE_RADIUS, power, theta);
    }
    else
    {
        float theta = glm::radians(Tank2Barrel->Rotation);
        projectile = new Projectile(glm::vec2(BARREL2_POS.x + BARREL_SIZE.x - PROJECTILE_RADIUS, BARREL2_POS.y), PROJECTILE_RADIUS, power, theta);
    }
    PlaySound("tankFiringSound.wav");
    player1Turn = !player1Turn;
    power = 0.0f;
    PowerMeter->Size = glm::vec2(0.0f, 25.0f);
    collisionDetected = false;
}

void Game::Render()
{
    // Draw all Game Objects
    GroundPlane->Draw(*Renderer);
    Tank1->Draw(*Renderer);
    Tank2->Draw(*Renderer);
    Tank1Barrel->Draw(*Renderer);
    Tank2Barrel->Draw(*Renderer);
    PowerMeter->Draw(*Renderer);

    if (projectile) projectile->Draw(*Renderer);
}

bool Game::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void Game::PlaySound(std::string soundFile)
{
    if (!sound_buffer.loadFromFile(soundFile))
    {
        std::cerr << "ERROR: Sound File Failed Loading!" << std::endl;
    }
    sound.setPosition(sf::Vector3f());
    sound.setVolume(20.0f);
    sound.setBuffer(sound_buffer);
    sound.play();
}