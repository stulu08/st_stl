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
		buildoptions {
			"-GS", "-W4", "-WX", "-wd4251", "-wd4275", "-nologo", "-J",
			"-D_UNICODE", "-DUNICODE", "-DWIN32", "-D_WIN32",
			"-EHs-c-", "-D_HAS_EXCEPTIONS=0", "-GR-", "-wd4702", "-utf-8"
		}
		
	filter "system:linux"
		pic "On"
		defines {
			"GTEST_OS_LINUX",
			"GTEST_HAS_RTTI"
		}
		buildoptions {
			"-fno-exceptions", "-Wall", "-Wshadow", "-Wno-error=dangling-else",
			"-Wextra", "-Wno-unused-parameter", "-Wno-missing-field-initializers"
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
