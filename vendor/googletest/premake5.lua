project "googletest"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "")
	objdir ("bin-int/" .. outputdir .. "")
	

	files
	{
		"src/**.*",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	includedirs
	{
		"include",
		""
	}

	filter "system:windows"
		systemversion "latest"
		defines "GTEST_OS_WINDOWS"
		
	filter "system:linux"
		pic "On"
		defines {
			"GTEST_OS_LINUX",
			"GTEST_HAS_RTTI"
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		optimize "off"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
		symbols "on"

	filter "configurations:Dist"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
		symbols "off"
