#include "projectile.h"

Projectile::Projectile(glm::vec2 pos, float radius, float power, float theta)
	: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), glm::vec3(1.0f, 0.5f, 0.2f)), radius(radius), power(power) 
{ 
	Velocity = glm::vec2(power * glm::cos(theta), power * glm::sin(theta));
}

glm::vec2 Projectile::Move(float dt)
{
	Velocity -= glm::vec2(0.0f, -9.8f);
	Position += Velocity * dt;
	return Position;
}