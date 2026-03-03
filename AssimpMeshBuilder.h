#pragma once
#include <assimp/scene.h>
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

[[nodiscard]] std::unique_ptr<Mesh> loadModel(const std::string& path);

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices, unsigned int& indicesOffset, const glm::mat4& parentTransform);
glm::mat4 assimpToGLM(aiMatrix4x4& a);

