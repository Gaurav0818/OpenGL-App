#pragma once

#include <vector>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	// Load the model from a file
	void LoadModel(const std::string& fileName);

	// Render the model
	void RenderModel();

	// Clear the model data
	void ClearModel();

	~Model();

private:
	// Helper functions for loading the model
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

	std::vector<Mesh*> meshList;             // List of meshes in the model
	std::vector<Texture*> textureList;       // List of textures used by the model
	std::vector<unsigned int> meshToTex;     // Mapping of mesh index to texture index
};