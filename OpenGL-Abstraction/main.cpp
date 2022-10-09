#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "Texture.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        IndexBuffer ib(indices, 6);


        glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
        //This matrice normalizes our world to our monitor -1 to 1 from left  to right and...
       
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        //This matrice positions our objects according to camera
        
        Shader shader("res/shaders/Basic.shader");

        Texture texture("res/textures/government.png");
        texture.Bind();
       
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        ImGui::StyleColorsDark();

        glm::vec3 translationA(0, 0, 0);
        glm::vec3 translationB(0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA); //This matrice handles translations, rotations and scaling
                glm::mat4 mvp = proj * view * model; //Model View Projection Matrice
                
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                
                renderer.Draw(va, ib, shader);
            }

            shader.Bind();
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); //This matrice handles translations, rotations and scaling
                glm::mat4 mvp = proj * view * model; //Model View Projection Matrice
                
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }
           

            {

                ImGui::Begin("Basic Transformator"); 

                ImGui::SliderFloat("TranslationA X: ", &translationA.x, 0.0f, 640.0f);
                ImGui::SliderFloat("TranslationA Y: ", &translationA.y, 0.0f, 480.0f);

                ImGui::SliderFloat("TranslationB X: ", &translationB.x, 0.0f, 640.0f);
                ImGui::SliderFloat("TranslationB Y: ", &translationB.y, 0.0f, 480.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}