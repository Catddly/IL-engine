#include "ilpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace IL
{

	static GLenum ShaderDataType2GLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case IL::ShaderDataType::Float:
		case IL::ShaderDataType::Float2:
		case IL::ShaderDataType::Float3:
		case IL::ShaderDataType::Float4:
		case IL::ShaderDataType::Mat3:
		case IL::ShaderDataType::Mat4:   return GL_FLOAT;
		case IL::ShaderDataType::Int:
		case IL::ShaderDataType::Int2:
		case IL::ShaderDataType::Int3:
		case IL::ShaderDataType::Int4:	 return GL_INT;
		case IL::ShaderDataType::Bool:   return GL_BOOL;
		}

		IL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		// Initialize draw call data of OpenGL
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		IL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Empty vertexBuffer layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			// explain the data to the GPU
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataType2GLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStrike(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}