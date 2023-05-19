#include "Model.h"

Model::Model()
{
}

void Model::RenderModel()
{
	// Iterate over all meshes in the model.
	for (size_t i = 0; i < meshList.size(); i++)
	{
		// Get the material index for the current mesh.
		unsigned int materialIndex = meshToTex[i];
		// Check if the material index is valid and if the texture exists.
		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			// Bind the texture for the current material.
			textureList[materialIndex]->UseTexture();
		}
		// Render the current mesh.
		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	// Use the importer to read the model file and process it with various post-processing options
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate |
															aiProcess_FlipUVs |
															aiProcess_GenSmoothNormals |
															aiProcess_JoinIdenticalVertices);

	// Check if the scene was successfully loaded
	if (!scene)
	{
		// If the scene failed to load, print an error message with the file name and the importer's error string
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	// Call the LoadNode() function to recursively load the nodes of the scene starting from the root node
	LoadNode(scene->mRootNode, scene);

	// Call the LoadMaterials() function to load the materials used by the model
	LoadMaterials(scene);
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	// Iterate over the meshes in the current node
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		// Call LoadMesh() to load the mesh data and create a new Mesh object
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	// Iterate over the child nodes of the current node
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		// Recursively call LoadNode() for each child node
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{
	// Create vectors to store the vertex data and indices for the mesh
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	// Iterate over the vertices of the mesh
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Add vertex position data to the vertices vector
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,
													mesh->mVertices[i].y,
													mesh->mVertices[i].z });

		// Check if the mesh has texture coordinates
		if (mesh->mTextureCoords[0])
		{
			// Add texture coordinate data to the vertices vector
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,
														mesh->mTextureCoords[0][i].y });
		}
		else {
			// If the mesh does not have texture coordinates, add default (0, 0) coordinates
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		// Add vertex normal data (negated) to the vertices vector
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,
													-mesh->mNormals[i].y,
													-mesh->mNormals[i].z });
	}

	// Iterate over the faces of the mesh
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		// Get the current face
		aiFace face = mesh->mFaces[i];

		// Iterate over the indices of the current face and add them to the indices vector
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Create a new Mesh object and initialize it with the vertex and index data
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());

	// Add the new Mesh object to the meshList vector of the Model
	meshList.push_back(newMesh);

	// Store the material index for the mesh in the meshToTex vector of the Model
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	// Resize the textureList vector to accommodate the number of materials in the scene
	textureList.resize(scene->mNumMaterials);

	// Iterate over the materials in the scene
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		// Get the current material
		aiMaterial* material = scene->mMaterials[i];

		// Initialize the texture at the current index to nullptr
		textureList[i] = nullptr;

		// Check if the material has a diffuse texture
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			// Get the path of the first diffuse texture
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Extract the filename from the path
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				// Create the texture path by combining the filename with the "Textures/" directory
				std::string texPath = std::string("Textures/") + filename;

				// Create a new Texture object and assign it to the textureList at the current index
				textureList[i] = new Texture(texPath.c_str());

				// Load the texture
				if (!textureList[i]->LoadTexture())
				{
					// If the texture fails to load, print an error message and clean up
					printf("Failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		// If the texture was not loaded or does not exist, assign a default texture
		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureWithAlpha();
		}
	}
}


void Model::ClearModel()
{
	// Iterate over the meshes in the meshList vector and clean up memory
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	// Iterate over the textures in the textureList vector and clean up memory
	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
	ClearModel();
}
