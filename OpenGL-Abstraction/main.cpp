#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "TestMultipleObjects.h"
#include "TestClearColor.h"

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
       
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Test Clear Color");
        testMenu->RegisterTest<test::TestMultipleObjects>("Test Multiple Objects");

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();

                ImGui::Begin(currentTest->testName.c_str());
                //this is hacky solution, maybe using enums mayb e more suitable
                if (currentTest->testName != "Test Menu" && ImGui::Button("Back"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    

        if (currentTest != testMenu)
        {
            delete testMenu;
            testMenu = nullptr;
        }
        delete currentTest;
        currentTest = nullptr;
        
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}