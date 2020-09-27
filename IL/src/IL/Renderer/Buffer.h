#pragma once

namespace IL
{
	
	enum class ShaderDataType
	{
		None = 0, 
		Float, Float2, Float3, Float4, 
		Mat3, Mat4, 
		Int, Int2, Int3, Int4, 
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case IL::ShaderDataType::None:   return 0;
			case IL::ShaderDataType::Float:  return 4;
			case IL::ShaderDataType::Float2: return 4 * 2;
			case IL::ShaderDataType::Float3: return 4 * 3;
			case IL::ShaderDataType::Float4: return 4 * 4;
			case IL::ShaderDataType::Mat3:   return 4 * 3 * 3;
			case IL::ShaderDataType::Mat4:   return 4 * 4 * 4;
			case IL::ShaderDataType::Int:    return 4;
			case IL::ShaderDataType::Int2:   return 4 * 2;
			case IL::ShaderDataType::Int3:   return 4 * 3;
			case IL::ShaderDataType::Int4:   return 4 * 4;
			case IL::ShaderDataType::Bool:   return 1;
		}

		IL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(ShaderDataType type, std::string name, bool normalized = false)
			: Type(type), Name(name), Offset(0), Size(ShaderDataTypeSize(type)), Normalized(normalized) {}

		uint32_t GetComponentCount() const 
		{
			switch (Type)
			{
				case IL::ShaderDataType::None:   return 0;
				case IL::ShaderDataType::Float:  return 1;
				case IL::ShaderDataType::Float2: return 2;
				case IL::ShaderDataType::Float3: return 3;
				case IL::ShaderDataType::Float4: return 4;
				case IL::ShaderDataType::Mat3:   return 3 * 3;
				case IL::ShaderDataType::Mat4:	 return 4 * 4;
				case IL::ShaderDataType::Int:	 return 1;
				case IL::ShaderDataType::Int2:	 return 2;
				case IL::ShaderDataType::Int3:	 return 3;
				case IL::ShaderDataType::Int4:	 return 4;
				case IL::ShaderDataType::Bool:	 return 1;
			}

			IL_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(std::initializer_list<BufferElement> elements) 
			:m_Elements(elements)
		{
			CalOffsetAndStrike();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStrike() const { return m_Strike; }

		std::vector<BufferElement>::iterator       begin()       { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator       end()         { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.cbegin(); }
		std::vector<BufferElement>::const_iterator end()   const { return m_Elements.cend(); }
	private:
		void CalOffsetAndStrike()
		{
			uint32_t offset = 0;
			m_Strike = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Strike += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Strike;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual inline uint32_t GetCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};

}