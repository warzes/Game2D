#pragma once

class Shader final
{
public:
	bool Create(const char* vertexCode, const char* fragmentCode);
	void Destroy();

	void Bind() const;

	int GetUniformId(const std::string& name) const;

	void SetUniform(int location, bool value) const;
	void SetUniform(int location, int value) const;
	void SetUniform(int location, float value) const;
	void SetUniform(int location, const glm::vec2& value) const;
	void SetUniform(int location, const glm::vec3& value) const;
	void SetUniform(int location, const glm::vec4& value) const;
	void SetUniform(int location, const glm::mat2& mat) const;
	void SetUniform(int location, const glm::mat3& mat) const;
	void SetUniform(int location, const glm::mat4& mat) const;

	void SetUniform(const std::string& name, bool value) const;
	void SetUniform(const std::string& name, int value) const;
	void SetUniform(const std::string& name, float value) const;
	void SetUniform(const std::string& name, const glm::vec2& value) const;
	void SetUniform(const std::string& name, const glm::vec3& value) const;
	void SetUniform(const std::string& name, const glm::vec4& value) const;
	void SetUniform(const std::string& name, const glm::mat2& mat) const;
	void SetUniform(const std::string& name, const glm::mat3& mat) const;
	void SetUniform(const std::string& name, const glm::mat4& mat) const;
private:
	bool checkShaderCompileErrors(GLuint shader, const std::string& type);
	bool checkProgramLinkErrors(GLuint program);


	unsigned int m_id{ 0 };
};