#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "Tank.h"

class Utils
{
    private:
        Utils() {}
        
    public:
        /// Summary: This method prompts the user for the total amount of players and returns that number
        int static GetPlayers();
        
        /// Summary: Get the size of asteroid field size 
        int static GetAsteroidDiameter();
        
        /// Summary: Set the size of asteroid field size
        void static SetAsteroidDiameter(int diameter);
        
        /// Summary: Spawn a random Tank in the world for each player with a starting health of 100
        void static SpawnPlayers(std::vector<Tank*>& playerTanks, const int& numPlayers);
        
        /// Summary: Print all player spawn positions and health values. ///
        void static PrintPlayerSpawnPositions(std::vector<Tank*>& playerTanks);
        
        /// Summary: Get the player's desired shooting angle ///
        void static GetAngle(int& angle);
        
        /// Summary: Get the player's desired shooting power ///
        void static GetPower(int& power);
        
        /// Summary: Loops through all playerTanks to check if any have died. If so, it is removed
        void static CheckForDeadTanks(std::vector<Tank*>& playerTanks);
        
        /// Summary: Checks how many tanks are remaining. If there is only 1 tank remaining, then that player wins. End the game
        bool static CheckForWinner(std::vector<Tank*>& playerTanks);
};

#endif // UTILS_H
