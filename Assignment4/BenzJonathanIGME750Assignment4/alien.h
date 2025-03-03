#ifndef ALIEN_H
#define ALIEN_H

#include "game_object.h"

class Alien : public GameObject
{

public:
    static glm::vec2 Velocity;
    static float yPos;

    // constructors
    Alien(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f));
    virtual ~Alien();

    // draw sprite
    void Draw(Sprite& renderer) override;
    void Move(float dt) override;
    void Die() override;
};
#endif