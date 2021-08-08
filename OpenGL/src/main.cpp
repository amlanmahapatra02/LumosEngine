#include <iostream>

#include<GL/glew.h>
#include <glfw3.h>

// window dimensions
int WIDTH = 800, HEIGHT = 800;

unsigned int VAO, VBO, shader;

//Vertex Shader
static const char* vShader = "                      \n\
#version 330                                        \n\
                                                    \n\
layout (location = 0) in vec3 pos;                  \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
     gl_Position = vec4(0.4 * pos,1.0);     \n\
}";

//Fragment shader
static const char* fShader = "              \n\
#version 330                                \n\
                                            \n\
out vec4 Color;                             \n\
                                            \n\
void main()                                 \n\
{                                           \n\
     Color = vec4(0.0f,1.0,1.0,1.0);        \n\
}";



void CreateTriangle()
{
    // vertice location point of an triangle

    float vertices[] = {
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenFramebuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //Unbind the data
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(unsigned int theProgram, const char* shaderCode, GLenum shaderType)
{

    unsigned int theShader = glCreateShader(shaderType);

    const char* theCode[1];
    theCode[0] = shaderCode;

    int codelength[1];
    codelength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codelength);
    glCompileShader(theShader);

    int result = 0;
    char log[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(log), NULL, log);
        std::cout << "Error Compiling Shader" << shaderType << "  " << log << std::endl;
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShader()
{
    shader = glCreateProgram();
    if (!shader)
    {
        std::cout << "Error Creating Shader !" << std::endl;
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    //debugging part for console

    int result = 0;
    char log[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(log), NULL, log);
        std::cout << "Error Linking Program : " << log << std::endl;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(log), NULL, log);
        std::cout << "Error Validating Shaders : " << log << std::endl;
        return;
    }

}

int main(void)
{
    //Initialise GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW initialisation failed !" << std::endl;
        glfwTerminate();

        return 1;
    }


    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

    if (!mainWindow)
    {
        std::cout << "GLFW window creation failed " << std::endl;
        glfwTerminate();
        return 1;
    }

    // Get the Buffer size info
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set contex for glew to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extenison features
    glewExperimental = GL_TRUE;

    //Initialise GLEW

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew initialisation failed ! " << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();

        return 1;
    }

    //Setup ViewPort size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShader();


    // Loop until window close
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Handle User-Input
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }


    return 0;
}