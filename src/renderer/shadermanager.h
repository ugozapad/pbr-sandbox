#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

class ShaderManager
{
public:
	static ShaderProgram* Create(const char* vertexFilename, const char* pixelFilename);

};


#endif // !SHADERMANAGER_H
