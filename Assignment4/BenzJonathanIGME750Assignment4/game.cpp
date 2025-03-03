#include "game.h"
#include "sprite.h"
#include "game_object.h"
#include "projectile.h"
#include "alien.h"
#include "threadpool.h"

#include <SFML/Audio.hpp>
#include <vector>
#include <thread>
#include <mutex>

// Const Colors 
const glm::vec3 RED = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 BLUE = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 YELLOW = glm::vec3(1.0f, 1.0f, 0.0f);
const glm::vec3 MAGENTA = glm::vec3(1.0f, 0.0f, 1.0f);
const glm::vec3 CYAN = glm::vec3(0.0f, 1.0f, 1.0f);
const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);

// Const Sizes
const glm::vec2 TANK_SIZE(50.0f, 50.0f);
const glm::vec2 ALIEN_SIZE(40.0f, 40.0f);
const glm::vec2 ALIEN_SPAWN(130.0f, Alien::yPos);
const float PLAYER_SPEED = 20000.0f;

// Game Objects 
Sprite* Renderer;
GameObject* Tank;
std::vector<Alien*> Aliens;

bool collisionDetected = false;

// Projectile variables
std::vector<Projectile*> Projectiles;
const float PROJECTILE_RADIUS = 5.0f;
float power = 1000.0f;
float shootingCooldown = 1.0f;
float playerTimer = 0.0f;
float alienTimer = 0.0f;

// Sound variables
sf::Sound sound;
sf::SoundBuffer sound_buffer;

std::mutex collisionMutex;
std::mutex projectileMutex;

ThreadPool threadPool(4);

Game::Game(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT) : SCREEN_WIDTH(SCREEN_WIDTH), SCREEN_HEIGHT(SCREEN_HEIGHT), Keys() { }

Game::~Game()
{
    delete Renderer;
    delete Tank;
    for (Alien* a : Aliens)
    {
        delete a;
    }
    for (Projectile* p : Projectiles)
    {
        delete p;
    }
}

void Game::Init()
{
    std::srand(std::time(nullptr));
    Renderer = new Sprite();
    Tank = new GameObject(glm::vec3((SCREEN_WIDTH / 2) - (TANK_SIZE.x /2), SCREEN_HEIGHT - 75.0f, 0.0f), TANK_SIZE, BLUE);
    Aliens.reserve(5);
    Aliens.push_back(new Alien(glm::vec2(1 * ALIEN_SPAWN.x, ALIEN_SPAWN.y), ALIEN_SIZE, RED));
    Aliens.push_back(new Alien(glm::vec2(2 * ALIEN_SPAWN.x, ALIEN_SPAWN.y), ALIEN_SIZE, GREEN));
    Aliens.push_back(new Alien(glm::vec2(3 * ALIEN_SPAWN.x, ALIEN_SPAWN.y), ALIEN_SIZE, YELLOW));
    Aliens.push_back(new Alien(glm::vec2(4 * ALIEN_SPAWN.x, ALIEN_SPAWN.y), ALIEN_SIZE, MAGENTA));
    Aliens.push_back(new Alien(glm::vec2(5 * ALIEN_SPAWN.x, ALIEN_SPAWN.y), ALIEN_SIZE, CYAN));
}

void Game::Update(float dt)
{
    // Update timer which is used to check against Projectile Firing Cooldown
    playerTimer += dt;
    alienTimer += dt;

    // Enqueue tasks for moving game objects
    threadPool.enqueue([this, dt]() { Tank->Move(dt); });

    for (Alien* a : Aliens) {
        threadPool.enqueue([this, a, dt]() { a->Move(dt); });

        // Handle alien projectile spawning directly in the main thread
        if (a->enabled && rng(0.01)) {
            SpawnProjectile(false, a->Position, ALIEN_SIZE, a->Color);
        }
    }

    // Enqueue tasks for moving projectiles
    for (Projectile* p : Projectiles) {
        threadPool.enqueue([this, p, dt]() { p->Move(dt); });
    }

    // Enqueue tasks for collision checks
    for (Alien* a : Aliens) {
        threadPool.enqueue([this, a]() { CheckCollision(*a, *Tank); });
    }

    for (Projectile* p : Projectiles) {
        threadPool.enqueue([this, p]() { CheckCollision(*p, *Tank); });

        for (Alien* a : Aliens) {
            threadPool.enqueue([this, p, a]() { CheckCollision(*p, *a); });
        }
    }
}

void Game::ProcessInput(float dt)
{
    // Fire
	if (Keys[GLFW_KEY_SPACE] && Tank->enabled)
	{
        SpawnProjectile(true, Tank->Position, TANK_SIZE, BLUE);
	}
    // Move Left
	if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT])
	{
		Tank->Velocity = glm::vec2(-PLAYER_SPEED, 0.0f) * dt;
	}
    // Move Right
	else if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT])
	{
        Tank->Velocity = glm::vec2(PLAYER_SPEED, 0.0f) * dt;
	}
    // Not Moving
    else
    {
        Tank->Velocity = glm::vec2(0.0f);
    }
}

void Game::SpawnProjectile(bool isPlayer, glm::vec2 pos, glm::vec2 size, glm::vec3 color)
{
    if (isPlayer)
    {
        if (playerTimer < shootingCooldown) return;
        Projectiles.push_back(new Projectile(glm::vec2(pos.x + (size.x / 2) - PROJECTILE_RADIUS, pos.y - 1), color, PROJECTILE_RADIUS, power, true));
        playerTimer = 0.0f;
        PlaySound("tankFiringSound.wav");
    }
    else
    {
        if (alienTimer < shootingCooldown) return;
        Projectiles.push_back(new Projectile(glm::vec2(pos.x + (size.x / 2) - PROJECTILE_RADIUS, pos.y + size.y + 1), color, PROJECTILE_RADIUS, power, false));
        alienTimer = 0.0f;
        PlaySound("alienFiringSound.wav");
    }
}

void Game::Render()
{
    // Draw all Game Objects using multithreading... or not. Nothing will render if I use multithreading with OpenGL.
    //std::vector<std::thread> renderThreads;
    //renderThreads.push_back(std::thread(&GameObject::Draw, Tank, std::ref(*Renderer)));

    Tank->Draw(*Renderer);
    for (Alien* a : Aliens)
    {
        a->Draw(*Renderer);
        //renderThreads.push_back(std::thread(&Alien::Draw, a, std::ref(*Renderer)));
    }

    for (Projectile* p : Projectiles)
    {
        p->Draw(*Renderer);
        //renderThreads.push_back(std::thread(&Projectile::Draw, p, std::ref(*Renderer)));
    }

    /*for (std::thread& t : renderThreads)
    {
        t.join();
    }*/
}

void Game::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB
{
    // Lock the mutex to prevent race conditions during collision checking
    std::lock_guard<std::mutex> lock(collisionMutex);

    if (!one.enabled || !two.enabled) return;
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    if (collisionX && collisionY) { one.Die(); two.Die(); }
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

bool Game::rng(float percentage)
{
    float rand = std::rand() % 100;
    return rand < percentage ? true : false;
}