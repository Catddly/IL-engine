#include "ilpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include "IL/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace IL
{

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			IL_CORE_ASSERT(false, "IL engine don't support null API now!"); return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		IL_CORE_ASSERT(false, "Wrong API selection!");
		return nullptr;
	}

}
