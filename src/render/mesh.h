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
	static Mesh* create_from_scene_node(aiMesh* mesh, aiNode* node, const aiScene* scene);

private:
	Mesh(const MaterialCreationInfo& info, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::mat4& mat);

public:
	~Mesh();

	void render();

private:
	void update_internal_matrices_and_vectors();

private:
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;

	glm::mat4 m_model_matr;
	glm::vec3 m_pos;
	glm::quat m_rot;
	glm::vec3 m_scale;
	Material m_mat;

	size_t m_vb_count, m_ib_count;
};