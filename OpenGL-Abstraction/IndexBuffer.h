#pragma once


class IndexBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; };

private:
	unsigned int m_RendererID = 0;
	unsigned int m_Count = 0;


};

