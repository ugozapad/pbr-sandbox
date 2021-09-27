#include "render/scene.h"

#include <spdlog/spdlog.h>

Scene* Scene::createFromFile(const char* filename)
{
	Assimp::Importer imported;
	const aiScene* scene = imported.ReadFile(filename,
		aiProcessPreset_TargetRealtime_Quality |                     // some optimizations and safety checks
		aiProcess_OptimizeMeshes |                                   // minimize number of meshes
		aiProcess_PreTransformVertices |                             // apply node matrices
		aiProcess_FixInfacingNormals | aiProcess_TransformUVCoords | // apply UV transformations
		aiProcess_FlipUVs
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		spdlog::error("scene: failed to load scene {}", imported.GetErrorString());
		return nullptr;
	}

	return new Scene(scene->mRootNode, scene);
}

Scene::Scene(aiNode *node, const aiScene *scene)
{
	processNode(node, scene);
}

void Scene::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(Mesh::createFromSceneNode(mesh, node, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
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

