#pragma once

#include <stdio.h>
#include <iostream>

#include <glew.h>
#include <GLFW/glfw3.h>

class GameWindow
{
public:
	GameWindow();
	~GameWindow();

	GameWindow(unsigned int windowWidth, unsigned int windowHeight);

	int Initialize();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeigth() { return bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }  // We Draw to a scene we cant see, after drawing swap with display buffer 

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

};

