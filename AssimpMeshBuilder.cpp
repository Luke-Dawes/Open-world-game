#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"
#include "AssimpMeshBuilder.h"
#include <memory>
#include <filesystem>

void debugMeshColorsAndTextures(const aiScene* scene) {
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) { //goes through each mesh
		const aiMesh* mesh = scene->mMeshes[i]; //sets the mesh
		const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; //gets the mesh material

		std::cout << "Mesh " << i
			<< " (" << mesh->mName.C_Str() << "):\n"; //prints the mesh's name

		// prints if it has colours 
		if (mesh->HasVertexColors(0)) {
			std::cout << "has vertex colours.\n";
		}
		else {
			std::cout << "no vertex colours.\n";
		}

		// check for textures
		unsigned int textureCount = material->GetTextureCount(aiTextureType_BASE_COLOR); //gets the amount of textures
		if (textureCount > 0) {
			std::cout << "has textures (" << textureCount << ")\n";
		}
		else {
			std::cout << "no texture.\n";
		}

		// checks for colours which are in their but arent vertex colours
		aiColor4D baseColour(1.f, 1.f, 1.f, 1.f);
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColour)) { //if there is a colour
			std::cout << "base colur: "
				<< baseColour.r << ", "
				<< baseColour.g << ", "
				<< baseColour.b << ", "
				<< baseColour.a << "\n";
		} //prints the red green blue alpha values

		std::cout << "\n";
	}
}


std::unique_ptr<Mesh> loadModel(const std::string& path) {

/*
* loadModel function. It loads the verticies and indicies into a mesh class.
* 
* parameters
* string& path in - it takes in the path to the GLB file, where the model is located
* 
* return
* It returns a fully made mesh which is unique and its lifetime is tied to whatever its set to once returned
* 
* bugs
* no textures
* 
*/


	Assimp::Importer importer; //creates a importer object

	std::cout << "\nabsolute path is " << std::filesystem::absolute(path); //prints the path of the file 

	if (!std::filesystem::exists(path)) {
		std::cout << "\nFile not found " << path;
	}
	
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices); 

	if (!scene) {
		std::cout << "\nscene is empty: " << importer.GetErrorString();
		throw std::runtime_error("scene isnt a thing");
	}

	//debugMeshColorsAndTextures(scene);


	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int indicesOffset = 0;
	
	processNode(scene->mRootNode, scene, vertices, indices, indicesOffset, glm::mat4(1.f));

	return std::make_unique<Mesh>(vertices, indices);
}

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned int& indicesOffset, const glm::mat4& parentTransform) {
	/*
	*ProcessNode function. Its a recursive function which goes through all the different meshes in the glb file and combines them into one.
	* 
	* parameters
	* aiNde* node - the class which holds all the infomation about the mesh, e.g. all the meshes which are connected to the inital mesh, so you
	* process them in the right order
	* const aiScene* scene - scene is the first node which is used, which is never changed meaning it can be const
	* std::vector<Mesh::vertex>& verticies - a vector which holds all the verticies and can be changed as its passed by refrence.
	* std::vector<usinged int>& indicies - a vector which holds all the indicies which it needs to contruct a mesh
	* unsigned int& indicesOffset - a offset applied to indicies based on verticies size
	* const glm::mat4& parentTransforms - as transforms stack it applies the transform from the parent node to the child node
	* 
	* returns nothing 
	* 
	*/
	glm::mat4 nodeTransform = assimpToGLM(node->mTransformation);

	glm::mat4 GlobalTransform = parentTransform * nodeTransform;

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			Mesh::Vertex vertex;
			glm::vec4 pos(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.f);

			pos = GlobalTransform * pos;

			vertex.Position = glm::vec3(pos);

			const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (mesh->HasVertexColors(0)) {
				vertex.Colour = glm::vec4(mesh->mColors[0][v].r, ///doesnt work
					mesh->mColors[0][v].g,
					mesh->mColors[0][v].b,
					mesh->mColors[0][v].a);
			}
			else { 

				const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; //get the material 

				aiColor4D matColour(1.f, 1.f, 1.f, 1.f); //set a white background
				if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, matColour)) { //if there is colours it diffuses them into matColour
					vertex.Colour = glm::vec4(matColour.r, matColour.g, matColour.b, matColour.a); //set the colours instead
				}
			
			}

			vertices.push_back(vertex);
		}


		for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
			auto face = mesh->mFaces[f];

			for (unsigned int j = 0; j < face.mNumIndices; ++j) {
				indices.push_back(face.mIndices[j] + indicesOffset);
			}
		}
		indicesOffset += mesh->mNumVertices;

	}

	for (unsigned int c = 0; c < node->mNumChildren; c++) {
		processNode(node->mChildren[c], scene, vertices, indices, indicesOffset, GlobalTransform);	
	}

}

glm::mat4 assimpToGLM(aiMatrix4x4& a) { ///this is horribly wrong - but when i try and fix it only gets worse
	/*
	* assimpToGLM - transforms a assimp 4x4 matrix to a GLM 4x4 matrix. it does this by manually mapping the different points of the assimp matrix
	* to the glm matrix.
	*/
	return glm::mat4(
		a.a1, a.b1, a.c1, a.d1,
		a.b1, a.b2, a.c2, a.d2,
		a.c1, a.b3, a.c3, a.d3,
		a.d1, a.b4, a.c4, a.d4
	);

}
