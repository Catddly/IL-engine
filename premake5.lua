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
<<<<<<< HEAD
		runtime "Debug"
=======
		buildoptions "/MDd"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
		symbols "On"

	filter "configurations:Release"
		defines "IL_RELEASE"
<<<<<<< HEAD
		runtime "Release"
=======
		buildoptions "/MD"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
		optimize "On"

	filter "configurations:Dist"
		defines "IL_DIST"
<<<<<<< HEAD
		runtime "Release"
		optimize "On"

IncludeDir = { }
IncludeDir["GLFW"] =  "IL/vendor/GLFW/include"
IncludeDir["glad"] =  "IL/vendor/glad/include"
IncludeDir["ImGui"] = "IL/vendor/imgui"

include "IL/vendor/GLFW"
include "IL/vendor/glad"
include "IL/vendor/imgui"
=======
		buildoptions "/MD"
		optimize "On"

IncludeDir = { }
IncludeDir["GLFW"] = "IL/vendor/GLFW/include"
IncludeDir["glad"] = "IL/vendor/glad/include"

include "IL/vendor/GLFW"
include "IL/vendor/glad"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c

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
<<<<<<< HEAD
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}"
=======
		"%{IncludeDir.glad}"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
	}

	links
	{
		"GLFW",
		"glad",
<<<<<<< HEAD
		"ImGui",
=======
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
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
<<<<<<< HEAD
		runtime "Debug"
=======
		buildoptions "/MDd"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
		symbols "On"

	filter "configurations:Release"
		defines "IL_RELEASE"
<<<<<<< HEAD
		runtime "Release"
=======
		buildoptions "/MD"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
		optimize "On"

	filter "configurations:Dist"
		defines "IL_DIST"
<<<<<<< HEAD
		runtime "Release"
=======
		buildoptions "/MD"
>>>>>>> cb6b5c1e64e11bf787d931c5eaf05ffaec344d0c
		optimize "On"

