#pragma once

class GraphicsSystem;

#define MAX_GLYPHS 128
#define SDF_SIZE 128

class Glyph final
{
public:
	unsigned int id{ 0 };
	int w{ 0 }, h{ 0 };
	int xoff{ 0 }, yoff{ 0 };
	int bearx{ 0 }, beary{ 0 };
	int xmin{ 0 }, xmax{ 0 };
	int xadvance{ 0 };
	unsigned char* bitmap{ nullptr };
};

class Font final
{
public:
	bool Create(const std::filesystem::path& fileName, float fontSize);
	void Destroy();

	void RenderGlyph(const GraphicsSystem& graphics, Glyph& glyph, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
	void RenderText(const GraphicsSystem& graphics, const char* text, const glm::vec2& pos, float scale, const glm::vec2& spacing, const glm::vec4& color, float max_x_length);
	void RenderWavyText(const GraphicsSystem& graphics, const char* text, const glm::vec2& pos, float scale, const glm::vec2& spacing, const glm::vec4& color, float max_x, float amplitude, float frequency, float phase);

	Glyph glyphs[128];
	int ascent = 0;
	int descent = 0;
	int lineGap = 0;

	int newline;
	int fontSize;
	stbtt_fontinfo fontInfo;
};