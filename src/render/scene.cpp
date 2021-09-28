#include "render/scene.h"

#include "render/constantbuffer.h"
#include "render/renderdevice.h"

#include <spdlog/spdlog.h>

struct PointLightsConstantBuffer
{
	glm::vec3 position;
	glm::vec3 color;

	void init()
	{
		position = glm::vec3(0.0f);
		color = glm::vec3(1.0f);
	}
};

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
	initSceneLights();
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

void Scene::initSceneLights()
{
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	for (int i = 0; i < 4; i++) {
		PointLight pointLight;
		pointLight.m_position = pointLightPositions[i];
		
		m_pointLights.push_back(pointLight);
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

