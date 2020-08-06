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
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "IL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "IL_DIST"
		optimize "On"

IncludeDir = { }
IncludeDir["GLFW"] = "IL/vendor/GLFW/include"

include "IL/vendor/GLFW"

project "IL"
	location "IL"
	kind "SharedLib"
	language "C++"

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
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"IL_PLATFORM_WINDOWS",
			"IL_BUILD_DLL"
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
		symbols "On"

	filter "configurations:Release"
		defines "IL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "IL_DIST"
		optimize "On"

