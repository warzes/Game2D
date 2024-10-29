#include "stdafx.h"
#include "Platform.h"

#if defined(_MSC_VER)
#	pragma comment( lib, "3rdpartyLib.lib" )
#endif

int main()
{
	Platform platform;
	if (platform.Create({}))
	{
		while (!platform.ShouldClose())
		{
			if (platform.IsKeyPress(GLFW_KEY_ESCAPE))
				platform.Exit();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			platform.Update();
		}
	}
	platform.Destroy();
}