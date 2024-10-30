#pragma once

class Texture2D final
{
public:
	bool Create(const std::filesystem::path& fileName);
	void Destroy();

	void Bind(unsigned slot = 0);
private:
	unsigned int m_id{ 0 };
};