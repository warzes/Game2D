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
			platform.Update();
		}
	}
	platform.Destroy();
}