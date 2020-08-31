#include "ilpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace IL
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
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

		glTexParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// upload texture
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);

		// we have uploaded the data to the GPU, we don't need data inside CPU
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot = 0) const
	{
		glBindTextureUnit(slot /* slot id */, m_RendererID);
	}
}