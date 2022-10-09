#pragma once
#include "Utility.h"
#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:
	unsigned int m_RendererID = 0;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer = nullptr;
	int m_Width = 0, m_Height = 0, m_BPP = 0;
};

