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
	const unsigned int SCREEN_WIDTH;
	const unsigned int SCREEN_HEIGHT;
	bool Keys[1024];

	Game(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT);
	~Game();

	void Init();
	void ProcessInput(float dt);
	void SpawnProjectile(bool isPlayer, glm::vec2 pos, glm::vec2 size, glm::vec3 color);
	void Update(float dt);
	void Render();
	void CheckCollision(GameObject& one, GameObject& two);
	void PlaySound(std::string soundFile);
	bool rng(float percentage);
};

#endif