#include "Projectile.h"

// Constants
// -----------------------------------------------------------
const float TOLERANCE = 2.5;
const float GRAVITATIONAL_CONSTANT = 6.67e-11;
const float PI = 3.14159;
// -----------------------------------------------------------

// Asteroid-Related Properties (needed to calculate projectile motion)
// -----------------------------------------------------------
int diameterOfAsteroid = 10;
// Radius is half of the diameter
float radiusOfAsteroid = diameterOfAsteroid / 2.0;
// Approximate the density of the asteroid to be around 2
float density = 2.0;
// Calculate volume of the asteroid 
float volume = (4.0/3) * PI * (radiusOfAsteroid*radiusOfAsteroid*radiusOfAsteroid);
// Calculate the mass of the asteroid
float massOfAsteroid = density * volume;
// Calculate the escape velocity required for the projectile to enter hyperbolic orbit
float escapeVelocity = std::sqrt((2.0*GRAVITATIONAL_CONSTANT*massOfAsteroid) / diameterOfAsteroid);
// -----------------------------------------------------------

// Drag will be 10% of the field size 
float drag = diameterOfAsteroid * 0.10;

// Constructor
Projectile::Projectile(const std::vector<Tank*>& playerTanks, Tank* currentTank, int damage, float xStartPos, float yStartPos, float angle, float power) 
        : playerTanks(playerTanks), currentTank(currentTank), damage(damage), xPos(xStartPos), yPos(yStartPos), theta(angle)
    {
        /* Initial conditions, assuming mass of the projectile = 1. 
         * Power is scaled down by a fudge factor of 6 magnitudes.
         * Through extensive testing and fine tuning, this was the best option 
         * to disallow projectiles from instantly reaching an escape velocity... */
        float fudgeFactor = 1e6;
        power /= fudgeFactor;
        xAccel = power * std::cos(theta);
        yAccel = power * std::sin(theta);
        xVel = xAccel;
        yVel = yAccel;
    }

Projectile::~Projectile() {}

// Return whether projectile ever enters hyperbolic orbit
bool Projectile::HasReachedEscapeVelocity(float currentVel) const 
    {
        if(currentVel > escapeVelocity) 
            {
                std::cout << "The projectile has reached escape velocity and entered hyperbolic orbit!" << std::endl; 
                return true; 
            }
        return false;
    }

// Check to see if the projectile has collided with the surface
bool Projectile::CheckForSurfaceCollision(float currentDistance) const 
{
    if(currentDistance < radiusOfAsteroid) 
    {
        std::cout << "The projectile has crashed into the asteroid's surface!" << std::endl; 
        return true;
    }
    return false;
}

// Check to see if the projectile has collided with a player tank
bool Projectile::CheckForTankCollisions(Tank* tank) const
    {
        // Edge case: Prevent shooting yourself
        if(tank == currentTank) return false;
        // Edge case: If tank is already dead, no need to check for collision
        if(tank->IsDead()) return false;
            
        float tankXPos = tank->GetX();
        float tankYPos = tank->GetY();
        
        // If this current projectile is within the tolerance ranges of the tank, it's a hit!
        if((xPos >= tankXPos - TOLERANCE && xPos <= tankXPos + TOLERANCE)
            && (yPos >= tankYPos - TOLERANCE && yPos <= tankYPos + TOLERANCE)) 
        {
            return true;
        }
        // Else it's a miss!
        return false;
    }

// Simulate projectile motion physics
bool Projectile::FireProjectile() 
    {
        std::cout << "Escape Velocity: " << escapeVelocity << std::endl;
        while(true) 
        {
            // Update new position
            xPos += xVel;
            yPos += yVel;
            std::cout << "Projectile is currently at position: (" << xPos << ", " << yPos << ")" << std::endl;
                
            // Calculate distance and unit vector
            float currentDistance = std::sqrt(xPos*xPos + yPos*yPos);
            float xUnitVector = xPos / currentDistance;
            float yUnitVector = yPos / currentDistance;
            
            // Deal with potential divide by zero exception
            if(xUnitVector == 0 || yUnitVector == 0) continue;
            
            // Update the total forces (acceleration due to gravity between two point masses formula) combined with the drag
            float xAccel = -((GRAVITATIONAL_CONSTANT * massOfAsteroid) / (currentDistance * currentDistance)) * drag * xUnitVector;
            float yAccel = -((GRAVITATIONAL_CONSTANT * massOfAsteroid) / (currentDistance * currentDistance)) * drag * yUnitVector;
                
            // Update the velocities
            xVel += xAccel;
            yVel += yAccel;
            
            // Check for surface collision
            if(CheckForSurfaceCollision(currentDistance)) return false;
            
            // Check for Escape Velocity
            float currentVelocity = std::sqrt((xVel*xVel) + (yVel*yVel));
            if(HasReachedEscapeVelocity(currentVelocity)) return false;
            
            // Check for tank collisions
            for(size_t i {}; i < playerTanks.size(); ++i) 
            {
                if(CheckForTankCollisions(playerTanks[i])) 
                {
                    // Make the collided player take damage. Display which player got hit.
                    playerTanks[i]->TakeDamage(damage);
                    std::cout << "\nBOOM! Nice shot!! You successfully hit Player " << playerTanks[i]->GetPlayerNum() << "!!!" << std::endl;
                    std::cout << "Player " << playerTanks[i]->GetPlayerNum() << "'s Tank has taken " << damage 
                        << " damage and now has " << playerTanks[i]->GetHealth() << " Health remaining!" << std::endl << std::endl;
                    return true;
                }
            }
        }
        return false;
    }