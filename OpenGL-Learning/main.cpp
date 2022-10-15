#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

//This is intristic to MSVC
#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef _DEBUG
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR]: (" << error << ") in function named " << function << " in file named " << file << 
        " in line " << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;

};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];


    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type != ShaderType::NONE)
            {
                ss[static_cast<int>(type)] << line << "\n";
            }
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type)); //We need to create shader context
    const char* src = source.c_str(); //We need to extract war char arrays from c++ string
    GLCall(glShaderSource(id, 1, &src, nullptr)); //We should feed our shader context with source
    //First one is the id of our shader context, 
    //second one is the number of shaders we want to give it, 
    //third one is the pointer of pointer of first char in our shader's source code (which is a pure string actually)
    //fourth one is the size of char pointer, if our source string has null terminatede char "/0", we can pass nullptr

    GLCall(glCompileShader(id)); //This compiles shader

    //Error Handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); //Get the compile status
    
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); //Get the length of message
        
        char* message = new char[length]; //Allocate memory in heap dynamically, because we need to raw chars
        GLCall(glGetShaderInfoLog(id, length, &length, message));
       
        std::cout << "Failed to compile shader:[";
        if (type == GL_VERTEX_SHADER)
        {
            std::cout << "VERTEX SHADER]:" << std::endl;
        }
        if (type == GL_FRAGMENT_SHADER)
        {
            std::cout << "FRAGMENT SHADER]:" << std::endl;
        }

        std::cout << message  << std::endl;

        delete[] message;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram()); //We need to create a program context
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader); //Our shader will be compiled into our program with an uid 
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs)); //We need to attach our program with shader,
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program)); //We need to link program with our current program

    GLCall(glValidateProgram(program)); //Finally, we need to vlaidate or program

    GLCall(glDeleteShader(vs)); //After, attaching our shaders with program context, we can delete our shaders
    GLCall(glDeleteShader(fs));

    return program;
}

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

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer)); 
    //We need to give an id to our buffer; don't use 0 here; our id will be assigned to given address
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); 
    //We selecting(binding) our buffer, the opengl is a state machine, so here we are updating our state
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW)); 
    //We supply data to buffer with size and hints

    GLCall(glEnableVertexAttribArray(0)); //We want to enable our attribute with its index
    
    GLCall(glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(float), static_cast<const void*>(0)));
    //This code links buffer to vao; setting up layout is linking it!!
    
    /*
    * This function determines the layout of buffer we want to send GPU, the parameters are:
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
    * 6. Which index we start to define this attrib, for instance if we want to select rgba after 3 doubles, we want to gice 3*sizeof(double) 
    * to access this attribute in vertex
    * 
    */
    
    unsigned int ibo;
   
    GLCall(glGenBuffers(1, &ibo));
    //We need to give an id to our buffer; don't use 0 here; our id will be assigned to given address
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    //We selecting(binding) our buffer, the opengl is a state machine, so here we are updating our state
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    //We supply data to buffer with size and hints

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader)); //We need to bind program

    GLCall(int location = glGetUniformLocation(shader, "u_Color")); //We acces our uniform in shader from cpu
    ASSERT(location != -1);
    
    GLCall(glUniform4f(location, 0.2f, 0.7f, 0.8f, 1.0f));

    
    //This section resets all the bindings
    //After specify our vertex array with specific vertex buffer with its layout we can cast to zero to use later
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glUseProgram(0));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        
    float r = 0.0f;
    float increment = 0.05f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));


        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.7f, 0.8f, 1.0f))
        
        GLCall(glBindVertexArray(vao));
            
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); 

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;
        
        //Drawing triangle
        //Second one is the indice that we want start with
        //Third one is to number of indices (not numbeerr of all elements!!) , here it is vect2

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    
    GLCall(glDeleteProgram(shader)); //We need to close gpu program context
    glfwTerminate();
    return 0;
}