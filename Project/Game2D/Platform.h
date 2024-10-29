#pragma once

#if PLATFORM_WIN32

struct PlatformCreateInfo final
{
	uint32_t         width = 1600;
	uint32_t         height = 900;
	std::string_view title = "Game";
	bool             fullScreen = false;
};

class Platform final
{
public:
	bool Create(const PlatformCreateInfo& createInfo);
	void Destroy();

	void Update();
	bool ShouldClose() const;

private:
	uint32_t    m_width;
	uint32_t    m_height;
	GLFWwindow* m_window = nullptr;
};

#endif // PLATFORM_WIN32