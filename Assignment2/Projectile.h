#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <iostream>
#include <vector>
#include <cmath>
#include "Tank.h"

/// Summary: This class is used to fire projectiles from Tank objects and simulate the projectile motion ///
class Projectile
{
    private:
        std::vector<Tank*> playerTanks;
        Tank* currentTank;
        int damage;
        float xPos;
        float yPos;
        float xVel;
        float yVel;
        float theta;
        float xAccel;
        float yAccel;
        
        // Return whether projectile ever enters hyperbolic orbit
        bool HasReachedEscapeVelocity(float currentVel) const;
        
        // Check to see if the projectile has collided with the surface
        bool CheckForSurfaceCollision(float currentDistance) const;

        // Check to see if the projectile has collided with a player tank
        bool CheckForTankCollisions(Tank* tank) const;
        
    public:
        // Constructor
        Projectile(const std::vector<Tank*>& playerTanks, Tank* currentTank, int damage, float xStartPos, float yStartPos, float angle, float power);
        ~Projectile();
        
        // Other Methods
        bool FireProjectile();

};

#endif // PROJECTILE_H