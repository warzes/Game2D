#include "stdafx.h"
#include "Font.h"
#include "GraphicsSystem.h"
#include "Core.h"

bool Font::Create(const std::filesystem::path& fileName, float FontSize)
{
	Print("loading font: " + fileName.string());

	FILE* fontFile{ nullptr };
	errno_t result = fopen_s(&fontFile, fileName.string().c_str(), "rb");
	if (!fontFile || result)
	{
		Error("Failed to open font file: " + fileName.string());
		return false;
	}
	fseek(fontFile, 0, SEEK_END);
	size_t fileSize = ftell(fontFile);
	fseek(fontFile, 0, SEEK_SET);
	
	std::vector<unsigned char> fontData(fileSize);
	fread(reinterpret_cast<char*>(fontData.data()), fileSize, 1, fontFile);
	fclose(fontFile);

	int stbres = stbtt_InitFont(&fontInfo, fontData.data(), 0);
	if (stbres == 0) 
	{
		Error("Font parse failed");
		return false;
	}

	fontSize = FontSize;

https://programmersought.com/article/94699807717/

	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

	float scale = stbtt_ScaleForPixelHeight(&fontInfo, FontSize) * 10.;

	newline = ascent - descent + lineGap;
	newline *= scale;
	for (int i = 32; i != 128; i++)
	{		
		Glyph& glyph = glyphs[i];
		glyph.bitmap = stbtt_GetCodepointSDF(&fontInfo, scale, i, 5, 180, 10., &glyph.w, &glyph.h, &glyph.xoff, &glyph.yoff);
		stbtt_GetCodepointHMetrics(&fontInfo, i, &glyph.xadvance, nullptr);
		stbtt_GetCodepointBitmapBox(&fontInfo, i, scale, scale, &glyph.xmin, &glyph.xmax, &glyph.bearx, &glyph.beary);
	}
	Print("font loaded");

	Print("uploading font to gpu...");
	for (int i = 32; i < 128; i++)
	{
		Glyph& glyph = glyphs[i];

		if (!glyph.bitmap) continue;

		glGenTextures(1, &glyph.id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, glyph.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph.w, glyph.h, 0, GL_RED, GL_UNSIGNED_BYTE, glyph.bitmap);
		if (glGetError() != GL_NO_ERROR)
		{
			Warning("failed to upload glyph to gpu: " + std::to_string(i));
			glDeleteTextures(1, &glyph.id);
			glyph.id = 0;
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	Print("font uploaded");
	
	return true;
}

void Font::Destroy()
{
	Print("destroying font...");
	for (int i = 32; i < 128; i++)
	{
		Glyph& glyph = glyphs[i];
		glDeleteTextures(1, &glyph.id);
		if (glyph.bitmap) free(glyph.bitmap);
	}
}

void Font::RenderGlyph(const GraphicsSystem& graphics, Glyph& glyph, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
	if (!glyph.id) return;
	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { pos[0], pos[1], 0.0f });
	model = glm::scale(model, { size[0], size[1], 1.f });

	graphics.font.shader.Bind();
	graphics.font.shader.SetUniform("model", model);
	graphics.font.shader.SetUniform("projection", graphics.orthoProj);
	graphics.font.shader.SetUniform("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glyph.id);

	glBindVertexArray(graphics.font.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Font::RenderText(const GraphicsSystem& graphics, const char* text, const glm::vec2& pos, float scale, const glm::vec2& spacing, const glm::vec4& color, float max_x_length)
{
	glm::vec2 cursor = { pos[0], pos[1] };
	int text_length = strlen(text);
	for (int i = 0; i < text_length; i++)
	{
		Glyph& glyph = glyphs[(int)text[i]];
		if (text[i] == '\n')
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			continue;
		}
		else if (text[i] == '\t')
		{
			cursor[0] += glyphs[(int)' '].xadvance * scale * 4;
			continue;
		}
		glm::vec2 ssz = { glyph.w * scale, glyph.h * scale };
		glm::vec2 sof = { glyph.xoff * scale, glyph.yoff * scale };
		glm::vec2 spc;
		spc[0] = ((fontSize * glyph.xadvance) >> 6) * scale * spacing[0];
		spc[1] = newline * scale * spacing[1];
		glm::vec2 glyphPos = { cursor[0] + sof[0], cursor[1] + sof[1] };
		if (glyphPos[0] + ssz[0] + spc[0] > max_x_length && max_x_length != 0.)
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			glyphPos[0] = cursor[0] + sof[0];
			glyphPos[1] = cursor[1] + sof[1];
		}
		RenderGlyph(graphics, glyph, glyphPos, ssz, color);
		cursor[0] += spc[0];
	}
}

void Font::RenderWavyText(const GraphicsSystem& graphics, const char* text, const glm::vec2& pos, float scale, const glm::vec2& spacing, const glm::vec4& color, float max_x, float amplitude, float frequency, float phase)
{
	glm::vec2 cursor = { pos[0], pos[1] };
	int text_length = strlen(text);
	for (int i = 0; i < text_length; i++)
	{
		Glyph& glyph = glyphs[(int)text[i]];
		if (text[i] == '\n')
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			continue;
		}
		else if (text[i] == '\t')
		{
			cursor[0] += glyphs[(int)' '].xadvance * scale * 4;
			continue;
		}
		glm::vec2 ssz = { glyph.w * scale, glyph.h * scale };
		glm::vec2 sof = { glyph.xoff * scale, glyph.yoff * scale };
		glm::vec2 spc;
		spc[0] = ((fontSize * glyph.xadvance) >> 6) * scale * spacing[0];
		spc[1] = newline * scale * spacing[1];
		glm::vec2 glyphPos = { cursor[0] + sof[0], cursor[1] + sof[1] };
		if (glyphPos[0] + ssz[0] + spc[0] > max_x && max_x != 0.)
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			glyphPos[0] = cursor[0] + sof[0];
			glyphPos[1] = cursor[1] + sof[1];
		}
		glyphPos[1] += amplitude * sin(frequency * (glyphPos[0] + phase));
		RenderGlyph(graphics, glyph, glyphPos, ssz, color);
		cursor[0] += spc[0];
	}
}