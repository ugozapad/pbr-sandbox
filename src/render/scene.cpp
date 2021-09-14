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
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene);
	}
}

Scene::~Scene()
{

}

void Scene::draw()
{

}

