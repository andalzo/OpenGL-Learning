#include "IndexBuffer.h"
#include "Utility.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    //Get an unique id from opengl to our '1' vertex buffer
    GLCall(glGenBuffers(1, &m_RendererID));
    
    Bind();

    //Give information to opengl about our data (data mode, data size, data itself, and two hints (DRAW and STATIC)
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count*sizeof(unsigned int), data, GL_STATIC_DRAW));
    //TODO: Maybe we can also need the type hint parameter here
    
}

IndexBuffer::~IndexBuffer()
{
    //Deleting the ÝNDEX buffer
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    //We selecting(binding) our buffer, the opengl is a state machine, so here we are updating our state
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    
}

void IndexBuffer::UnBind() const
{
    //Setting our choice to zero in GL_ARRAY_BUFFER mode
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
