#pragma once

class Vec2;
class Vec3;
class Vec4;
class Mat2;
class Mat3;
class Mat4;

class Shader final
{
public:
	bool Create(const char* vertexCode, const char* fragmentCode);
	void Destroy();

	void Bind() const;

	void SetUniform(const std::string& name, bool value) const;
	void SetUniform(const std::string& name, int value) const;
	void SetUniform(const std::string& name, float value) const;

private:
	unsigned int m_id{ 0 };
};