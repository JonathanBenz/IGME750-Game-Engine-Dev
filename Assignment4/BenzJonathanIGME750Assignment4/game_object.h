#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <mutex>

#include "sprite.h"


// Container object for holding all state relevant for a single game object entity. 
// Projectile and Alien inherit from this base class.
class GameObject
{
public:
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    bool enabled;

    // mutex used to turn critical threads into atomic threads 
    // (to prevent race conditions, e.g., the render method trying to render when Update is still running). 
    std::mutex updateMutex;
    std::mutex renderMutex;

    // constructor(s)
    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f));
    virtual ~GameObject();

    virtual void Draw(Sprite& renderer);
    virtual void Move(float dt);
    virtual void Die();
    virtual void PlaySound(std::string soundFile);
};

#endif