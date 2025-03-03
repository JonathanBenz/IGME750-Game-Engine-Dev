#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <string>

class Sprite
{
private:
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec2 aTexCoord;\n"
		"out vec2 textureCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = projection * model * vec4(aPos, 1.0f);\n"
		"  textureCoord = aTexCoord;\n"
		"}\0";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"in vec2 textureCoord;\n"
		"out vec4 color;\n"
		"uniform vec3 spriteColor;\n"
		"uniform sampler2D spriteTexture;\n"
		"void main()\n"
		"{\n"
		"  color = texture(spriteTexture, textureCoord) * vec4(spriteColor, 1.0f);\n"
		"}\0";

	unsigned int shaderProgram;
	unsigned int texture;
	unsigned int VAO;
	void initRenderData();

public:
	Sprite();
	~Sprite();

	unsigned int compileAndLinkShaderObject();
	unsigned int GenerateAndLoadTexture(std::string texturePath);
	void DrawSprite(glm::vec2 position, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), bool renderTexture = false);
};

#endif