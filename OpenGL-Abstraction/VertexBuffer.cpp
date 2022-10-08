#include "VertexBuffer.h"
#include "Utility.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //Get an unique id from opengl to our '1' vertex buffer
    GLCall(glGenBuffers(1, &m_RendererID));
    
    Bind();

    //Give information to opengl about our data (data mode, data size, data itself, and two hints (DRAW and STATIC)
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    //TODO: Maybe we can also need the type hint parameter here
    
}

VertexBuffer::~VertexBuffer()
{
    //Deleting the vertex buffer
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    //We selecting(binding) our buffer, the opengl is a state machine, so here we are updating our state
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    
}

void VertexBuffer::UnBind() const
{
    //Setting our choice to zero in GL_ARRAY_BUFFER mode
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
