#pragma once

#include "render/mesh.h"
#include "render/light.h"

class ConstantBuffer;

class Scene
{
public:
	static Scene* createFromFile(const char* filename);
private:
	Scene(aiNode *node, const aiScene *scene);
	void processNode(aiNode *node, const aiScene *scene);
	void initSceneLights();
public:
	~Scene();
	void draw();

private:
	std::vector<Mesh*> m_meshes;
	std::vector<PointLight> m_pointLights;

	ConstantBuffer* m_lightCb;
};