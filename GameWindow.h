#pragma once

#include <stdio.h>
#include <iostream>

#include <glew.h>
#include <GLFW/glfw3.h>

class GameWindow
{
public:
	GameWindow();

	GameWindow(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~GameWindow();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

};

