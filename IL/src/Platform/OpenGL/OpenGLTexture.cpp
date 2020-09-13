#include "ilpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace IL
{

	OpenGLTexture2D::OpenGLTexture2D(const uint32_t& width, const uint32_t& height)
		:m_Width(width), m_Height(height)
	{
		IL_PROFILE_FUNCTION();

		// internalFormat used to store image data in the GPU, format used to explain the stored data
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		// create texture and create space inside GPU to storage image data
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path), m_UVScaling(1.0f)
	{
		IL_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			IL_PROFILE_SCOPE("{stbi_load} OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		IL_CORE_ASSERT(data, "Load texture faliure!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, format = 0; // internalFormat used to store image data in the GPU, format used to explain the stored data
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
		}
		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			format = GL_RGB;
		}
		IL_CORE_ASSERT(internalFormat & format, "Unknown texture format!");

		// create texture and create space inside GPU to storage image data
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// upload texture
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);

		// we have uploaded the data to the GPU, we don't need data inside CPU
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		IL_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		IL_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		IL_CORE_ASSERT(size == bpp * m_Width * m_Height, "Data must be entire texture!");
		// upload texture
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot = 0) const
	{
		IL_PROFILE_FUNCTION();

		glBindTextureUnit(slot /* slot id */, m_RendererID);
	}
}