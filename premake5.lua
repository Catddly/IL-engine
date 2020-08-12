workspace "IL"
	architecture "x64"

	configurations 
	{
		"Debug", 
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"IL/vendor/spdlog/include",
		"IL/src"
	}

	links
	{
		"IL"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "IL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "IL_DIST"
		runtime "Release"
		optimize "On"

IncludeDir = { }
IncludeDir["GLFW"] =  "IL/vendor/GLFW/include"
IncludeDir["glad"] =  "IL/vendor/glad/include"
IncludeDir["ImGui"] = "IL/vendor/imgui"

include "IL/vendor/GLFW"
include "IL/vendor/glad"
include "IL/vendor/imgui"

project "IL"
	location "IL"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir	  ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ilpch.h"
	pchsource "IL/src/ilpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS",
			"IL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath}  ../bin/" .. outputdir .. "/SandBox")
		}

	filter "configurations:Debug"
		defines 
		{
			"IL_DEBUG",
			"IL_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "IL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "IL_DIST"
		runtime "Release"
		optimize "On"

