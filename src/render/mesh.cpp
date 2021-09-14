#include "render/mesh.h"
#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"

Mesh* Mesh::create_from_scene_node(aiMesh* mesh, const aiScene* scene)
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

	aiString albedo_name;
	if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &albedo_name) != aiReturn_SUCCESS)
		material->GetTexture(aiTextureType_DIFFUSE, 0, &albedo_name);

	material_info.m_albedo_filename = strdup(albedo_name.C_Str());

	aiString normal_name;
	material->GetTexture(aiTextureType_NORMALS, 0, &normal_name);
	material_info.m_normal_filename = strdup(normal_name.C_Str());

	return new Mesh(material_info, vertices, indices);
}

Mesh::Mesh(const MaterialCreationInfo& info, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_vb_count = vertices.size();
	m_vb = RenderDevice::get_instance()->create_vertex_buffer((void*)vertices.data(), vertices.size() * sizeof(Vertex), BufferAccess::Static);

	m_ib_count = indices.size();
	m_ib = RenderDevice::get_instance()->create_index_buffer((void*)indices.data(), indices.size(), BufferAccess::Static);

	m_mat.init(info);
}

Mesh::~Mesh()
{
	m_mat.release();
	RenderDevice::get_instance()->delete_index_buffer(m_ib);
	RenderDevice::get_instance()->delete_vertex_buffer(m_vb);
}

void Mesh::render()
{

}
