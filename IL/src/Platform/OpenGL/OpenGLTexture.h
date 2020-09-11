#pragma once

#include "IL/Renderer/Texture.h"

#include <glad/glad.h>

namespace IL
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const uint32_t& width, const uint32_t& height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override  { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot) const override;
	private:
		std::string m_Path;

		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		float m_UVScaling;
		GLenum m_InternalFormat, m_DataFormat;
	};

}