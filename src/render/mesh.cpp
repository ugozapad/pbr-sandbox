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

Mesh* Mesh::createFromSceneNode(aiMesh* mesh, aiNode* node, const aiScene* scene)
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

	aiString ai_normal_name;
	material->GetTexture(aiTextureType_NORMALS, 0, &ai_normal_name);

	std::string normalmap_name = ai_normal_name.C_Str();
	if (!normalmap_name.empty())
	{
		normalmap_name.clear();
		normalmap_name = "data/sponza/glTF/";
		normalmap_name += ai_normal_name.C_Str();

		material_info.m_normal_filename = strdup(normalmap_name.c_str());
	}

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
	m_vertex_buffer = RenderDevice::getInstance()->createVertexBuffer((void*)vertices.data(), vertices.size() * sizeof(Vertex), BufferAccess::Static);

	m_ib_count = indices.size();
	m_index_buffer = RenderDevice::getInstance()->createIndexBuffer((void*)indices.data(), indices.size() * sizeof(uint32_t), BufferAccess::Static);

	m_material.init(info);

	m_transform = mat;

	updateInternalMatricesAndVectors();
}

Mesh::~Mesh()
{
	m_material.release();
	RenderDevice::getInstance()->deleteIndexBuffer(m_index_buffer);
	RenderDevice::getInstance()->deleteVertexBuffer(m_vertex_buffer);
}

void Mesh::render()
{
	RenderDevice::getInstance()->setVertexBuffer(m_vertex_buffer);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	RenderDevice::getInstance()->setIndexBuffer(m_index_buffer);

	m_material.render(m_ib_count, m_transform);
}

void Mesh::updateInternalMatricesAndVectors()
{
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_transform, m_scale, m_rot, m_pos, skew, perspective);
}
