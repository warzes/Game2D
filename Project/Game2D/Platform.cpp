#include "stdafx.h"
#include "Platform.h"
#include "Core.h"

bool IsShouldClose = false;

#if defined(_MSC_VER)
#	pragma comment( lib, "glfw3.lib" )
#endif

void framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept
{
	glViewport(0, 0, width, height);
}

bool Platform::Create(const PlatformCreateInfo& createInfo)
{
	m_width = createInfo.width;
	m_height = createInfo.height;

	if (!glfwInit())
	{
		Fatal("Failed to initialize GLFW");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // backward compatibility

	m_window = glfwCreateWindow(m_width, m_height, "LearnOpenGL", NULL, NULL);
	if (m_window == NULL)
	{
		Fatal("Failed to create GLFW window");
		return false;
	}
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		Fatal("Failed to initialize GLAD");
		return false;
	}

	glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

	glViewport(0, 0, m_width, m_height);

	IsShouldClose = false;
	return true;
}

void Platform::Destroy()
{
	glfwTerminate();
}

void Platform::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

bool Platform::ShouldClose() const
{
	return glfwWindowShouldClose(m_window) || IsShouldClose;
}

void Platform::Exit()
{
	IsShouldClose = true;
}

bool Platform::IsKeyPress(int key)
{
	return (glfwGetKey(m_window, key) == GLFW_PRESS);
}