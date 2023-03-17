#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;

}

Mesh::~Mesh()
{
    ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned* indices, unsigned numOfVertices, unsigned numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO); // Amount of Array we want to create and gives us its ID  
    glBindVertexArray(VAO); // Binding our created ID, so now any openGL Buffer or Array will take palace in this ID

    glGenBuffers(1, &IBO);  //Create a index Buffer Object inside that VAO and give us its ID (for Indices or elements) 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);     
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &VBO); // Create a vertex Buffer Object inside that VAO and give us its ID 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW ); // Binding Data With Buffer and we cam also use GL_DYNAMIC_DRAW

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0])*5, 0);
    glEnableVertexAttribArray(0); // which element of Array we want to start form like above example will have 9/3 = 3  ==> { 0,1,2} arrays and start from 0
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0])*5, reinterpret_cast<void*>(sizeof(vertices[0]) * 3));   // for texture
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vertex Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   // unbind indices(element) buffer

    glBindVertexArray(0);   // unbind Array
}

void Mesh::RenderMesh()
{
    glBindVertexArray(VAO);     // Bind Vertex Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);       // UnBind Vertex Array
}

void Mesh::ClearMesh()
{
    if(IBO != 0)
    {
	    glDeleteBuffers(1, &IBO);
        IBO = 0;

    }
    if(VBO != 0)
    {
	    glDeleteBuffers(1, &VBO);
        VBO = 0;

    }
    if(VAO != 0)
    {
	    glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}
