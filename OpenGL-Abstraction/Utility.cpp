#include "Utility.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR]: (" << error << ") in function named " << function << " in file named " << file <<
            " in line " << line << std::endl;
        return false;
    }
    return true;
}