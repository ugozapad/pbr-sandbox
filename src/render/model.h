#pragma once

#include "render/material.h"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class VertexBuffer;
class IndexBuffer;

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

class Model
{
public:
	static Model* create_from_scene_node(aiMesh* mesh, const aiScene* scene);

private:
	Model(const MaterialCreationInfo& info, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

public:
	~Model();

	void render();

private:
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;

	Material m_mat;

	size_t m_vb_count, m_ib_count;

};