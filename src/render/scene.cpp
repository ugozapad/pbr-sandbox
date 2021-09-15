#include "render/scene.h"

#include <spdlog/spdlog.h>

Scene* Scene::create_from_file(const char* filename)
{
	Assimp::Importer imported;
	const aiScene* scene = imported.ReadFile(filename, aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		spdlog::error("scene: failed to load scene {}", imported.GetErrorString());
		return nullptr;
	}

	return new Scene(scene->mRootNode, scene);
}

Scene::Scene(aiNode *node, const aiScene *scene)
{
	process_node(node, scene);
}

void Scene::process_node(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(Mesh::create_from_scene_node(mesh, node, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Scene::~Scene()
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		if (*it) {
			delete *it;
			*it = nullptr;
		}
	}
}

void Scene::draw()
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		(*it)->render();
	}
}

