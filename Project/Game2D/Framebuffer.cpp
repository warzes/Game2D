#include "stdafx.h"
#include "Framebuffer.h"
#include "Core.h"

bool Framebuffer::Create(uint32_t width, uint32_t height)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &m_depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Error("Framebuffer not complete");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_width = width;
	m_height = height;

	return true;
}

void Framebuffer::Destroy()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
	if (m_textureColorBuffer) glDeleteTextures(1, &m_textureColorBuffer);
	if (m_depthRBO) glDeleteRenderbuffers(1, &m_depthRBO);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_width, m_height);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	Destroy();
	Create(width, height);
}

void Framebuffer::BindColorTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
}