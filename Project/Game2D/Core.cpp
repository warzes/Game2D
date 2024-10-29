#include "stdafx.h"
#include "Core.h"

//=============================================================================
#pragma region [ Log ]

extern bool IsShouldClose;

void Print(const std::string& msg)
{
	puts(msg.c_str());
}

void Warning(const std::string& msg)
{
	Print("WARNING: " + msg);
}

void Error(const std::string& msg)
{
	Print("ERROR: " + msg);
}

void Fatal(const std::string& msg)
{
	Print("FATAL: " + msg);
	IsShouldClose = true;
}

#pragma endregion