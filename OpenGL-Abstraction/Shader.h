#pragma once
#include <string>
#include <unordered_map>
#include "glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;

};


class Shader
{
public:
	Shader() = default;
	Shader(const std::string& filepath);
	~Shader();


	void SetPath(const std::string& filepath);
	void Bind() const;
	void UnBind() const;

	//Set Uniforms
	void SetUniform1i(const std::string& name, const int& value);
	
	void SetUniform1f(const std::string& name, const float& value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);
	
	void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	std::string m_FilePath;
	unsigned int m_RendererID = 0;
	std::unordered_map<std::string, int> m_UniformLocationCache;


	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader();
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	

};