#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite
{
private:
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = projection * model * vec4(aPos, 1.0f);\n"
		"}\0";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 color;\n"
		"uniform vec3 spriteColor;\n"
		"void main()\n"
		"{\n"
		"  color = vec4(spriteColor, 1.0f);\n"
		"}\0";

	unsigned int shaderProgram;
	unsigned int VAO;
	void initRenderData();

public:
	Sprite();
	~Sprite();

	unsigned int compileAndLinkShaderObject();
	void DrawSprite(glm::vec2 position, glm::vec2 size, float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
};

#endif