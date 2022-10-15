#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    void Clear();
};

