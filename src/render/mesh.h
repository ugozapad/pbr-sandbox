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

class Mesh
{
public:
	static Mesh* createFromSceneNode(aiMesh* mesh, aiNode* node, const aiScene* scene);

private:
	Mesh(const MaterialCreationInfo& info, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::mat4& mat);

public:
	~Mesh();

	void render();

private:
	void updateInternalMatricesAndVectors();

private:
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;

	glm::mat4 m_transform;
	glm::vec3 m_pos;
	glm::quat m_rot;
	glm::vec3 m_scale;
	Material m_material;

	size_t m_vb_count, m_ib_count;
};