﻿#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

// Textures
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

// Lights
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Materials
Material shinyMaterial;
Material dullMaterial;


//Model blackHawk;


GLfloat deltaTime = 0.0f; // Change in time.
GLfloat lastTime = 0.0f; // What the last time was.

// Vertex Shader
static const char* vShader = "src/Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "src/Shaders/shader.frag";

void CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, 
	GLfloat * vertices, unsigned int verticeCount, unsigned int vertexLength, unsigned int normalOffset);

void CreateObjects() 
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// U(S) and V(T) values are the UV values, i.e. coordinate of the textures.
	// Norm values are values of the normal to that vertex/
	GLfloat vertices[] = {
	//	X		Y	   Z	U(S)  V(T)		NormX NormY NormZ
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,  0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,  1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,   0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	// Indices for the floor of the world.
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	// Floor of the world.
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, // Top left.
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f, // Top right. We are TILING the texture, so we want it to appear 10 times on the surface.
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f, // Bottom left.
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f // Bottom right.
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* floor = new Mesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floor);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() 
{

	mainWindow = Window(1366, 768); // Standard widescreen.
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	// Creating textures
	brickTexture = Texture("src/Textures/brick.png");
	brickTexture.LoadTexture();

	dirtTexture = Texture("src/Textures/dirt.png");
	dirtTexture.LoadTexture();

	plainTexture = Texture("src/Textures/plain.png");
	plainTexture.LoadTexture();

	Model xWing = Model();
	xWing.LoadModel("src/Models/uh60.obj");

	// Creating lights
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								 0.2f, 0.4f,
								 2.0f, -1.0f, -2.0f);
	
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.0f, 0.0f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0]= SpotLight(	0.0f, 0.0f, 1.0f,
								0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f,
								0.3f, 0.2f, 0.1f,
								20.0f);

	spotLightCount++;
	// Creating Materials

	// A shine is usually a form of 2, a power of 2. 32 is common for the average shiny object.
	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, 
		uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	lastTime = glfwGetTime(); // Initializing the time.

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // Gets the current time (seconds, system time)
		deltaTime = now - lastTime; // calculating the time difference between last time in the loop and now.
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		// Handle camera control with keys
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		// Handle camera control with mouse
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); // USes the camera to get our view matrix.
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f);
		 
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		////model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//brickTexture.UseTexture();
		//shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[0]->RenderMesh();

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		////model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dirtTexture.UseTexture();
		//dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[1]->RenderMesh();
//
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, -2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f) / 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		xWing.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vertexLength, unsigned int normalOffset)
{
	// Jumping from triangle to triangle, in the indices table.
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		// vertexLength represents how many values there are in a vertex. In our case, it's 8.
		// By doing this multiplication, we get straigth to the x,y,z values of a vertex in the vertices[] array.
		unsigned int in0 = indices[i] * vertexLength;
		unsigned int in1 = indices[i + 1] * vertexLength;
		unsigned int in2 = indices[i + 2] * vertexLength;

		// So our above code points to the start of each of the 3 vertex value used in a face.

		// Now, for each vertex, we create 2 lines leading from it, and we do the cross product. This will give us the perfect normal for each vertex!
		// Creating first line
		glm::vec3 v1(vertices[in1] - vertices[in0], // Subtracting x values
			vertices[in1 + 1] - vertices[in0 + 1], // Subtracting y values
			vertices[in1 + 2] - vertices[in0 + 2] // Subtracting z values
			);
		glm::vec3 v2(vertices[in2] - vertices[in0], // Subtracting x values
			vertices[in2 + 1] - vertices[in0 + 1], // Subtracting y values
			vertices[in2 + 2] - vertices[in0 + 2] // Subtracting z values
		);
		glm::vec3 normal = glm::cross(v1, v2); // Getting the normal
		normal = glm::normalize(normal); // Normalizing because we just want the direction.

		// Going to the normal values
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		// Everytime a vertex is used in one of those normal, we add its X value. Because we are computing the average of all the normals we come up with.
		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vertexLength; i++)
	{
		// Normal offset for this individual vertex
		unsigned int nOffset = i * vertexLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		// Normalizing the normal vector.
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}
