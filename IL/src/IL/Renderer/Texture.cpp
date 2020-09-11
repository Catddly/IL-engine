#include "ilpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlTexture.h"

namespace IL
{

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

	IL::Ref<IL::Texture2D> Texture2D::Create(const uint32_t& width, const uint32_t& height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}