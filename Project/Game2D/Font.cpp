#include "stdafx.h"
#include "Font.h"
#include "Shader.h"
#include "Core.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

bool Font::Create(const std::filesystem::path& fileName, float fontSize)
{
	Print("loading font: " + fileName.string());
	FILE* file = fopen(fileName.string().c_str(), "rb");
	if (!file)
	{
		Error("Failed to open font file: " + fileName.string());
		return false;
	}
	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* buf = (unsigned char*)malloc(filesize);
	fread(buf, filesize, 1, file);
	fclose(file);
	this->sz = sz;
	stbtt_InitFont(&this->info, buf, 0);
	float scale = stbtt_ScaleForPixelHeight(&this->info, sz) * 10.;
	stbtt_GetFontVMetrics(&this->info, &this->asc, &this->des, &this->linegap);
	this->newline = this->asc - this->des + this->linegap;
	this->newline *= scale;
	for (int i = 32; i != 128; i++)
	{
		//CRINFO("loading font [%i/%i] %i%%", i, 128, (i * 100) / 128);
		Glyph* glyph = (Glyph*)malloc(sizeof(Glyph));
		glyph->bitmap = stbtt_GetCodepointSDF(&this->info, scale, i, 5, 180, 10., &glyph->w, &glyph->h, &glyph->xoff, &glyph->yoff);
		stbtt_GetCodepointHMetrics(&this->info, i, &glyph->xadvance, NULL);
		stbtt_GetCodepointBitmapBox(&this->info, i, scale, scale, &glyph->xmin, &glyph->xmax, &glyph->bearx, &glyph->beary);
		this->glyphs[i] = glyph;
		if (glyph->bitmap == NULL)
		{
			Warning("empty SDF generated for character code " + std::to_string(i));
		}
	}
	fputs("\033[A\033[2K", stdout);
	rewind(stdout);
	Print("font loaded");

	Print("uploading font to gpu...");
	for (int i = 32; i < 128; i++)
	{
		Glyph* glyph = this->glyphs[i];
		glGenTextures(1, &glyph->id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, glyph->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->w, glyph->h, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap);
		if (glGetError() != GL_NO_ERROR)
		{
			//Warning("failed to upload glyph to gpu: %i '%c'", i, i);
			glDeleteTextures(1, &glyph->id);
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
		Glyph* glyph = this->glyphs[i];
		glDeleteTextures(1, &glyph->id);
		free(glyph->bitmap);
		free(glyph);
	}
}

void Font::RenderGlyph(const Shader& shader, Glyph* glyph, Vec2 pos, Vec2 size, Vec4 color)
{
	Vec2 rpos = { 0.f, 0.f };
	if (glyph == NULL) return;
	shader.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, glyph->id);
	Mat4 model;
	model.Identity();

	Translate(model, { pos[0], pos[1], 0.f });
	Vec3 scaleVec = { size[0], size[1], 1.f };
	Scale(model, scaleVec);
	Translate(model, { rpos[0], rpos[1], 0.f });
	glUniformMatrix4fv(glGetUniformLocation(gfx->textshd.id, "model"), 1, false, (const GLfloat*)model);
	glUniformMatrix4fv(glGetUniformLocation(gfx->textshd.id, "projection"), 1, false, (const GLfloat*)gfx->proj);
	glUniform4f(glGetUniformLocation(gfx->textshd.id, "textColor"), color[0], color[1], color[2], color[3]);
	glBindVertexArray(gfx->bVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindVertexArray(0);
}

void Font::RenderText(const Shader& shader, const char* text, Vec2 pos, float scale, Vec2 spacing, Vec4 color, float max_x_length)
{
	Vec2 cursor = { pos[0], pos[1] };
	int text_length = strlen(text);
	for (int i = 0; i < text_length; i++)
	{
		Glyph* glyph = glyphs[(int)text[i]];
		if (text[i] == '\n')
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			continue;
		}
		else if (text[i] == '\t')
		{
			cursor[0] += glyphs[(int)' ']->xadvance * scale * 4;
			continue;
		}
		Vec2 ssz = { glyph->w * scale, glyph->h * scale };
		Vec2 sof = { glyph->xoff * scale, glyph->yoff * scale };
		Vec2 spc;
		spc[0] = ((sz * glyph->xadvance) >> 6) * scale * spacing[0];
		spc[1] = newline * scale * spacing[1];
		Vec2 glyphPos = { cursor[0] + sof[0], cursor[1] + sof[1] };
		if (glyphPos[0] + ssz[0] + spc[0] > max_x_length && max_x_length != 0.)
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			glyphPos[0] = cursor[0] + sof[0];
			glyphPos[1] = cursor[1] + sof[1];
		}
		RenderGlyph(shader, glyph, glyphPos, ssz, color);
		cursor[0] += spc[0];
	}
}

void Font::RenderWavyText(const Shader& shader, const char* text, Vec2 pos, float scale, Vec2 spacing, Vec4 color, float max_x, float amplitude, float frequency, float phase)
{
	Vec2 cursor = { pos[0], pos[1] };
	int text_length = strlen(text);
	for (int i = 0; i < text_length; i++)
	{
		Glyph* glyph = glyphs[(int)text[i]];
		if (text[i] == '\n')
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			continue;
		}
		else if (text[i] == '\t')
		{
			cursor[0] += glyphs[(int)' ']->xadvance * scale * 4;
			continue;
		}
		Vec2 ssz = { glyph->w * scale, glyph->h * scale };
		Vec2 sof = { glyph->xoff * scale, glyph->yoff * scale };
		Vec2 spc;
		spc[0] = ((sz * glyph->xadvance) >> 6) * scale * spacing[0];
		spc[1] = newline * scale * spacing[1];
		Vec2 glyphPos = { cursor[0] + sof[0], cursor[1] + sof[1] };
		if (glyphPos[0] + ssz[0] + spc[0] > max_x && max_x != 0.)
		{
			cursor[0] = pos[0];
			cursor[1] += newline * scale * spacing[1];
			glyphPos[0] = cursor[0] + sof[0];
			glyphPos[1] = cursor[1] + sof[1];
		}
		glyphPos[1] += amplitude * sin(frequency * (glyphPos[0] + phase));
		RenderGlyph(shader, glyph, glyphPos, ssz, color);
		cursor[0] += spc[0];
	}
}

https://github.com/ppekko/r2
https://github.com/blurrypiano/littleVulkanEngine