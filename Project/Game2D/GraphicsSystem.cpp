#include "stdafx.h"
#include "GraphicsSystem.h"

bool GraphicsSystem::Create()
{
	// fonts data
	{
		const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 aPos;

uniform mat4 model;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
	gl_Position = projection * model * vec4(aPos.xy, 0.0, 1.0);
	TexCoords = aPos.zw;
}
)";

		const char* fragmentShaderSource = R"(
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{
	float sdf = texture(text, TexCoords).r;
	float smoothness = fwidth(sdf) * 0.5;
	float edge = smoothstep(0.5 - smoothness, 0.5 + smoothness, sdf);
	FragColor = vec4(textColor.rgb, textColor.a * edge);
}
)";
		if (!font.shader.Create(vertexShaderSource, fragmentShaderSource))
			return false;

		glGenVertexArrays(1, &font.VAO);
		glBindVertexArray(font.VAO);

		float vertices[] = {
				0.f, 1.f, 0.f, 1.f,
				1.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 0.f,

				0.f, 1.f, 0.f, 1.f,
				1.f, 1.f, 1.f, 1.f,
				1.f, 0.f, 1.f, 0.f
		};
		glGenBuffers(1, &font.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, font.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}

	return true;
}

void GraphicsSystem::Destroy()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &font.VAO);
	glDeleteBuffers(1, &font.VBO);
	font.shader.Destroy();
}
