#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game_object.h"

/// <summary>
/// The Game class is responsible for handling all render and gameplay code.
/// </summary>
class Game
{
public:
	bool Keys[1024];

	Game();
	~Game();

	void Init();
	void ProcessInput(float dt);
	void SpawnProjectile();
	void Update(float dt);
	void Render();
	bool CheckCollision(GameObject& one, GameObject& two);
	void PlaySound(std::string soundFile);
};

#endif