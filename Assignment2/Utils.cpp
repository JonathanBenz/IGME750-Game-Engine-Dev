#include "Utils.h"

int diameter {};

/// Summary: This method prompts the user for the total amount of players and returns that number
int Utils::GetPlayers() 
{
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

/// Summary: Get the size of asteroid field size
int Utils::GetAsteroidDiameter() 
{
    return diameter;
}

/// Summary: Set the size of asteroid field size
void Utils::SetAsteroidDiameter(int size) 
{
    diameter = size;
}

/// Summary: Spawn a random Tank in the world for each player with a starting health of 100
void Utils::SpawnPlayers(std::vector<Tank*>& playerTanks, const int& numPlayers) 
{
    for(int i{}; i < numPlayers; ++i) 
    {
        float radius = diameter / 2.0;
        float randomDegree = std::rand() % 360;
        float randomRadian = randomDegree * (3.14159 / 180);
        
        float randX = std::cos(randomRadian) * radius;
        float randY = std::sin(randomRadian) * radius;
        
        int health = 100;
        playerTanks.push_back(new Tank(i+1, randX, randY, health));
    }
}

/// Summary: Print all player spawn positions and health values
void Utils::PrintPlayerSpawnPositions(std::vector<Tank*>& playerTanks) 
{
    for(size_t i{}; i < playerTanks.size(); ++i) 
    {
        std::cout << "Player " << playerTanks[i]->GetPlayerNum() << " spawns at position: ("
            << playerTanks[i]->GetX() << ", " << playerTanks[i]->GetY() 
            << ") with a starting health of: " << playerTanks[i]->GetHealth() << std::endl;
    }
    std::cout << std::endl;
}

/// Summary: Get the player's desired shooting angle
void Utils::GetAngle(int& angle) 
{
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

/// Summary: Get the player's desired shooting power
void Utils::GetPower(int& power) 
{
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

/// Summary: Loops through all playerTanks to check if any have died. If so, it is removed
void Utils::CheckForDeadTanks(std::vector<Tank*>& playerTanks) 
{
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

/// Summary: Checks how many tanks are remaining. If there is only 1 tank remaining, then that player wins. End the game
bool Utils::CheckForWinner(std::vector<Tank*>& playerTanks) 
{
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