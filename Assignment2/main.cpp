/* AUTHOR: Jonathan Benz
 * DATE: 01/30/2025
 * No known issues. 
 */

#include <iostream>
#include <vector>
#include <ctime>

#include "Tank.h"
#include "Projectile.h"
#include "Utils.h"

int main() {
    // Print Title
    std::cout << "============ Jonathan Benz's IGME 750 Assignment 2: Scorched Planet ============" << std::endl;
    
    // Prompt user for how many players there will be
    int numPlayers = Utils::GetPlayers();
    
    // Seed a number generator with current time (to be used when we randomly spawn the players). 
    std::srand(std::time(nullptr));
    
    // Spawning players randomly on an asteroid of field size 10
    Utils::SetAsteroidDiameter(10);
    std::vector<Tank*> playerTanks;
    Utils::SpawnPlayers(playerTanks, numPlayers);
    
    // Print player spawn locations
    Utils::PrintPlayerSpawnPositions(playerTanks);
    
    // MAIN GAME LOOP
    size_t currentTurn {};
    while(true) {
        // Check for any tanks that might have died since the last tick. Remove them from the playerTanks vector if so. 
        Utils::CheckForDeadTanks(playerTanks);
        
        // Check to see if there is 1 tank remaining. If so, they won the game! Break out of the game loop and end the program. 
        if(Utils::CheckForWinner(playerTanks)) break;
        
        // Loop back to player 1's turn after the last player's turn
        if(currentTurn >= playerTanks.size()) currentTurn = 0;
        
        std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "It is Player " << playerTanks[currentTurn]->GetPlayerNum() << "'s turn!" << std::endl;
        
        // Prompt for input, get angle and power
        int angle {-1};
        int power {-1};
        Utils::GetAngle(angle);
        Utils::GetPower(power);
        
        // Fire the projectile
        Tank* currentTank = playerTanks[currentTurn];
        int projectileDamage = 50;
        float startXPos = playerTanks[currentTurn]->GetX();
        float startYPos = playerTanks[currentTurn]->GetY();
        std::cout << "Player " << currentTank->GetPlayerNum() << " fires their cannon!" << std::endl << std::endl;
        Projectile* projectile = new Projectile(playerTanks, currentTank, projectileDamage, startXPos, startYPos, angle, power);
        projectile->FireProjectile();
        delete projectile;
        
        // Turn is over. Iterate to next available player
        currentTurn++;
    }
    
    return 0;
}