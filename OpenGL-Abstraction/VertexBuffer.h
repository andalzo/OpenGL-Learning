#pragma once

class VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_RendererID = 0;
};

