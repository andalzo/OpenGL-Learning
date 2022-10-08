#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "Renderer.h"

int main()
{
    GLFWwindow* window;

    /* Initialize the library GLFW*/
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "[GLEW INIT ERROR]: Could not init opengl\n";
    }


    // Printing the version of openGL
    std::cout << glGetString(GL_VERSION) << std::endl;

    /*
    * Enable vertical sync, meaning that the GPU will only render
    * when the monitor is ready for the next frame, at 60Hz vs. vs.
    */
    glfwSwapInterval(1);

    { //We need this internal scope to prevent error searching keeps running infinitely, this is a issue with opengl
        float positions[] =
        {
            -0.5f,  -0.5f,
             0.5f,  -0.5f,
             0.5f,   0.5f,
            -0.5f,   0.5f,
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        IndexBuffer ib(indices, 6);
        
        Shader shader("res/shaders/Basic.shader");

        float r = 0.0f;
        float increment = 0.05f;

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.7f, 0.8f, 1.0f);
            
            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    glfwTerminate();
    return 0;
}