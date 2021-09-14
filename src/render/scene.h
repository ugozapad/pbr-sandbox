#pragma once

#include "render/mesh.h"

class Scene
{
public:
	static Scene* create_from_file(const char* filename);
private:
	Scene(aiNode *node, const aiScene *scene);
	void process_node(aiNode *node, const aiScene *scene);
public:
	~Scene();
	void draw();

private:
	std::vector<Mesh*> m_meshes;

};