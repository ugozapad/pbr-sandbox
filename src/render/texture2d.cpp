#include "render/texture2d.h"

#include "glad/glad.h"

#include <stdlib.h>
#include <math.h>
#include <limits>
#include <algorithm>

uint32_t get_gl_format(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::FMT_RGB:
		return GL_RGB;
	case ImageFormat::FMT_RGBA:
		return GL_RGBA;
	case ImageFormat::FMT_DEPTH24_STENCIL8:
		return GL_DEPTH24_STENCIL8;
	case ImageFormat::FMT_DEPTH32_STENCIL_8_F:
		return GL_DEPTH32F_STENCIL8;
	case ImageFormat::FMT_SRGB:
		return GL_SRGB;
	case ImageFormat::FMT_RGB32F:
		return GL_RGB32F;
	case ImageFormat::FMT_RGBA32F:
		return GL_RGBA32F;
	case ImageFormat::FMT_RGB16F:
		return GL_RGB16F;
	case ImageFormat::FMT_RGBA16F:
		return GL_RGBA16F;
	default:
		break;
	}

	return 0;
}

void Texture2D::init_from_memory(void* data, int width, int height, ImageFormat format)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_handle);

	GLenum internal_format = 0;
	if (format == ImageFormat::FMT_RGB)
		internal_format = GL_RGB8;
	else if (format == ImageFormat::FMT_RGBA)
		internal_format = GL_RGBA8;

	int mipmap_levels = (GLsizei)floor(log2(std::max(width, height)));
	if (mipmap_levels == 0)
		mipmap_levels = 1;

	glTextureStorage2D(m_texture_handle, mipmap_levels, internal_format, width, height);
	glTextureSubImage2D(m_texture_handle, 0, 0, 0, width, height, get_gl_format(format), GL_UNSIGNED_BYTE, data);

	if (mipmap_levels > 1)
		glGenerateTextureMipmap(m_texture_handle);

	glTextureParameterf(m_texture_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameterf(m_texture_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameterf(m_texture_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (mipmap_levels > 1)
		glTextureParameterf(m_texture_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTextureParameterf(m_texture_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#define ENABLE_TEST_ANISOTROPIC_FILTERING

#ifdef ENABLE_TEST_ANISOTROPIC_FILTERING

	glTextureParameterf(m_texture_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(m_texture_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameterf(m_texture_handle, GL_TEXTURE_MAX_ANISOTROPY, 16);
#endif // ENABLE_TEST_ANISOTROPIC_FILTERING

}

void Texture2D::bind(int slot)
{
	glBindTextureUnit(slot, m_texture_handle);
}

