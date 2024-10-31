#include "stdafx.h"
#include "Core.h"
#include "Platform.h"
#include "GraphicsSystem.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Framebuffer.h"
#include "Font.h"

#if defined(_MSC_VER)
#	pragma comment( lib, "3rdpartyLib.lib" )
#endif

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

//uniform vec4 uColor;
uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}
)";

int main()
{
	Platform platform;
	GraphicsSystem graphics;
	if (   platform.Create({})
		&& graphics.Create())
	{
		glEnable(GL_DEPTH_TEST);

		float vertices[] = {
			 // vertex            // color            // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,       // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,       // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,       // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,       // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coords attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		Shader shader;
		shader.Create(vertexShaderSource, fragmentShaderSource);

		Texture2D texture;
		texture.Create("container.jpg");

		Framebuffer fb;
		fb.Create(120, 120);


		Font font;
		font.Create("monaspace.ttf", 24);

		graphics.orthoProj = glm::ortho(0.0f, 1600.0f, 900.0f, 0.0f, -1.0f, 1.0f);

		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		while (!platform.ShouldClose())
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (platform.IsKeyPress(GLFW_KEY_ESCAPE))
				platform.Exit();

			glEnable(GL_DEPTH_TEST);
			fb.Bind();
			{
				shader.Bind();
				texture.Bind();
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}

			glDisable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, platform.GetWindowWidth(), platform.GetWindowHeight());
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			fb.BindColorTexture();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			font.RenderWavyText(graphics, "the quick brown fox jumped \nover the lazy dog", { 100, 400 }, 0.25, { 0.4, 1.f }, { 1.f, 0.f, 1.f, 1.f }, 0.f, 5.f, 2.f, static_cast<float>(glfwGetTime()));
			glDisable(GL_BLEND);

			platform.Update();
		}
		font.Destroy();
		fb.Destroy();
		texture.Destroy();
		shader.Destroy();
	}
	graphics.Destroy();
	platform.Destroy();
}