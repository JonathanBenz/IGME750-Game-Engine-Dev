#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "game_object.h"

class Projectile : public GameObject
{
public:
	float radius;
	float power;
	float theta;
	Projectile(glm::vec2 pos, float radius, float power, float theta);
	glm::vec2 Move(float dt);
};

#endif