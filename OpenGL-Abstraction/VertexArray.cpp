#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Utility.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const std::vector<VertexBufferElement>& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const VertexBufferElement& element = elements.at(i);
        GLCall(glEnableVertexAttribArray(i));
        //We want to enable our attribute with its index
        
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized , 
                                     layout.GetStride(), (const void*)offset));
        /*
        * This function determines the layout of buffer we want to send GPU, the parameters are:
        *
        * 1. The Index Of Our Attribute in Vertes , For instance we can have (x,y,z), (red, green, blue, alpha)
        * so if we want to specify positions, we should write 0
        *
        * 2. The size of attribute, (not actual size, but the size of elements.) If we want to select 3d positions, we should write 3.
        *
        * 3. The type of elements in this attrib, (GlType)
        *
        * 4. If GL_TRUE opengl normalizes our numbers to between -1.0, 1.0
        *
        * 5. The size of attributes (bytes), ýf we want to select positions with doubles we should give 3*sizeof(double)
        *
        * 6. Which index we start to define this attrib, for instance if we want to select rgba after 3 doubles, we want to give 3*sizeof(double)
         * to access this attribute in vertex
         */

        offset += element.count*VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
    GLCall(glBindVertexArray(0));
}

