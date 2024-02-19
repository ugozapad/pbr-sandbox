#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include "texture2d_gl.h"

// pixel format convertion
uint32_t getGLPF(PixelFormat pf);
uint32_t getGLInternalPF(PixelFormat pf);
uint32_t getGLTypePF(PixelFormat pf);

Texture2D_GL::Texture2D_GL()
{
	m_pf = PF_UNKNOWN;
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_handle = -1;
}

Texture2D_GL::~Texture2D_GL()
{
	if (m_handle >= 0)
		Destroy();

	m_pf = PF_UNKNOWN;
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_handle = -1;
}
//
//void Texture2D_GL::createBlackTexture(int width, int height, int channels)
//{
//	size_t textureSize = width * height * channels;
//	uint8_t* data = new uint8_t[textureSize];
//	assert(data);
//
//	for (int i = 0; i < textureSize; i++) {
//		data[i] = 0;
//	}
//
//	createFromExistedData(data, width, height, channels);
//
//	delete[] data;
//}
//
//void Texture2D::createWhiteTexture(int width, int height, int channels)
//{
//	size_t textureSize = width * height * channels;
//	uint8_t* data = new uint8_t[textureSize];
//	assert(data);
//
//	for (int i = 0; i < textureSize; i++) {
//		data[i] = 255;
//	}
//
//	createFromExistedData(data, width, height, channels);
//
//	delete[] data;
//}
//
//void Texture2D::createGrayTexture(int width, int height, int channels)
//{
//	size_t textureSize = width * height * channels;
//	uint8_t* data = new uint8_t[textureSize];
//	assert(data);
//
//	for (int i = 0; i < textureSize; i++) {
//		data[i] = 255 / 2;
//	}
//
//	createFromExistedData(data, width, height, channels);
//
//	delete[] data;
//}
//
//void Texture2D::createTexture_Generator(int width, int height, int channels, int color)
//{
//	size_t textureSize = width * height * channels;
//	uint8_t* data = new uint8_t[textureSize];
//	assert(data);
//
//	m_textureFileName = "$generator_texture$";
//
//	for (int i = 0; i < textureSize; i++) {
//		data[i] = color;
//	}
//
//	createFromExistedData(data, width, height, channels);
//
//	delete[] data;
//}
//
//void Texture2D::createFromExistedData(void* data, int width, int height, int channels)
//{
//	//assert(data);
//
//	m_width = width;
//	m_height = height;
//	m_channels = channels;
//
//	glGenTextures(1, &m_handle);
//	glBindTexture(GL_TEXTURE_2D, m_handle);
//	glTexImage2D(GL_TEXTURE_2D, 0, (channels == 3) ? GL_RGB : GL_RGBA, width, height, 0, (channels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

void Texture2D_GL::CreateRaw(void* data, int width, int height, PixelFormat pf)
{
	m_width = width;
	m_height = height;
	m_channels =  (pf == PF_R8G8B8) ? 3 : 4;
	m_pf = pf;

	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalPF(pf), width, height, 0, getGLInternalPF(pf), GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D_GL::Destroy()
{
	glDeleteTextures(1, &m_handle);
}

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

void Texture2D_GL::GenerateMipmaps()
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D_GL::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

//void Texture2D::setWrapS(TextureWrap wrap)
//{
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGlWrap(wrap));
//}
//
//void Texture2D::setWrapT(TextureWrap wrap)
//{
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGlWrap(wrap));
//}
//
//void Texture2D::setMin(TextureFilter filter)
//{
//	GLint param = 0;
//	param = getGlTexFilter(filter);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
//}
//
//void Texture2D::setMag(TextureFilter filter)
//{
//	GLint param = 0;
//	param = getGlTexFilter(filter);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
//}
//
//GLint getGlWrap(TextureWrap wrap)
//{
//	GLint param = 0;
//
//	if (wrap == TextureWrap::Repeat)
//		param = GL_REPEAT;
//	else if (wrap == TextureWrap::MirroredRepeat)
//		param = GL_MIRRORED_REPEAT;
//	else if (wrap == TextureWrap::ClampToEdge)
//		param = GL_CLAMP_TO_EDGE;
//	else if (wrap == TextureWrap::ClampToBorder)
//		param = GL_CLAMP_TO_BORDER;
//
//	return param;
//}
//
//GLint getGlTexFilter(TextureFilter filter)
//{
//	GLint param = 0;
//
//	if (filter == TextureFilter::Linear)
//		param = GL_LINEAR;
//	else if (filter == TextureFilter::Nearest)
//		param = GL_NEAREST;
//	else if (filter == TextureFilter::LinearMipmapLinear)
//		param = GL_LINEAR_MIPMAP_LINEAR;
//	else if (filter == TextureFilter::LinearMipmapNearest)
//		param = GL_LINEAR_MIPMAP_NEAREST;
//	else if (filter == TextureFilter::NearestMipmapLinear)
//		param = GL_NEAREST_MIPMAP_LINEAR;
//	else if (filter == TextureFilter::NearestMipmapNearest)
//		param = GL_NEAREST_MIPMAP_NEAREST;
//
//	return param;
//}

uint32_t getGLPF(PixelFormat pf)
{
	return 0;
	//return uint32_t();
}

// Kirill: Remove to render_main.cpp or something else
uint32_t getGLInternalPF(PixelFormat pf)
{
	switch (pf)
	{
	case PF_UNKNOWN:
		return 0;

	case PF_R8G8B8:
	case PF_R8G8B8F:
		return GL_RGB;

	case PF_R8G8B8A8:
	case PF_R8G8B8A8F:
		return GL_RGBA;
	}

	return 0;
}

uint32_t getGLTypePF(PixelFormat pf)
{
	switch (pf)
	{
	case PF_UNKNOWN:
		return 0;

	case PF_R8G8B8:
	case PF_R8G8B8A8:
		return GL_UNSIGNED_BYTE;

	case PF_R8G8B8F:
	case PF_R8G8B8A8F:
		return GL_FLOAT;
	}

	return 0;
}

//uint32_t getGLBlendFactor(BlendFactor factor)
//{
//	switch (factor)
//	{
//	case BF_ZERO:
//		return GL_ZERO;
//		break;
//	case BF_ONE:
//		return GL_ONE;
//		break;
//	case BF_SRC_COLOR:
//		return GL_SRC_COLOR;
//		break;
//	case BF_ONE_MINUS_SRC_COLOR:
//		return GL_ONE_MINUS_SRC_COLOR;
//		break;
//	case BF_DST_COLOR:
//		return GL_DST_COLOR;
//		break;
//	case BF_ONE_MINUS_DST_COLOR:
//		return GL_ONE_MINUS_DST_COLOR;
//		break;
//	case BF_SRC_ALPHA:
//		return GL_SRC_ALPHA;
//		break;
//	case BF_ONE_MINUS_SRC_ALPHA:
//		return GL_ONE_MINUS_SRC_ALPHA;
//		break;
//	case BF_DST_ALPHA:
//		return GL_DST_ALPHA;
//		break;
//	case BF_ONE_MINUS_DST_ALPHA:
//		return GL_ONE_MINUS_DST_ALPHA;
//		break;
//	case BF_CONSTANT_COLOR:
//		return GL_CONSTANT_COLOR;
//		break;
//	case BF_ONE_MINUS_CONSTANT_COLOR:
//		return GL_ONE_MINUS_CONSTANT_COLOR;
//		break;
//	case BF_CONSTANT_ALPHA:
//		return GL_CONSTANT_ALPHA;
//		break;
//	case BF_ONE_MINUS_CONSTANT_ALPHA:
//		return GL_ONE_MINUS_CONSTANT_ALPHA;
//		break;
//	default:
//		break;
//	}
//
//	return 0;
//}
