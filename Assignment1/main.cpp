/* AUTHOR: Jonathan Benz
 * DATE: 01/20/2025
 * No known issues. 
 * 
 * My program handles additional players (I capped it at 4 players but the value can be increased), 
 * additional physics such as wind resistance, 
 * as well as player health (in addition to the standard base requirements). 
 * 
 * Also, the Concrete Donkey is the greatest Worms Weapon of all time, not the Banana Bomb!*/

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <sstream>

// ----------------------------------------------------- GLOBAL VARIABLES --------------------------------------------------------
// Global const variable for the world size: 100x100 grid. Adjust if you want a smaller or bigger world
const int WORLDSPACE {100};
// Global const variable for the acceleration due to gravity
const float GRAVITY = -9.8;
// Global const variable for the hit tolerance
const float TOLERANCE = 2.5;
// Global variable for wind resistance that changes each turn
float windResistance = 0;
// -------------------------------------------------------------------------------------------------------------------------------

// -------------------------------------------------------- CLASSES --------------------------------------------------------------
/// Summary: This class is used to instantiate the player tank objects ///
class Tank {
    private:
        int playerNum;
        float spawnX;
        float spawnY;
        int health;
        bool isDead;
        
    public:
        // Constructor
        Tank(int playerNum, float spawnX, float spawnY, int health)
            : playerNum(playerNum), spawnX(spawnX), spawnY(spawnY), health(health), isDead(false) {}
        
        // Getters
        int GetPlayerNum() const { return playerNum; }
        float GetX() const { return spawnX; }
        float GetY() const { return spawnY; }
        int GetHealth() const { return health; }
        bool IsDead() const { return isDead; }
        
        // Other Methods
        void TakeDamage(int damageTaken) 
        { 
            health -= damageTaken;
            if(health <= 0) { health = 0; isDead = true; }
        }
};

class Projectile {
    private:
        std::vector<Tank*> playerTanks;
        int damage;
        float xPos;
        float yPos;
        float xVel;
        float yVel;
        float theta;
        float xAccel;
        float yAccel;
        
        // Return whether projectile is within bounds of the world space
        bool WithinBounds() const 
        {
            if(xPos < 0 || xPos > WORLDSPACE || yPos < 0) return false;
            else return true;
        }
        
        // Check to see if the projectile has collided with a player tank
        bool CheckForCollisions(Tank* tank) const
        {
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
        
    public:
        // Constructor
        Projectile(const std::vector<Tank*>& playerTanks, int damage, float xStartPos, float yStartPos, float angle, float power) 
            : playerTanks(playerTanks), damage(damage), xPos(xStartPos), yPos(yStartPos), theta(angle)
        {
            // Initial conditions, assuming mass = 1
            xAccel = power * std::cos(theta);
            yAccel = power * std::sin(theta);
            xVel = xAccel;
            yVel = yAccel;
        }
        
        // Other Methods
        bool FireProjectile() 
        {
            while(WithinBounds()) 
            {
                // Update new position
                xPos += xVel;
                yPos += yVel;
                std::cout << "Projectile is currently at position: (" << xPos << ", " << yPos << ")" << std::endl;
                
                // Update the forces
                xAccel = windResistance;
                yAccel = GRAVITY;
                
                // Update the velocities
                xVel += xAccel;
                yVel += yAccel;
                
                // Check for collisions
                for(size_t i {}; i < playerTanks.size(); ++i) 
                {
                    if(CheckForCollisions(playerTanks[i])) 
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
            // No players got hit. Womp womp. 
            std::cout << "\nProjectile did not hit anything. You MISSED! Better luck next time..." << std::endl << std::endl;
            return false;
        }
};
// -------------------------------------------------------------------------------------------------------------------------------

// -------------------------------------------------------- METHODS --------------------------------------------------------------
/// Summary: This method prompts the user for the total amount of players and returns that number ///
int GetPlayers() {
    int numPlayers {};
    do {
        std::string playerInput;
        std::cout << "\nHow many players are playing? Enter an integer value (2-4): ";
        std::cin >> playerInput;
        std::istringstream inputStream {playerInput};
        // If an integer was entered
        if(inputStream >> numPlayers) 
        {
            // If the number is outside the bounds of the amount of players we want... Reject it
            if(numPlayers < 2 || numPlayers > 4) std::cout << "WRONG INPUT, try again!" << std::endl;
            else std::cout << "Num Players: " << numPlayers << std::endl << std::endl;
        }
        // If bogus input is detected... Reject it
        else 
        {
            std::cout << "WRONG INPUT, try again!" << std::endl;
        }
    } while(!(numPlayers >= 2 && numPlayers <= 4));
    return numPlayers;
}

/// Summary: Spawn a random Tank in the world for each player with a starting health of 100 ///
/// If DEBUG is set to true, then have player 1 shoot at an angle of 45 degrees and a power of 100. 
/// This will test a guaranteed projectile collision against player 2. ///
void SpawnPlayers(std::vector<Tank*>& playerTanks, const int& numPlayers, bool DEBUG = false) {
    if(DEBUG) 
    {
        playerTanks.push_back(new Tank(1, 0, 0, 100));
        playerTanks.push_back(new Tank(2, 52, 85, 10));
        return;
    }
    for(int i{}; i < numPlayers; ++i) 
    {
        float randX = std::rand() % WORLDSPACE;
        float randY = std::rand() % WORLDSPACE;
        int health = 100;
        playerTanks.push_back(new Tank(i+1, randX, randY, health));
    }
}

/// Summary: Print all player spawn positions and health values. ///
void PrintPlayerSpawnPositions(std::vector<Tank*>& playerTanks) {
    for(size_t i{}; i < playerTanks.size(); ++i) 
    {
        std::cout << "Player " << playerTanks[i]->GetPlayerNum() << " spawns at position: ("
            << playerTanks[i]->GetX() << ", " << playerTanks[i]->GetY() 
            << ") with a starting health of: " << playerTanks[i]->GetHealth() << std::endl;
    }
    std::cout << std::endl;
}

/// Summary: Get the player's desired shooting angle ///
void GetAngle(int& angle) {
    do {
        std::string playerInput;
        std::cout << "Enter the angle you would like to fire at (1-360 degrees): ";
        std::cin >> playerInput;
        std::istringstream inputStream {playerInput};
        if(inputStream >> angle) 
        {
            if(!(angle > 0 && angle <= 360)) std::cout << "Invalid Input... OUT OF THE VALID RANGE. Try again." << std::endl;
            else std::cout << "Chosen Angle: " << angle << std::endl << std::endl;
        }
        else 
        {
            std::cout << "Invalid Input... Try again." << std::endl;
        }
    } while(!(angle > 0 && angle <= 360));
}

/// Summary: Get the player's desired shooting power ///
void GetPower(int& power) {
    do {
        std::string playerInput;
        std::cout << "Enter the power of your shot (1-100): ";
        std::cin >> playerInput;
        std::istringstream inputStream {playerInput};
        if(inputStream >> power) 
        {
            if(!(power > 0 && power <= 100)) std::cout << "Invalid Input... OUT OF THE VALID RANGE. Try again." << std::endl;
            else std::cout << "Chosen Power: " << power << std::endl << std::endl;
        }
        else 
        {
            std::cout << "Invalid Input... Try again." << std::endl;
        }
    } while(!(power > 0 && power <= 100));
}

/// Summary: Loops through all playerTanks to check if any have died. If so, it is removed ///
void CheckForDeadTanks(std::vector<Tank*>& playerTanks) {
    for(size_t i {}; i < playerTanks.size(); ++i) 
    {
        if(playerTanks[i]->IsDead())
        {
            std::cout << "Player " << playerTanks[i]->GetPlayerNum() << "'s Tank has exploded! They're out of the game!!" << std::endl << std::endl;
            playerTanks.erase(playerTanks.begin() + i);
            return;
        }
    }
}

/// Summary: Checks how many tanks are remaining. If there is only 1 tank remaining, then that player wins. End the game. ///
bool CheckForWinner(std::vector<Tank*>& playerTanks) {
    if(playerTanks.size() <= 1) 
    {
        std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "VICTORY! Player " << playerTanks[0]->GetPlayerNum() 
            << " has defeated all of the other tanks and has WON!!" << std::endl;
        std::cout << "Thanks for playing! Goodbye!" << std::endl;
        return true;
    }
    return false;
}
// -------------------------------------------------------------------------------------------------------------------------------

int main() {
    // Print Title
    std::cout << "============ Jonathan Benz's IGME 750 Assignment 1: Projectile Physics and Scorched Earth ============" << std::endl;
    
    // Prompt user for how many players there will be
    int numPlayers = GetPlayers();
    
    // Seed a number generator with current time (to be used when we randomly spawn the players). 
    std::srand(std::time(nullptr));

    // Spawning players randomly within the world space (Check the global const variable WORLDSPACE for the world size)
    std::vector<Tank*> playerTanks;
    // Look at the summary of the SpawnPlayers() method for more information about the debug parameter
    // For normal play, this value should be set to false
    bool debug = false;
    SpawnPlayers(playerTanks, numPlayers, debug);
    
    // Print player spawn locations
    PrintPlayerSpawnPositions(playerTanks);
    
    // MAIN GAME LOOP
    int currentTurn {};
    while(true) {
        // Check for any tanks that might have died since the last tick. Remove them from the playerTanks vector if so. 
        CheckForDeadTanks(playerTanks);
        
        // Check to see if there is 1 tank remaining. If so, they won the game! Break out of the game loop and end the program. 
        if(CheckForWinner(playerTanks)) break;
        
        // Loop back to player 1's turn after the last player's turn
        if(currentTurn >= playerTanks.size()) currentTurn = 0;
        
        std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "It is Player " << playerTanks[currentTurn]->GetPlayerNum() << "'s turn!" << std::endl;
        
        // Get a new random value for wind resistance between the values of -10 and +10
        // If debug is on, do not have any wind resistance. Just for testing purposes. 
        windResistance = debug ? 0 : (std::rand() % 21) - 10;
        std::cout << "CURRENT WIND RESISTANCE: " << windResistance << std::endl << std::endl;
        
        // Prompt for input, get angle and power
        int angle {-1};
        int power {-1};
        GetAngle(angle);
        GetPower(power);
        
        // Fire the projectile
        int projectileDamage = 20;
        float startXPos = playerTanks[currentTurn]->GetX();
        float startYPos = playerTanks[currentTurn]->GetY();
        std::cout << "Player " << playerTanks[currentTurn]->GetPlayerNum() << " fires their cannon!" << std::endl << std::endl;
        Projectile* projectile = new Projectile(playerTanks, projectileDamage, startXPos, startYPos, angle, power);
        projectile->FireProjectile();
        delete projectile;
        
        // Turn is over. Iterate to next available player
        currentTurn++;
    }
    
    return 0;
}