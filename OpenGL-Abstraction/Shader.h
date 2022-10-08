#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;

};


class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	//Set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	std::string m_FilePath;
	unsigned int m_RendererID = 0;
	std::unordered_map<std::string, unsigned int> m_UniformLocationCache;


	unsigned int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader();
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	

};