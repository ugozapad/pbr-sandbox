#ifndef RENDERINTERFACE_H
#define RENDERINTERFACE_H

#include <glm/glm.hpp>

struct SDL_Window;

// texture format
enum PixelFormat
{
	PF_UNKNOWN,
	PF_R8G8B8,
	PF_R8G8B8A8,
	PF_R8G8B8F,
	PF_R8G8B8A8F,

	// Depth formats
	PF_DEPTH32F
};

enum PrimitiveType
{
	PT_POINTS,
	PT_LINES,
	PT_TRIANGLES,
	PT_MAX
};

struct ShaderDescriptor
{
	const void* vertexData;
	const int vertexDataSize;
	const void* pixelData;
	const int pixelDataSize;

	ShaderDescriptor(const void* _vertexData,
		const int _vertexDataSize,
		const void* _pixelData,
		const int _pixelDataSize) :
		vertexData(_vertexData),
		vertexDataSize(_vertexDataSize),
		pixelData(_pixelData),
		pixelDataSize(_pixelDataSize)
	{
	}
};

struct SurfaceDescriptor
{
	int width;
	int height;
	PixelFormat format;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;
	virtual void UpdateData(const void* data, int size) = 0;
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;
	virtual void UpdateData(const void* data, int size) = 0;
};

class ShaderProgram
{
public:
	virtual ~ShaderProgram() = default;

	// Uniform API
	virtual uint32_t GetUniformLocation(const char* uniformName) = 0;
	virtual void SetTextureSampler(int slot, const char* name) = 0;
	virtual void SetInteger(int slot, int value) = 0;
	virtual void SetFloat(int slot, float value) = 0;
	virtual void SetVector3(int slot, const glm::vec3& vector) = 0;
	virtual void SetMatrix4(int slot, const glm::mat4& matrix) = 0;

};

class Texture2D
{
public:
	virtual ~Texture2D() = default;

	virtual void GenerateMipmaps() = 0;
	virtual PixelFormat GetPixelFormat() = 0;
};

class RenderInterface
{
public:
	virtual ~RenderInterface() = default;

	virtual void Init(SDL_Window* window) = 0;
	virtual void Shutdown() = 0;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void EnableVSync(bool value) = 0;
	virtual void Present() = 0;

	virtual Texture2D* CreateTexture2D(int width, int height, void* data, PixelFormat pixelFormat, bool generateMips = false) = 0;
	virtual void SetTexture2D(Texture2D* texture, int slot) = 0;

	virtual VertexBuffer* CreateVertexBuffer(const void* data, int size, int stride, bool dynamic) = 0;
	virtual IndexBuffer* CreateIndexBuffer(const void* data, int size, bool dynamic) = 0;

	virtual void SetVertexBuffer(VertexBuffer* buffer) = 0;
	virtual void SetIndexBuffer(IndexBuffer* buffer) = 0;

	virtual ShaderProgram* CreateShader(const ShaderDescriptor& shaderDesc) = 0;
	virtual void SetShader(ShaderProgram* shader) = 0;

	virtual void DrawArrays(PrimitiveType mode, int first, int count) = 0;
};

class RenderFacade
{
public:
	static RenderFacade* GetInstance();

public:
	RenderFacade();
	~RenderFacade();

	RenderInterface* GetRenderInterface();
};

#endif // !RENDERINTERFACE_H
