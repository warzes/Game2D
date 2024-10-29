#include "stdafx.h"
#include "Platform.h"
#include "Core.h"

bool IsShouldClose = false;

#if defined(_MSC_VER)
#	pragma comment( lib, "glfw3.lib" )
#endif

bool Platform::Create(const PlatformCreateInfo& createInfo)
{
	m_width = createInfo.width;
	m_height = createInfo.height;

	

	IsShouldClose = false;
	return true;
}

void Platform::Destroy()
{
}

void Platform::Update()
{
}

bool Platform::ShouldClose() const
{
	return IsShouldClose;
}