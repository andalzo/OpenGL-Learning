#include "Shader.h"
#include "Utility.h"
#include <fstream>
#include <iostream>
#include <sstream>


Shader::Shader(const std::string& filepath) : m_FilePath(filepath)
{
    ShaderProgramSource source = ParseShader();
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::SetPath(const std::string& filepath)
{
    m_FilePath = filepath;
    ShaderProgramSource source = ParseShader();
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, const int& value)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, const float& value)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value));
}
void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniform2f(location, value.x, value.y));
}
void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniform3f(location, value.x, value.y, value.z));
}
void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]));
    
}
void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache.at(name);

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: Uniform " << name << " doesn't exist" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram()); //We need to create a program context
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //Our shader will be compiled into our program with an uid 
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs)); //We need to attach our program with shader,
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program)); //We need to link program with our current program

    GLCall(glValidateProgram(program)); //Finally, we need to vlaidate or program

    GLCall(glDeleteShader(vs)); //After, attaching our shaders with program context, we can delete our shaders
    GLCall(glDeleteShader(fs));

    return program;
}


ShaderProgramSource Shader::ParseShader()
{
    std::ifstream stream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];


    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type != ShaderType::NONE)
            {
                ss[static_cast<int>(type)] << line << "\n";
            }
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type)); //We need to create shader context
    const char* src = source.c_str(); //We need to extract war char arrays from c++ string
    GLCall(glShaderSource(id, 1, &src, nullptr)); //We should feed our shader context with source
    //First one is the id of our shader context, 
    //second one is the number of shaders we want to give it, 
    //third one is the pointer of pointer of first char in our shader's source code (which is a pure string actually)
    //fourth one is the size of char pointer, if our source string has null terminatede char "/0", we can pass nullptr

    GLCall(glCompileShader(id)); //This compiles shader

    //Error Handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); //Get the compile status

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); //Get the length of message

        char* message = new char[length]; //Allocate memory in heap dynamically, because we need to raw chars
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile shader:[";
        if (type == GL_VERTEX_SHADER)
        {
            std::cout << "VERTEX SHADER]:" << std::endl;
        }
        if (type == GL_FRAGMENT_SHADER)
        {
            std::cout << "FRAGMENT SHADER]:" << std::endl;
        }

        std::cout << message << std::endl;

        delete[] message;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

