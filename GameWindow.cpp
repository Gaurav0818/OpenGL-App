#include "GameWindow.h"

GameWindow::GameWindow()
{
	width = 800;
	height = 600;
}

GameWindow::~GameWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

GameWindow::GameWindow(unsigned windowWidth, unsigned windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int GameWindow::Initialize()
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
    
    mainWindow = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr);

    if(!mainWindow)
    {
        std::cout<<"GLFW Window Creation Failed! \n";
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size Information
    // Get Info of Area within the Window which will contain all our OpenGL data
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
}
