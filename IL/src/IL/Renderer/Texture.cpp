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
			return std::make_shared<OpenGLTexture2D>(path);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}