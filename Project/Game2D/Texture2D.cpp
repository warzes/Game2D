#include "stdafx.h"
#include "Texture2D.h"
#include "Core.h"

bool Texture2D::Create(const std::filesystem::path& fileName)
{
	assert(!m_id);
	if (!std::filesystem::exists(fileName))
	{
		Error("texture file '" + fileName.string() + "' does not exist");
		return false;
	}

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName.string().c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	if (!data)
	{
		Error("texture file '" + fileName.string() + "' failed");
		return false;
	}

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return false;
}

void Texture2D::Destroy()
{
	if (m_id) glDeleteTextures(1, &m_id);
	m_id = 0;
}

void Texture2D::Bind(unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}