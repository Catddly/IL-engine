workspace "IL"
	architecture "x64"
	startproject "IL-Editor"

	configurations 
	{
		"Debug", 
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = { }
IncludeDir["GLFW"] =  "IL/vendor/GLFW/include"
IncludeDir["glad"] =  "IL/vendor/glad/include"
IncludeDir["ImGui"] = "IL/vendor/imgui"
IncludeDir["glm"] = "IL/vendor/glm"
IncludeDir["stb_image"] = "IL/vendor/stb_image"
IncludeDir["entt"] = "IL/vendor/entt/include"

group "Dependencies"

	include "IL/vendor/GLFW"
	include "IL/vendor/glad"
	include "IL/vendor/imgui"

group ""

project "IL"
	location "IL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir	  ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ilpch.h"
	pchsource "IL/src/ilpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS",
			"IL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines 
		{
			"IL_DEBUG",
			"IL_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "IL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "IL_DIST"
		runtime "Release"
		optimize "on"

		
project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"IL/vendor/spdlog/include",
		"IL/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"IL"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "IL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "IL_DIST"
		runtime "Release"
		optimize "on"

project "IL-Editor"
	location "IL-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"IL/vendor/spdlog/include",
		"IL/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"IL"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "IL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "IL_DIST"
		runtime "Release"
		optimize "on"