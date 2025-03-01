#ifndef TANK_H
#define TANK_H

/// Summary: This class is used to instantiate the player tank objects ///
class Tank
{
    private: 
        int playerNum;
        float spawnX;
        float spawnY;
        int health;
        bool isDead;
    public:
        // Constructor
        Tank(int playerNum, float spawnX, float spawnY, int health);
        ~Tank();
        
        // Getters
        int GetPlayerNum() const; 
        float GetX() const; 
        float GetY() const; 
        int GetHealth() const; 
        bool IsDead() const; 
        
        // Other Methods
        void TakeDamage(int damageTaken);

};

#endif // TANK_H