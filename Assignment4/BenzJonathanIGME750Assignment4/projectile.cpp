#include "projectile.h"

Projectile::Projectile(glm::vec2 pos, glm::vec3 color, float radius, float power, bool isUp = false)
	: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), color), radius(radius), power(power), isUp(isUp)
{ 
	// signage determines which direction the projectile is shot (player shoots up, aliens shoot down)
	power *= isUp ? -1 : 1;
	Velocity = glm::vec2(0.0f, power);
}

Projectile::~Projectile() {}