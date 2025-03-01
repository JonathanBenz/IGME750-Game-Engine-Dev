#include "sprite.h"
#include <iostream>

Sprite::Sprite() { initRenderData(); }
Sprite::~Sprite() { }

unsigned int Sprite::compileAndLinkShaderObject()
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for successful compilation, else print error message
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR: Shader Vertex Compilation Failed!\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Now do the same exact above steps but for the Fragment Shader this time
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR: Shader Fragment Compilation Failed!\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check to see that linking was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR: Shader Program Linking Failed!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Sprite::initRenderData()
{
	shaderProgram = compileAndLinkShaderObject();

	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		0.0f, 1.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::DrawSprite(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	glUseProgram(shaderProgram);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// prepare transformations
	glm::mat4 model = glm::mat4(1.0f); // initialize identity matrix
	
	// translation
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// rotation (translating halfway inward so that the center of rotation is within the middle of the sprite instead of in the corner
	//           after rotating from that center point, we translate it back to where it was before)
	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0));

	// scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	// change uniform values in our shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderProgram, "spriteColor"), color.x, color.y, color.z);
	
	// Draw
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}