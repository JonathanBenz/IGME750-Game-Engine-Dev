#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "game_object.h"

class Projectile : public GameObject
{
public:
	float radius;
	float power;
	bool isUp;
	Projectile(glm::vec2 pos, glm::vec3 color, float radius, float power, bool isUp);
	~Projectile();
};

#endif