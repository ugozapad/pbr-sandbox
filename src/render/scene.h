#pragma once

#include "render/mesh.h"

class Scene
{
public:
	static Scene* createFromFile(const char* filename);
private:
	Scene(aiNode *node, const aiScene *scene);
	void processNode(aiNode *node, const aiScene *scene);
public:
	~Scene();
	void draw();

private:
	std::vector<Mesh*> m_meshes;

};