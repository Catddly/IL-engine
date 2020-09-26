#pragma once

#include "IL/Renderer/FrameBuffer.h"

namespace IL
{

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferProps& props);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FrameBufferProps& GetProperties() const override { return m_Props; }
	private:
		FrameBufferProps m_Props;
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
	};

}