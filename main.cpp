﻿#include <iostream>

#include <glew.h>
#include <GLFW/glfw3.h>

constexpr  GLint width = 1920, height =1080;
GLuint vao, vbo, shader, uniformXMove, uniformSize;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0002f;

// Vertex Shader
static const char* v_shader = R"(
    #version 460

    layout (location = 0 ) in vec3 pos;
    uniform float size;
    uniform float xMove; 
    
    void main()
    {
        gl_Position =  vec4(size * pos.x + xMove, size * pos.y, size * pos.z, 1.0);
    })";

// Fragment Shader
static const char* f_shader = R"(
    #version 460

    out vec4 colour;
    
    void main()
    {
        colour = vec4( 0.3f, 0.3f, 0.8f, 1.0f);
    })";

void create_triangle()
{
    GLfloat vertices[] =
        {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        };

    glGenVertexArrays(1, &vao); // Amount of Array we want to create and gives us its ID  
    glBindVertexArray(vao); // Binding our created ID, so now any openGL Buffer or Array will take palace in this ID

    glGenBuffers(1, &vbo); // Create a Buffer Object inside that VAO and give us its ID 
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW ); // Binding Data With Buffer and we cam also use GL_DYNAMIC_DRAW

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); // which element of Array we want to start form like above example will have 9/3 = 3  ==> { 0,1,2} arrays and start from 0

    glBindBuffer(GL_ARRAY_BUFFER,0); // unbind Buffer
    glBindVertexArray(0);   // unbind Array
}

void add_shader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);  // create Shader 

    const GLchar* theCode[1];       // Grab The shader Code 
    theCode[0] = shaderCode;        // Get shader Code Length 

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);      // Put that into Shader
    glCompileShader(theShader);     // Compile Shader 

    // Validate Shader 
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(theShader, sizeof(eLog), nullptr, eLog);
        std::cout<<" Error Compiling the "<< shaderType <<" : \n"<<eLog<<std::endl;
        return;
    }
    glAttachShader(theProgram, theShader);
}

void compile_shader()
{
    shader = glCreateProgram();         // Create a Program for Shader 
    if(!shader)
    {
        std::cout<<"Error Creating Shader Program!\n";
        return;
    }
    
    add_shader(shader, v_shader, GL_VERTEX_SHADER);     // Add vertex Shader to Shader Program
    add_shader(shader, f_shader, GL_FRAGMENT_SHADER);   // Add fragment Shader to Shader Program 

    // Validate program
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout<<" Error Linking Program: \n"<<eLog<<std::endl;
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout<<" Error Validating Program: \n"<<eLog<<std::endl;
        return;
    }

    // Bind Uniform Variables 
    uniformXMove = glGetUniformLocation(shader, "xMove");
    uniformSize = glGetUniformLocation(shader, "size");
}

int main()
{
    if(!glfwInit())         //initialise GLFW 
    {
        std::cout<<"GLFW initialization failed! \n";
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile = no backward Compatibility  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *mainWindow = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr);
    if(!mainWindow)
    {
        std::cout<<"GLFW Window Creation Failed! \n";
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size Information
    // Get Info of Area within the Window which will contain all our OpenGL data
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set Context for GLEW to use
    // Set window which will be used by OPenGl can be used in a case where we are using two window
    glfwMakeContextCurrent(mainWindow);

    // Allow Modern Extension Feature
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        std::cout<<"GLEW initialize failed! \n";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    // Setup ViewPort Size
    // Define the size to defined window we will be using 
    glViewport(0, 0, bufferWidth, bufferHeight);

    create_triangle();
    compile_shader();

    // window loop
    while(!glfwWindowShouldClose(mainWindow))     // WindowShouldClose allows us to use things like "X icon" to close the window
    {
        // Get + Handle user input events
        glfwPollEvents();

        if(direction)
            triOffset += triIncrement;
        else
            triOffset -= triIncrement;

        if(abs(triOffset) >= triMaxOffset)
            direction = !direction;
        
        // Clear Window
        glClearColor( 0.05f, 0.2f, 0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);       // openGL has more then color, which we need to clear;

        glUseProgram(shader);   // tell gpu to use this shader

        //Change Value of uniform Variable 
        glUniform1f(uniformXMove, triOffset);
        glUniform1f(uniformSize, 0.2);
        
        glBindVertexArray(vao);     // Bind Vertex Array 
        glDrawArrays(GL_TRIANGLES, 0, 3);       
        glBindVertexArray(0);       // UnBind Vertex Array 
        
        
        glfwSwapBuffers(mainWindow);    // We Draw to a scene we cant see, after drawing swap with display buffer 
    }
    return 0;
}