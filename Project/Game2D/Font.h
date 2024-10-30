#pragma once

class Vec2;
class Vec4;
class Shader;

#define MAX_GLYPHS 128
#define SDF_SIZE 128

class Glyph final
{
public:
	unsigned int id;
	int w, h;
	int xoff, yoff;
	int bearx, beary;
	int xmin, xmax;
	int xadvance;
	unsigned char* bitmap;
};

class Font final
{
public:
	bool Create(const std::filesystem::path& fileName, float fontSize);
	void Destroy();

	void RenderGlyph(const Shader& shader, Glyph* glyph, Vec2 pos, Vec2 size, Vec4 color);
	void RenderText(const Shader& shader, const char* text, Vec2 pos, float scale, Vec2 spacing, Vec4 color, float max_x_length);
	void RenderWavyText(const Shader& shader, const char* text, Vec2 pos, float scale, Vec2 spacing, Vec4 color, float max_x, float amplitude, float frequency, float phase);

	Glyph* glyphs[128];
	int asc, des, linegap;
	int newline;
	int sz;
	stbtt_fontinfo info;
};