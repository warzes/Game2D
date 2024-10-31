#pragma once

#include "Shader.h"

class GraphicsSystem final
{
public:
	bool Create();
	void Destroy();

	glm::mat4 orthoProj = glm::mat4(1.0f);
	
	struct FontData
	{
		Shader shader;
		unsigned int VBO;
		unsigned int VAO;
	} font;
};