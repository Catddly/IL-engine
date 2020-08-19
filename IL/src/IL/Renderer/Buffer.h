#pragma once

namespace IL
{

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Run() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual inline uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};

}