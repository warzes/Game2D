#include "stdafx.h"
#include "Shader.h"
#include "Core.h"

bool Shader::Create(const char* vertexCode, const char* fragmentCode)
{
	assert(!m_id);

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, nullptr);
	glCompileShader(vertex);
	if (!checkShaderCompileErrors(vertex, "VERTEX")) return false;
	
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, nullptr);
	glCompileShader(fragment);
	if (!checkShaderCompileErrors(fragment, "FRAGMENT")) return false;

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);
	bool result = checkProgramLinkErrors(m_id);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return result;
}

void Shader::Destroy()
{
	if (m_id) glDeleteProgram(m_id);
	m_id = 0;
}

void Shader::Bind() const
{
	glUseProgram(m_id);
}

int Shader::GetUniformId(const std::string& name) const
{
	return glGetUniformLocation(m_id, name.c_str());
}

void Shader::SetUniform(int location, bool value) const
{
	glUniform1i(location, (int)value);
}

void Shader::SetUniform(int location, int value) const
{
	glUniform1i(location, value);
}

void Shader::SetUniform(int location, float value) const
{
	glUniform1f(location, value);
}

void Shader::SetUniform(int location, const glm::vec2& value) const
{
	glUniform2fv(location, 1, &value[0]);
}

void Shader::SetUniform(int location, const glm::vec3& value) const
{
	glUniform3fv(location, 1, &value[0]);
}

void Shader::SetUniform(int location, const glm::vec4& value) const
{
	glUniform4fv(location, 1, &value[0]);
}

void Shader::SetUniform(int location, const glm::mat2& mat) const
{
	glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniform(int location, const glm::mat3& mat) const
{
	glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniform(int location, const glm::mat4& mat) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniform(const std::string& name, bool value) const
{
	SetUniform(GetUniformId(name), value);
}

void Shader::SetUniform(const std::string& name, int value) const
{
	SetUniform(GetUniformId(name), value);
}

void Shader::SetUniform(const std::string& name, float value) const
{
	SetUniform(GetUniformId(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) const
{
	SetUniform(GetUniformId(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) const
{
	SetUniform(GetUniformId(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) const
{
	SetUniform(GetUniformId(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::mat2& mat) const
{
	SetUniform(GetUniformId(name), mat);
}

void Shader::SetUniform(const std::string& name, const glm::mat3& mat) const
{
	SetUniform(GetUniformId(name), mat);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& mat) const
{
	SetUniform(GetUniformId(name), mat);
}

bool Shader::checkShaderCompileErrors(GLuint shader, const std::string& type)
{
	GLint success;
	GLchar infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
		Error("Failed to compile " + type + " shader: " + "\n" + infoLog + "\n -- --------------------------------------------------- -- ");
		return false;
	}
	return true;
}

bool Shader::checkProgramLinkErrors(GLuint program)
{
	GLint success;
	GLchar infoLog[1024];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, nullptr, infoLog);
		Error("Failed to link shader." + std::string("\n") + infoLog + "\n -- --------------------------------------------------- -- ");
		return false;
	}

	return true;
}