#include <iostream>
#include <vector>


#include <glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Mesh.h"
#include "Shader.h"



std::vector<Shader> shaderList;
std::vector<Mesh*> meshList;

constexpr  GLint width = 1920, height =1080;
constexpr float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.8f;
float triIncrement = 0.0002f;
float curAngle = 0.0f;

static const char* vShader = "Shader/shader.vert";
static const char* fShader = "Shader/shader.frag";

// Vertex Shader
static const char* v_shader = R"(
    #version 460

    layout (location = 0 ) in vec3 pos;

    out vec4 vCol;

    uniform mat4 model; 
    uniform mat4 projection; 
    
    void main()
    {
        gl_Position =  projection * model * vec4(pos, 1.0);
        vCol = vec4(clamp(pos,0.0f,1.0f),1.0f);
    })";

// Fragment Shader
static const char* f_shader = R"(
    #version 460
    
    in vec4 vCol;

    out vec4 colour;
    
    void main()
    {
        colour = vCol;
    })";

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2 
    };
    GLfloat vertices[] = {
       -1.0f, -1.0f, 0.0f,
       0.0f, -1.0f, 1.0f,
       1.0f, -1.0f, 0.0f,
       0.0f, 1.0f, 0.0f
    };

    Mesh *obj1 = new Mesh;
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

	Mesh *obj2 = new Mesh;
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader  *shader1 = new Shader();
    //shader1->CreateFromString(v_shader, f_shader);
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void update()
{
    if(direction)
        triOffset += triIncrement;
    else
        triOffset -= triIncrement;

    if(abs(triOffset) >= triMaxOffset)
        direction = !direction;

    curAngle+= 0.01f;
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

    glEnable(GL_DEPTH_TEST); // so that knows which vertex in front and which is on back 
    
    // Setup ViewPort Size
    // Define the size to defined window we will be using 
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateObjects();
    CreateShaders();

    unsigned int uniformProjection = 0, uniformModel =0;

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat) bufferWidth/(GLfloat) bufferHeight , 0.1f, 100.0f);
    
    // window loop
    while(!glfwWindowShouldClose(mainWindow))     // WindowShouldClose allows us to use things like "X icon" to close the window
    {
        // Get + Handle user input events
        glfwPollEvents();

        // Clear Window
        glClearColor( 0.05f, 0.2f, 0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // openGL has more then color, which we need to clear;
        
        shaderList[0].UseShader();   // tell gpu to use this shader

        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();

        update();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
        model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.5f,1.0f));
        //Change Value of uniform Variable
    
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3( triOffset, 1.0f, -2.5f));
        model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.5f,1.0f));
        //Change Value of uniform Variable
    
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));


        meshList[0]->RenderMesh();

        glUseProgram(0);
        
        glfwSwapBuffers(mainWindow);    // We Draw to a scene we cant see, after drawing swap with display buffer 
    }
    return 0;
}