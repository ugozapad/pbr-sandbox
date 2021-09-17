#include "render/mesh.h"
#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"

#include <string>

#include <glm/gtx/matrix_decompose.hpp>

#include "glad/glad.h"

inline static glm::mat4 ai_to_gl(const aiMatrix4x4& from)
{
	return glm::mat4(
		(double)from.a1, (double)from.b1, (double)from.c1, (double)from.d1,
		(double)from.a2, (double)from.b2, (double)from.c2, (double)from.d2,
		(double)from.a3, (double)from.b3, (double)from.c3, (double)from.d3,
		(double)from.a4, (double)from.b4, (double)from.c4, (double)from.d4
	);
}

Mesh* Mesh::create_from_scene_node(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if (mesh->HasNormals()) {
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		//if (mesh->HasTextureCoords(0))
		if (mesh->mTextureCoords[0]) {
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		} else {
			vertex.texcoord = glm::vec2(0.0f, 0.0f);
		}
		
		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	MaterialCreationInfo material_info;
	material_info.init();

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	aiString ai_albedo_name;
	if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &ai_albedo_name) != aiReturn_SUCCESS)
		material->GetTexture(aiTextureType_DIFFUSE, 0, &ai_albedo_name);

	std::string albedo_name = "data/sponza/glTF/";
	albedo_name += ai_albedo_name.C_Str();

	material_info.m_albedo_filename = strdup(albedo_name.c_str());

	aiString normal_name;
	material->GetTexture(aiTextureType_NORMALS, 0, &normal_name);
	material_info.m_normal_filename = strdup(normal_name.C_Str());

	glm::mat4 matr = glm::mat4(1.0f);
	matr = ai_to_gl(node->mTransformation);
	//matr = glm::scale(matr, glm::vec3(0.1f));

	return new Mesh(material_info, vertices, indices, matr);
}

Mesh::Mesh(const MaterialCreationInfo& info, 
	const std::vector<Vertex>& vertices, 
	const std::vector<uint32_t>& indices, 
	const glm::mat4& mat)
{
	m_vb_count = vertices.size();
	m_vb = RenderDevice::get_instance()->create_vertex_buffer((void*)vertices.data(), vertices.size() * sizeof(Vertex), BufferAccess::Static);

	m_ib_count = indices.size();
	m_ib = RenderDevice::get_instance()->create_index_buffer((void*)indices.data(), indices.size() * sizeof(uint32_t), BufferAccess::Static);

	m_mat.init(info);

	m_model_matr = mat;

	update_internal_matrices_and_vectors();
}

Mesh::~Mesh()
{
	m_mat.release();
	RenderDevice::get_instance()->delete_index_buffer(m_ib);
	RenderDevice::get_instance()->delete_vertex_buffer(m_vb);
}

void Mesh::render()
{
	RenderDevice::get_instance()->set_vertex_buffer(m_vb);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	RenderDevice::get_instance()->set_index_buffer(m_ib);

	m_mat.render(m_ib_count, m_model_matr);
}

void Mesh::update_internal_matrices_and_vectors()
{
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_model_matr, m_scale, m_rot, m_pos, skew, perspective);
}
