#include "Tank.h"

// Constructor
Tank::Tank(int playerNum, float spawnX, float spawnY, int health)
    : playerNum(playerNum), spawnX(spawnX), spawnY(spawnY), health(health), isDead(false) {}
Tank::~Tank() {}

// Getters
int Tank::GetPlayerNum() const { return playerNum; }
float Tank::GetX() const { return spawnX; }
float Tank::GetY() const { return spawnY; }
int Tank::GetHealth() const { return health; }
bool Tank::IsDead() const { return isDead; }

// Other Methods
void Tank::TakeDamage(int damageTaken) 
    { 
        health -= damageTaken;
        if(health <= 0) { health = 0; isDead = true; }
    }
