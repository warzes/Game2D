#pragma once

class Framebuffer final
{
public:
	bool Create(uint32_t width, uint32_t height);
	void Destroy();
	void Bind();
	void Resize(uint32_t width, uint32_t height);

	void BindColorTexture();
private:
	GLuint   m_fbo{ 0 };
	GLuint   m_textureColorBuffer{ 0 };
	GLuint   m_depthRBO{ 0 };
	uint32_t m_width{ 0 };
	uint32_t m_height{ 0 };
};