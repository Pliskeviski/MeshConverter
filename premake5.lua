project "Converter"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "Convert.cpp",
        "Convert.h",
		"deps/glm/**.hpp",
		"deps/glm/**.inl",
    }
	
	includedirs
	{
		"deps/Include"
	}

	libdirs 
	{ 
		"deps/Lib"
	}
	
	links 
	{ 
		"assimp-vc140-mt.lib"
	}
    
	filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "10.0.17134.0"
        staticruntime "On"

		defines 
		{ 
            "_CRT_SECURE_NO_WARNINGS"
		}
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"