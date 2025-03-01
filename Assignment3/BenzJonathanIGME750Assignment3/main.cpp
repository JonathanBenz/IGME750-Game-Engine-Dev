// Author: Jonathan Benz
// 02/16/2025
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Audio.hpp>

#include "game.h"
#include "sprite.h"

// ------------------------------------------ PROTOTYPE FUNCTIONS ------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// ---------------------------------------------------------------------------------------------------------

// ------------------------------------------ GLOBAL VARIABLES ---------------------------------------------
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
Game* game = new Game();
// ---------------------------------------------------------------------------------------------------------

int main()
{
	// Initialize GLFW, tell it we are using OpenGL 3.3 and to use the core profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window Object
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jonathan Benz - IGME 750 Assignment 3", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Make sure GLAD is initialized so that it can manage OpenGL Function Pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Initialize the viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Set a callback function for resizing the window
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	 
	game->Init();
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Run the render loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculating deltaTime (the time between frames)
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// USER INPUT
		game->ProcessInput(deltaTime);

		// UPDATE
		game->Update(deltaTime);

		// RENDER
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		game->Render();
		// ----------------------------------------------------------------------------------

		glfwSwapBuffers(window);
	}

	// Deallocate all glfw resources and exit the program
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		game->SpawnProjectile();
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			game->Keys[key] = true;
		else if (action == GLFW_RELEASE)
			game->Keys[key] = false;
	}
}