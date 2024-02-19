#ifndef RENDERINTERFACE_H
#define RENDERINTERFACE_H

struct SDL_Window;

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
